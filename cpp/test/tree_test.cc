/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree classes defined in tree.cc
 * @copyright MIT License
 */

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pdcip/cpp/tree.h"
#include "pdcip/cpp/types.h"

#ifdef _MSC_VER
#pragma warning(disable : 4514 4625 4626 5026 5027)
#endif  /* _MSC_VER */

namespace pdcip {
namespace tests {

namespace {

/**
 * Test fixture class managing commonly reused `tree` objects.
 */
class TreeTest : public ::testing::Test {
protected:
  TreeTest() : root_(std::make_shared<tree>(root_value_)) {}

  /**
   * Set up the `TreeTest` instance per test.
   *
   * Performs some checking of the initialized members.
   */
  void SetUp() override
  {
    ASSERT_EQ(root_->value(), root_value_);
    ASSERT_EQ(0, root_->n_children());
  }

  /**
   * Return `tree` children using `tree::make_children`.
   *
   * We often reuse this method for quickly constructing tiered trees.
   */
  tree_ptr_vector_ptr make_test_children()
  {
    return tree::make_children(test_children_values_);
  }

  /**
   * Set the children of `root_` to those from `make_test_children`.
   */
  void give_root_direct() { root_->set_children(make_test_children()); }

  /**
   * Set the children of `root_` to be a subtree from `make_test_children`.
   */
  void give_root_subtree()
  {
    tree_ptr_vector_ptr first_children = make_test_children();
    tree_ptr_vector_ptr second_children = make_test_children();
    root_->set_children(first_children);
    first_children->at(1)->set_children(second_children);
  }

  /**
   * Return number of children in `test_children_values`.
   */
  size_t n_test_children() const
  {
    return static_cast<size_t>(test_children_values_.size());
  }

  const tree_ptr root_;

  // initialization values for root_ and make_test_children return
  static const double root_value_;
  static const double_vector test_children_values_;
};

const double TreeTest::root_value_ = 5.7;
const double_vector TreeTest::test_children_values_({4.5, 1.3, 6.5, 9, 8.7});

/**
 * Test that created children have the right values and have no children.
 */
TEST_F(TreeTest, MakeChildrenTest)
{
  tree_ptr_vector_ptr children = make_test_children();
  for (std::size_t i = 0; i < children->size(); i++) {
    ASSERT_DOUBLE_EQ(test_children_values_[i], children->at(i)->value());
    ASSERT_EQ(0, children->at(i)->n_children());
  }
}

/**
 * Test that depth-first search yields the expected result.
 */
TEST_F(TreeTest, DepthFirstSearchTest)
{
  give_root_subtree();
  tree_ptr_vector_ptr nodes = pdcip::tree::dfs(root_);
  // expected values from test_children_values_ and root_value_. values:
  //
  // {
  //   test_children_values_[0],
  //   test_children_values_[0],
  //   /* ... */
  //   test_children_values_[test_children_values_.size() - 1],
  //   test_children_values_[1],
  //   /* ... */
  //   test_children_values_[test_children_values_.size() - 1],
  //   root_value_
  //  }
  //
  double_vector true_values(test_children_values_);
  auto true_iterator = true_values.begin();
  std::advance(true_iterator, 1);
  true_values.insert(
    true_iterator, test_children_values_.begin(), test_children_values_.end()
  );
  true_values.push_back(root_value_);
  ASSERT_EQ(true_values.size(), nodes->size());
  for (std::size_t i = 0; i < nodes->size(); i++) {
    ASSERT_DOUBLE_EQ(true_values[i], nodes->at(i)->value());
  }
}

/**
 * Test that breadth-first search yields the expected result.
 */
TEST_F(TreeTest, BreadthFirstSearchTest)
{
  give_root_subtree();
  tree_ptr_vector_ptr nodes = pdcip::tree::bfs(root_);
  // expected values from test_children_values_ and root_value_. values:
  //
  // {
  //   root_value_,
  //   test_children_values_[0],
  //   /* ... */
  //   test_children_values_[test_children_values_.size() - 1],
  //   test_children_values_[0],
  //   /* ... */
  //   test_children_values_[test_children_values_.size() - 1]
  //  }
  //
  double_vector true_values({root_value_});
  for (std::size_t i = 0; i < 2; i++) {
    true_values.insert(
      true_values.end(),
      test_children_values_.begin(),
      test_children_values_.end()
    );
  }
  ASSERT_EQ(true_values.size(), nodes->size());
  for (std::size_t i = 0; i < nodes->size(); i++) {
    ASSERT_DOUBLE_EQ(true_values[i], nodes->at(i)->value());
  }
}

/**
 * Test fixture class managinc commonly reused `binary_tree` objects.
 */
class BinaryTreeTest : public ::testing::Test {
protected:
  /**
   * `BinaryTreeTest` construtor that make `root_` an empty `binary_tree_ptr`.
   */
  BinaryTreeTest() : root_(std::make_shared<binary_tree>()) {}

