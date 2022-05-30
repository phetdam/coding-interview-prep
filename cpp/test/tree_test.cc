/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree classes defined in tree.cc
 * @copyright MIT License
 */

#include <algorithm>
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
  ASSERT_EQ(11, nodes->size());
  pdcip::double_vector true_values = {
    4.5, 4.5, 1.3, 6.5, 9, 8.7, 1.3, 6.5, 9, 8.7, 5.7
  };
  for (size_t i = 0; i < nodes->size(); i++) {
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

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
