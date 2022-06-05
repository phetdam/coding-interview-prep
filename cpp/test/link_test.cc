/**
 * @file link_test.cc
 * @author Derek Huang
 * @brief Unit tests for the linked list implementations in link.cc
 * @copyright MIT License
 */

#include "pdcip/cpp/link.h"

#include <memory>

#include <gtest/gtest.h>

#include "pdcip/cpp/types.h"
#include "pdcip/cpp/testing/msvc.h"

#ifdef _MSC_VER
#pragma warning (disable: GTEST_FIXTURE_WARNINGS)
#endif  // _MSC_VER

namespace pdcip {
namespace testing {

namespace {

/**
 * Test fixture class for all linked list node tests.
 *
 * @note Later reorganize to have separate static functions adding links next
 *    and previous to a specified head node. Maybe add to library?
 */
class LinkTest : public ::testing::Test {
protected:
  LinkTest()
    : single_head_(std::make_shared<single_link>(head_value_)),
      double_head_(std::make_shared<double_link>(head_value_))
  {}

  /**
   * Setup function checking the sanity of the head nodes.
   */
  void SetUp() override
  {
    ASSERT_DOUBLE_EQ(head_value_, single_head_->value());
    ASSERT_EQ(nullptr, single_head_->next());
    ASSERT_DOUBLE_EQ(head_value_, double_head_->value());
    ASSERT_EQ(nullptr, double_head_->prev());
    ASSERT_EQ(nullptr, double_head_->next());
  }

  /**
   * Add next links to `single_head_`.
   */
  void add_next_links()
  {
    single_link_ptr cur = single_head_;
    for (double value : next_link_values_) {
      cur->set_next(std::make_shared<single_link>(value));
      cur = cur->next();
      ASSERT_DOUBLE_EQ(value, cur->value());
      ASSERT_EQ(nullptr, cur->next());
    }
  }

  single_link_ptr single_head_;
  double_link_ptr double_head_;
  static double head_value_;
  static const double next_link_value_;
  static const double_vector next_link_values_;
};

double LinkTest::head_value_ = 5;
const double LinkTest::next_link_value_ = 7.1;
const double_vector LinkTest::next_link_values_ = {
  LinkTest::next_link_value_, 4.5, 6.7, 1, 9.8
};

// this way, we can split the fixture tests into different suites
using SingleLinkTest = LinkTest;
using DoubleLinkTest = LinkTest;

/**
 * Test that the `single_link::n_next` method works as intended.
 */
TEST_F(SingleLinkTest, NNextTest)
{
  add_next_links();
  ASSERT_EQ(next_link_values_.size(), single_head_->n_next());
}

}  // namespace

}  // namespace pdcip::testing
}  // namespace pdcip
