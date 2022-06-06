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
 * Test fixture base class for all linked list node tests.
 */
class LinkTest : public ::testing::Test {
protected:
  virtual ~LinkTest() = default;
  static double head_value_;
  static const double_vector next_values_;
  static const double next_value_first_;
  static const double next_value_last_;
};

double LinkTest::head_value_ = 5;
const double_vector LinkTest::next_values_ = {7.1, 4.5, 6.7, 1, 9.8};
const double LinkTest::next_value_first_ = LinkTest::next_values_.front();
const double LinkTest::next_value_last_ = LinkTest::next_values_.back();

/**
 * Test fixture for `single_link` tests.
 */
class SingleLinkTest : public LinkTest {
protected:
  SingleLinkTest() : head_(std::make_shared<single_link>(head_value_)) {}
  const single_link_ptr head_;
};

/**
 * Test that the `count_links` template works as expected with `single_link`.
 *
 * @note Indirectly tests `single_link::insert_next`, which is already
 *    indirectly tested by the `InsertLinksTest` for the `SingleLinkTest`.
 */
TEST_F(SingleLinkTest, CountLinksTest)
{
  single_link::insert_next(head_, next_values_);
  ASSERT_EQ(next_values_.size() + 1, count_links<single_link>(head_));
}

/**
 * Test that `insert_link` template works as expected with `single_link`.
 *
 * Tests both the case where next node is null and when next node is not null.
 */
TEST_F(SingleLinkTest, InsertLinkTest)
{
  auto next_ = insert_link<single_link>(head_, next_value_first_);
  ASSERT_EQ(head_->next(), next_);
  ASSERT_DOUBLE_EQ(next_value_first_, head_->next()->value());
  next_ = insert_link<single_link>(head_, next_value_first_ + 1);
  ASSERT_EQ(head_->next(), next_);
  ASSERT_DOUBLE_EQ(next_value_first_ + 1, head_->next()->value());
}

/**
 * Test that `insert_links` template works as expected.
 */
TEST_F(SingleLinkTest, InsertLinksTest)
{
  auto link_pair = insert_links<single_link>(head_, next_values_);
  single_link_ptr cur = link_pair.first;
  ASSERT_EQ(head_->next(), cur);
  for (double value : next_values_) {
    ASSERT_DOUBLE_EQ(value, cur->value());
    if (!cur->next()) {
      ASSERT_EQ(link_pair.second, cur);
    }
    cur = cur->next();
  }
}

/**
 * Test that the `append_link` template works as expected with `single_link`.
 */
TEST_F(SingleLinkTest, AppendLinkTest)
{
  auto link_pair = single_link::insert_next(head_, next_values_);
  auto last = append_link<single_link>(head_, head_value_);
  ASSERT_DOUBLE_EQ(head_value_, last->value());
  ASSERT_DOUBLE_EQ(link_pair.second->next()->value(), last->value());
}

/**
 * Test fixture for `double_link` tests.
 */
class DoubleLinkTest : public LinkTest {
protected:
  DoubleLinkTest() : head_(std::make_shared<double_link>(head_value_)) {}
  const double_link_ptr head_;
};

/**
 * Test that the `count_links` template works as expected with `double_link`.
 *
 * @note Indirectly tests `single_link::insert_next`, which is already
 *    indirectly tested by the `InsertLinksTest` for the `SingleLinkTest`.
 */
TEST_F(DoubleLinkTest, CountLinksTest)
{
  double_link::insert_next(head_, next_values_);
  ASSERT_EQ(next_values_.size() + 1, count_links<double_link>(head_));
}

/**
 * Test that `insert_link` template works as expected with `double_link`.
 *
 * Tests both the case where next node is null and when next node is not null.
 *
 * @note The inserted node's `prev` attribute is *NOT* `head_` but rather a
 *    copy of that `std::shared_ptr` (pointing to the same node)!
 */
TEST_F(DoubleLinkTest, InsertLinkTest)
{
  auto next_ = insert_link<double_link>(head_, next_value_first_);
  ASSERT_EQ(head_->next(), next_);
  // note that ASSERT_EQ(head_, next_->prev()) is false
  ASSERT_DOUBLE_EQ(next_value_first_, head_->next()->value());
  ASSERT_DOUBLE_EQ(head_value_, next_->prev()->value());
  next_ = insert_link<double_link>(head_, next_value_first_ + 1);
  ASSERT_EQ(head_->next(), next_);
  // again, ASSERT_EQ(head_, next_->prev()) is false
  ASSERT_DOUBLE_EQ(next_value_first_ + 1, head_->next()->value());
  ASSERT_DOUBLE_EQ(head_value_, next_->prev()->value());
}

/**
 * Test that `insert_links` template works as expected.
 *
 * @note See comments in `InsertLinkTest` for `DoubleLinkTest` fixture on why
 *    `ASSERT_EQ(head_, next_->prev())` assertion will fail.
 */
TEST_F(DoubleLinkTest, InsertLinksTest)
{
  auto link_pair = insert_links<double_link>(head_, next_values_);
  ASSERT_EQ(head_->next(), link_pair.first);
  ASSERT_DOUBLE_EQ(next_value_first_, link_pair.first->value());
  ASSERT_DOUBLE_EQ(head_value_, link_pair.first->prev()->value());
  double_link_ptr cur = link_pair.first->next();
  for (unsigned int i = 1; i < next_values_.size(); i++) {
    ASSERT_DOUBLE_EQ(next_values_[i], cur->value());
    ASSERT_DOUBLE_EQ(next_values_[i - 1], cur->prev()->value());
    if (!cur->next()) {
      ASSERT_EQ(link_pair.second, cur);
    }
    cur = cur->next();
  }
}

/**
 * Test that the `append_link` template works as expected with `double_link`.
 */
TEST_F(DoubleLinkTest, AppendLinkTest)
{
  auto link_pair = double_link::insert_next(head_, next_values_);
  auto last = append_link<double_link>(head_, head_value_);
  ASSERT_DOUBLE_EQ(head_value_, last->value());
  ASSERT_DOUBLE_EQ(link_pair.second->next()->value(), last->value());
  ASSERT_DOUBLE_EQ(link_pair.second->value(), last->prev()->value());
}

}  // namespace

}  // namespace pdcip::testing
}  // namespace pdcip
