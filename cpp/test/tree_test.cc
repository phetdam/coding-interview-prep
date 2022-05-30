/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree classes defined in tree.cc
 * @copyright MIT License
 */

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "pdcip/cpp/tree.h"
#include "pdcip/cpp/types.h"

namespace pdcip {
namespace tests {

namespace {

/**
 * Test fixture class managing commonly reused `tree` objects.
 */
class TreeTest: public ::testing::Test {
protected:
  TreeTest() : root_(std::make_shared<tree>(root_value_)) {}

  /**
   * Set up the `TreeTest` instance.
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
    return pdcip::tree::make_children(test_children_values_);
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

  tree_ptr root_;

  // initialization values for root_ and make_test_children return
  static double root_value_;
  static double_vector test_children_values_;
};

double TreeTest::root_value_ = 5.7;
double_vector TreeTest::test_children_values_({4.5, 1.3, 6.5, 9, 8.7});

/**
 * Test that created children have the right values and have no children.
 */
TEST_F(TreeTest, MakeChildrenTest)
{
  pdcip::tree_ptr_vector_ptr children = make_test_children();
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
  pdcip::double_vector true_values(test_children_values_);
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
  pdcip::double_vector true_values({root_value_});
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
 * Test that `binary_tree` insertion works as expected.
 */
TEST(BinaryTreeTests, SortedValuesTest)
{
  std::vector<double> values({4.5, 1.3, 6.5, 9});
  std::vector<double> values_sorted = values;
  std::sort(values_sorted.begin(), values_sorted.end());
  pdcip::binary_tree tree;
  for (auto x : values) {
    tree.insert(x);
  }
  ASSERT_EQ(values_sorted, *(tree.sorted_values()));
}

/**
 * Test that depth-first search yields the expected result with `binary_tree`.
 */

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