  /**
   * Per-test setup function that serves to check `root_` initialization.
   */
  void SetUp() override
  {
    ASSERT_TRUE(std::isnan(root_->value()));
    ASSERT_EQ(nullptr, root_->left());
    ASSERT_EQ(nullptr, root_->right());
  }

  /**
   * Per-suite (class) setup function.
   *
   * We use this to setup `tree_values_sorted_`.
   */
  static void SetUpTestSuite()
  {
    tree_values_sorted_ = tree_values_;
    std::sort(tree_values_sorted_.begin(), tree_values_sorted_.end());
  }

  /**
   * Return the value of `root_` when it is populated with `tree_values_`.
   *
   * This is just the first value in `tree_values_`.
   */
  double root_value() const { return tree_values_[0]; }

  /**
   * Insert `tree_values_` into `root_`.
   */
  void root_insert_values() {
    for (double value : tree_values_) {
      root_->insert(value);
    }
  }

  const binary_tree_ptr root_;

  // values used for a full constructor call
  static const double_pair init_pair_;
  // values used to initialize the tree at root_ with values + sorted version.
  // note sorted version cannot be const since we need to assign to it
  static const double_vector tree_values_;
  static double_vector tree_values_sorted_;
};

const double_pair BinaryTreeTest::init_pair_(2, 5.1);
const double_vector BinaryTreeTest::tree_values_ = {4.5, 1.3, 6.5, 9, 8.1};
// cannot leave uninitialized, so just make it an empty
double_vector BinaryTreeTest::tree_values_sorted_;

/**
 * Test that `binary_tree` full construction works as expected.
 */
TEST_F(BinaryTreeTest, FullInitTest)
{
  binary_tree root = binary_tree(
    root_value(),
    std::make_shared<binary_tree>(init_pair_.first),
    std::make_shared<binary_tree>(init_pair_.second)
  );
  ASSERT_DOUBLE_EQ(root_value(), root.value());
  ASSERT_DOUBLE_EQ(init_pair_.first, root.left()->value());
  ASSERT_DOUBLE_EQ(init_pair_.second, root.right()->value());
  ASSERT_EQ(nullptr, root.left()->left());
  ASSERT_EQ(nullptr, root.left()->right());
  ASSERT_EQ(nullptr, root.right()->left());
  ASSERT_EQ(nullptr, root.right()->right());
}

/**
 * Test that `binary_tree` value insertion works as expected.
 */
TEST_F(BinaryTreeTest, InsertValuesTest)
{
  root_insert_values();
  // the resulting tree from inserting tree_values_ is:
  //
  //            4.5
  //           +   +
  //          1.3  6.5
  //                 +
  //                  9
  //                 +
  //                8.1
  //
  ASSERT_DOUBLE_EQ(root_value(), root_->value());
  ASSERT_DOUBLE_EQ(tree_values_[1], root_->left()->value());
  ASSERT_DOUBLE_EQ(tree_values_[2], root_->right()->value());
  ASSERT_DOUBLE_EQ(tree_values_[3], root_->right()->right()->value());
  ASSERT_DOUBLE_EQ(tree_values_[4], root_->right()->right()->left()->value());
}

/**
 * Test that `binary_tree::sorted_values` works as expected.
 */
TEST_F(BinaryTreeTest, SortedValuesTest)
{
  root_insert_values();
  ASSERT_EQ(tree_values_sorted_, *(root_->sorted_values()));
}

/**
 * Test that depth-first search gives the expected result with `binary_tree`.
 */
TEST_F(BinaryTreeTest, DepthFirstSearchTest)
{
  root_insert_values();
  // values retrieved by depth-first search are {1.3, 8.1, 9, 6.5, 4}
  double_vector true_values(tree_values_.size());
  true_values[0] = tree_values_[1];
  true_values[1] = tree_values_[4];
  true_values[2] = tree_values_[3];
  true_values[3] = tree_values_[2];
  true_values[4] = tree_values_[0];
  ASSERT_EQ(true_values, *tree::value_vector(tree::dfs(root_)));
}

/**
 * Test that breadth-first search gives the expected result with `binary_tree`.
 */
TEST_F(BinaryTreeTest, BreadthFirstSearchTest)
{
  root_insert_values();
  // values retrieved by breadth-first search happen to match the insertion
  // order, so we can directly compare against tree_values_
  ASSERT_EQ(tree_values_, *tree::value_vector(tree::bfs(root_)));
}

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
