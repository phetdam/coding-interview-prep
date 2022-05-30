/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree structs defined in tree.c
 * @copyright MIT License
 */

#include <stdlib.h>

#include <vector>

#include <gtest/gtest.h>

#include "pdcip/tree.h"

/**
 * @note Google naming convention is followed as no underscores allowed.
 * @note Google Test documentation recommends testing if a pointer if `NULL`
 *  by using `ASSERT_EQ(nullptr, the_ptr)`, not `ASSERT_EQ(NULL, the_ptr)`.
 */

namespace {

/**
 * Test fixture for `gen_tree` tests that require a base root and children.
 *
 * Ctor and dtor manage `malloc` + `free` for `gen_tree*` `root_`.
 *
 * @note `SetUp` method is used since we use `ASSERT_*` macros to do some
 *    checks. These `ASSERT_*` macros can't be used in ctor/dtor.
 */
class GenTreeTest: public ::testing::Test {
protected:
  GenTreeTest()
    : root_(gen_tree_malloc_default(1.7)),
      direct_values_({4.5, 1.8, 8.9, 2, 6.5})
  {}

  ~GenTreeTest() { gen_tree_free(root_); }

  /**
   * Set up the `GenRootTest` fixture instance.
   *
   * May cause assertion error if `root_` is not malloc'd correctly.
   */
  void SetUp() override
  {
    ASSERT_DOUBLE_EQ(1.7, root_->value);
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
  gen_tree** gen_tree_make_direct_children()
  {
    return gen_tree_make_children(n_direct_children(), direct_values_.data());
  }

  /**
   * Return number of direct children made by `gen_tree_make_direct_children`.
   */
  size_t n_direct_children() const { return direct_values_.size(); }

  gen_tree* root_;
  const std::vector<double> direct_values_;
};

/**
 * Test fixture for `binary_tree` tests that require a base root.
 *
 * Ctor and dtor manage `malloc` + `free` for `bianry_tree*` `root_`.
 *
 * See `GenRootTest` definition for notes.
 */
class BinaryTreeTest: public ::testing::Test {
protected:
  BinaryTreeTest() : root_(binary_tree_malloc_default(5)) {}
  ~BinaryTreeTest() { binary_tree_free(root_); }

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
 * Test that making and freeing direct `gen_tree` children works as intended.
 *
 * @note Leaks memory on failure due to checking inside loop.
 */
TEST_F(GenTreeTest, MakeFreeChildrenTest)
{
  gen_tree **children = gen_tree_make_direct_children();
  for (unsigned int i = 0; i < n_direct_children(); i++) {
    ASSERT_DOUBLE_EQ(direct_values_.data()[i], children[i]->value);
    ASSERT_EQ(0, children[i]->n_children);
    ASSERT_EQ(nullptr, children[i]->children);
  }
  gen_tree_set_children(root_, n_direct_children(), children);
  gen_tree_free_children(root_);
  ASSERT_EQ(0, root_->n_children);
  ASSERT_EQ(nullptr, root_->children);
}

/**
 * Test that making and freeing `gen_tree` subtrees works as intended.
 */
TEST_F(GenTreeTest, MakeFreeChildrenDeepTest)
{
  gen_tree **first_children = gen_tree_make_direct_children();
  gen_tree **second_children = gen_tree_make_direct_children();
  gen_tree_set_children(root_, n_direct_children(), first_children);
  gen_tree_set_children(
    first_children[1], n_direct_children(), second_children
  );
  gen_tree_free_children_deep(root_);
  ASSERT_EQ(0, root_->n_children);
  ASSERT_EQ(nullptr, root_->children);
}

/**
 * Test that making and shallow freeing `gen_tree` child arrays works.
 */
TEST_F(GenTreeTest, MakeFreeChildrenArrayTest)
{
  gen_tree **children = gen_tree_make_direct_children();
  gen_tree_free_children_array(children, n_direct_children());
}

/**
 * Test that making and deep freeing `gen_tree` child arrays works.
 */
TEST_F(GenTreeTest, MakeFreeChildrenArrayDeepTest)
{
  gen_tree **children = gen_tree_make_direct_children();
  for (unsigned i = 0; i < (n_direct_children() / 2); i++) {
    children[i]->children = gen_tree_make_direct_children();
  }
  gen_tree_free_children_array_deep(children, n_direct_children());
}

/**
 * Test that making and freeing direct `binary_tree` children works as intended.
 */
TEST_F(BinaryTreeTest, MakeFreeChildrenTest)
{
  root_->left = binary_tree_malloc_default(2.3);
  root_->right = binary_tree_malloc_default(4.6);
  binary_tree_free_children(root_);
  ASSERT_EQ(nullptr, root_->left);
  ASSERT_EQ(nullptr, root_->right);
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
  ASSERT_EQ(nullptr, root_->left);
  ASSERT_EQ(nullptr, root_->right);
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
  double *values_act = binary_tree_sorted_values(root_, &n_values_act);
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
