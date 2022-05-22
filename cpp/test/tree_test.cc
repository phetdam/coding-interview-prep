/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree classes defined in structures/tree.cc
 * @copyright MIT License
 */

#include <algorithm>
#include <memory>

#include <gtest/gtest.h>

#include "pdcip/cpp/tree.h"

namespace pdcip {
namespace tests {

namespace {

/**
 * Test that created children have the right values and have no children.
 */
TEST(TreeTests, MakeChildrenTest)
{
  std::vector<double> values({1.0, 5.6, 9.8});
  pdcip::tree_children_ptr children = pdcip::tree::make_children(values);
  for (std::size_t i = 0; i < children->size(); i++) {
    EXPECT_DOUBLE_EQ(values[i], children->at(i)->value());
    EXPECT_EQ(nullptr, children->at(i)->children());
  }
}

/**
 * Test that `binary_tree` insertion works as expected.
 */
TEST(BinaryTreeTest, SortedValuesTest)
{
  std::vector<double> values({4.5, 1.3, 6.5, 9});
  std::vector<double> values_sorted = values;
  std::sort(values_sorted.begin(), values_sorted.end());
  pdcip::binary_tree btree;
  for (auto x : values) {
    btree.insert(x);
  }
  EXPECT_EQ(values_sorted, btree.sorted_values());
}

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
