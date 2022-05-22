/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree classes defined in structures/tree.cc
 * @copyright MIT License
 */

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
  }
}

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
