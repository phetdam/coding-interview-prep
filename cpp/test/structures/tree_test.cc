/**
 * @file tree_test.cc
 * @author Derek Huang
 * @brief Unit tests for the tree classes defined in structures/tree.cc
 * @copyright MIT License
 */

#include <memory>

#include <gtest/gtest.h>

#include "pdcip/cpp/structures/tree.h"

namespace pdcip {
namespace tests {
namespace structures {

TEST(TreeTests, DummyTest)
{
  pdcip::structures::tree_children_ptr
  children = pdcip::structures::make_children<pdcip::structures::tree>(
    {1.0, 5.6, 9.8}
  );
  ASSERT_EQ(children->at(0), children->at(0));
}

}  // namespace pdcip::tests::structures
}  // namespace pdcip::tests
}  // namespace pdcip
