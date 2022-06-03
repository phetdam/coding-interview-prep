/**
 * @file graph_test.cc
 * @author Derek Huang
 * @brief Unit tests for the graph-related classes in graph.cc
 * @copyright MIT License
 */

#include <memory>

#include <gtest/gtest.h>

#include "pdcip/cpp/graph.h"
#include "pdcip/cpp/types.h"

namespace pdcip {
namespace tests {

namespace {

/**
 * Test fixture for `edge` tests, with an `edge` and its `vertex` pointers.
 */
class EdgeTest : public ::testing::Test {
protected:
  /**
   * Constructor setting up the `vertex` instances and `edge`.
   *
   * @note Members are initialized in the order they are *declared* in the
   *    class, NOT in how they are listed in the initialization list.
   */
  EdgeTest()
    : vert_start_(std::make_shared<vertex>(start_value_)),
      vert_end_(std::make_shared<vertex>(end_value_)),
      edge_(std::make_shared<edge>(vert_start_, vert_end_, edge_weight_))
  {}

  /**
   * Setup function used to check the sanity of the fixtures.
   */
  void SetUp() override
  {
    ASSERT_EQ(start_value_, vert_start_->value());
    ASSERT_EQ(end_value_, vert_end_->value());
    ASSERT_NE(vert_start_, vert_end_);
    ASSERT_EQ(vert_start_, edge_->start());
    ASSERT_EQ(vert_end_, edge_->end());
    ASSERT_EQ(edge_weight_, edge_->weight());
  }

  const vertex_ptr vert_start_;
  const vertex_ptr vert_end_;
  edge_ptr edge_;
  static const double start_value_;
  static const double end_value_;
  static const double edge_weight_;
};

const double EdgeTest::start_value_ = 10;
const double EdgeTest::end_value_ = 4;
const double EdgeTest::edge_weight_ = 1;

/**
 * Test that `edge::connects` works as expected.
 *
 * @note `edge` instances act like directed edges unless specified.
 */
TEST_F(EdgeTest, ConnectsTest)
{
  ASSERT_TRUE(edge_->connects(vert_start_, vert_end_));
  ASSERT_FALSE(edge_->connects(vert_end_, vert_start_));
  ASSERT_TRUE(edge_->connects(vert_end_, vert_start_, true));
}

/**
 * Test that the `==`, `!=` operator overload for `edge` works as expected.
 */
TEST_F(EdgeTest, EqNeTest)
{
  double other_edge_weight_ = 7;
  edge_ptr other_edge_ = std::make_shared<edge>(
    vert_start_, vert_end_, other_edge_weight_
  );
  ASSERT_EQ(*edge_, *edge_);
  ASSERT_NE(*edge_, *other_edge_);
  other_edge_->set_weight(edge_weight_);
  ASSERT_EQ(*edge_, *other_edge_);
}

}  // namespace

}  // namespace pdcip::tests
}  // namespace pdcip
