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

TEST(TreeTests, DummyTest)
{
  pdcip::tree_children_ptr
  children = pdcip::make_tree_children<>({1.0, 5.6, 9.8});
  ASSERT_EQ(children->at(0), children->at(0));
}

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
