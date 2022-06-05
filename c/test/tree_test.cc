/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree structs defined in tree.c
 * @copyright MIT License
 */

#include "pdcip/tree.h"

#include <stdlib.h>

#include <iterator>
#include <vector>

#include <gtest/gtest.h>

#include "pdcip/helpers.h"

#ifdef _MSC_VER
#pragma warning (disable: 4625 4626)
#endif  /* _MSC_VER */

/**
 * @note Google naming convention is followed as no underscores allowed.
 * @note Google Test documentation recommends testing if a pointer if `NULL`
 *  by using `ASSERT_EQ(nullptr, the_ptr)`, not `ASSERT_EQ(NULL, the_ptr)`.
 */

namespace {

/**
 * Test fixture for `gen_tree` tests that require a base root and children.
 *
 * Ctor and dtor manage `malloc` + `free` for `gen_tree*` `root_`, and clean
 * up the children if there are children still attached.
 *
 * @note `SetUp` method is used since we use `ASSERT_*` macros to do some
 *    checks. These `ASSERT_*` macros can't be used in ctor/dtor.
 */
class GenTreeTest : public ::testing::Test {
protected:
  GenTreeTest() : root_(gen_tree_malloc_default(root_value_)) {}

  ~GenTreeTest() {
    // already checks if children is NULL
    gen_tree_free_children_deep(root_);
    gen_tree_free(root_);
  }

  /**
   * Set up the `GenRootTest` fixture instance.
   *
   * May cause assertion error if `root_` is not malloc'd correctly.
   */
  void SetUp() override
  {
    ASSERT_DOUBLE_EQ(root_value_, root_->value);
    ASSERT_EQ(0, root_->n_children);
    ASSERT_EQ(nullptr, root_->children);
  }

  /**
   * Return pointer to array of new `gen_tree *` children.
   *
   * Values are populated from `direct_values_`.
   *
   * @note Children must be freed later with `gen_tree_free_children` if bound
   *    as children of `gen_tree *` or manually freed later!
   */
  gen_tree** make_direct_children()
  {
    return gen_tree_make_children(n_direct_children(), direct_values_.data());
  }

  /**
   * Add direct children from `make_direct_children` to `root_`.
   *
   * @note Do not call any `::malloc_` methods after calling until
   *    `free_direct_children` has been called to free the memory.
   */
  void malloc_direct_children()
  {
    gen_tree_set_children(root_, n_direct_children(), make_direct_children());
  }

  /**
   * Free direct children added to `root_`.
   *
   * Also checks if `root_` has had its state properly reset.
   *
   * @note Do not call unless cleaning up after `malloc_direct_children` call.
   */
  void free_direct_children()
  {
    gen_tree_free_children(root_);
    ASSERT_EQ(0, root_->n_children);
    ASSERT_EQ(nullptr, root_->children);
  }

  /**
   * Add subtree using `make_direct_children` to `root_`.
   *
   * First layer of children is from `make_direct_children`, and the second
   * leftmost child in the first layer of children also has children from a
   * call to the `make_direct_children` method.
   *
   * @note Do not call any `::malloc` metods after calling until `free_subtree`
   *    has been called to free the memory used.
   */
  void malloc_subtree()
  {
    gen_tree** first_children = make_direct_children();
    gen_tree** second_children = make_direct_children();
    gen_tree_set_children(root_, n_direct_children(), first_children);
    gen_tree_set_children(
      first_children[1], n_direct_children(), second_children
    );
  }

  /**
   * Free subtree added to `root_`.
   *
   * Also checks if `root_` has had its state properly reset.
   *
   * @note Do not call unless cleaning up after `malloc_subtree`.
   */
  void free_subtree()
  {
    gen_tree_free_children_deep(root_);
    ASSERT_EQ(0, root_->n_children);
    ASSERT_EQ(nullptr, root_->children);
  }

  /**
   * Return number of direct children made by `make_direct_children`.
   */
  size_t n_direct_children() const { return direct_values_.size(); }

  gen_tree* root_;

  // initialization values for root_, make_direct_children return
  static const double root_value_;
  static const std::vector<double> direct_values_;
};

const double GenTreeTest::root_value_ = 1.7;
const std::vector<double> GenTreeTest::direct_values_({4.5, 1.8, 8.9, 2, 6.5});

/**
 * Test that making and freeing direct `gen_tree` children works as intended.
 *
 * @note Won't leak memory on failure since `GenTreeTest` dtor will clean up.
 */
TEST_F(GenTreeTest, MakeFreeChildrenTest)
{
  malloc_direct_children();
  for (unsigned int i = 0; i < n_direct_children(); i++) {
    ASSERT_DOUBLE_EQ(direct_values_.data()[i], root_->children[i]->value);
    ASSERT_EQ(0, root_->children[i]->n_children);
    ASSERT_EQ(nullptr, root_->children[i]->children);
  }
  free_direct_children();
}

/**
 * Test that making and freeing `gen_tree` subtrees works as intended.
 */
TEST_F(GenTreeTest, MakeFreeChildrenDeepTest)
{
  malloc_subtree();
  free_subtree();
}

/**
 * Test that making and shallow freeing `gen_tree` child arrays works.
 */
TEST_F(GenTreeTest, MakeFreeChildrenArrayTest)
{
  gen_tree** children = make_direct_children();
  gen_tree_free_children_array(children, n_direct_children());
}

/**
 * Test that making and deep freeing `gen_tree` child arrays works.
 */
TEST_F(GenTreeTest, MakeFreeChildrenArrayDeepTest)
{
  gen_tree** children = make_direct_children();
  for (unsigned i = 0; i < (n_direct_children() / 2); i++) {
    children[i]->children = make_direct_children();
  }
  gen_tree_free_children_array_deep(children, n_direct_children());
}

/**
 * Test that depth-first search on `gen_tree` works as expected.
 */
TEST_F(GenTreeTest, DepthFirstSearchTest)
{
  malloc_subtree();
  // write n_nodes, get array of gen_tree*
  size_t n_nodes;
  gen_tree** root_nodes = gen_tree_dfs(root_, &n_nodes);
  // if NULL, then nothing to free
  ASSERT_TRUE(root_nodes);
  // expected values, formed from direct_values_ and root_value_. values:
  //
  // {
  //   direct_values_[0],
  //   direct_values_[0],
  //   /* ... */
  //   direct_values_[direct_values_.size() - 1],
  //   direct_values_[1],
  //   /* ... */
  //   direct_values_[direct_values_.size() - 1],
  //   root_value_
  //  }
  //
  std::vector<double> root_node_values_exp(direct_values_);
  auto root_node_iter_exp = root_node_values_exp.begin();
  std::advance(root_node_iter_exp, 1);
  root_node_values_exp.insert(
    root_node_iter_exp, direct_values_.begin(), direct_values_.end()
  );
  root_node_values_exp.push_back(root_value_);
  // actual values, populate from root_nodes
  std::vector<double> root_node_values_act(n_nodes);
  for (unsigned i = 0; i < n_nodes; i++) {
    root_node_values_act[i] = root_nodes[i]->value;
  }
  // no longer need the copied gen_tree * and we clean up the subtree
  free(root_nodes);
  free_subtree();
  // now we can do our comparison
  ASSERT_EQ(root_node_values_exp.size(), n_nodes);
  for (unsigned i = 0; i < n_nodes; i++) {
    ASSERT_DOUBLE_EQ(root_node_values_exp[i], root_node_values_act[i]);
  }
}

/**
 * Test fixture for `binary_tree` tests that require a base root.
 *
 * Ctor and dtor manage `malloc` + `free` for `binary_tree*` `root_`, and clean
 * up the child subtrees if there are still children attached.
 *
 * See `GenRootTest` definition for notes.
 */
class BinaryTreeTest : public ::testing::Test {
protected:
  BinaryTreeTest() : root_(binary_tree_malloc_default(5)) {}
  ~BinaryTreeTest() {
    binary_tree_free_children_deep(root_);
    binary_tree_free(root_);
  }

  /**
   * Set up the `BinaryTreeTest` fixture instance.
   *
   * May cause assertion error if `root_` is not malloc'd correctly.
   */
  void SetUp() override
  {
    ASSERT_DOUBLE_EQ(5, root_->value);
    ASSERT_EQ(nullptr, root_->left);
    ASSERT_EQ(nullptr, root_->right);
  }

  binary_tree* root_;
};

/**
 * Test that making and freeing direct `binary_tree` children works as intended.
 */
TEST_F(BinaryTreeTest, MakeFreeChildrenTest)
{
  root_->left = binary_tree_malloc_default(2.3);
  root_->right = binary_tree_malloc_default(4.6);
  binary_tree_free_children(root_);
  EXPECT_EQ(nullptr, root_->left);
  EXPECT_EQ(nullptr, root_->right);
}

/**
 * Test that making and freeing `binary_tree` subtrees works as intended.
 */
TEST_F(BinaryTreeTest, MakeFreeChildrenDeepTest)
{
  root_->left = binary_tree_malloc(
    4.5, binary_tree_malloc_default(3), binary_tree_malloc_default(4.9)
  );
  root_->right = binary_tree_malloc(
    5.6,
    NULL,
    binary_tree_malloc(
      8, binary_tree_malloc_default(7.2), binary_tree_malloc_default(9)
    )
  );
  binary_tree_free_children_deep(root_);
  EXPECT_EQ(nullptr, root_->left);
  EXPECT_EQ(nullptr, root_->right);
}

/**
 * Test that `binary_tree_sorted_values` works as expected.
 *
 * @note Leaks memory on failure due to checking inside loop.
 */
TEST_F(BinaryTreeTest, SortedValuesTest)
{
  root_->left = binary_tree_malloc(
    4.5,
    binary_tree_malloc(
      3, binary_tree_malloc_default(2.7), binary_tree_malloc_default(3.3)
    ),
    binary_tree_malloc_default(4.9)
  );
  root_->right = binary_tree_malloc(
    5.6,
    NULL,
    binary_tree_malloc(
      8, binary_tree_malloc_default(7.2), binary_tree_malloc_default(9)
    )
  );
  size_t n_values_act;
  double* values_act = binary_tree_sorted_values(root_, &n_values_act);
  // recall that root_->value is 5
  double values_exp[] = {2.7, 3, 3.3, 4.5, 4.9, 5, 5.6, 7.2, 8, 9};
  // clean up now to reduce memory leak (values have been copied)
  binary_tree_free_children_deep(root_);
  // if size doesn't even match, clearly a mistake has been made
  ASSERT_EQ(10, n_values_act);
  for (unsigned int i = 0; i < n_values_act; i++) {
    ASSERT_DOUBLE_EQ(values_exp[i], values_act[i]);
  }
}

}  // namespace
