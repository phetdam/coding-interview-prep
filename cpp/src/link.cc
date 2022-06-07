/**
 * @file link.cc
 * @author Derek Huang
 * @brief C++ source for linked list implementations
 * @copyright MIT License
 */

#include "pdcip/cpp/link.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <memory>
#include <utility>

#include "pdcip/cpp/types.h"

namespace pdcip {

/**
 * `single_link` constructor through copy.
 *
 * @param value `double` value to assign to node
 * @param next `const single_link_ptr&` giving pointer to next node
 */
single_link::single_link(double value, const single_link_ptr& next)
  : T_value_t<double>(value), T_next_t<single_link_ptr>(next)
{}

/**
 * `single_link` constructor through move.
 *
 * @param value `double` value to assign to node
 * @param next `single_link_ptr&&` giving pointer to next node
 */
single_link::single_link(double value, single_link_ptr&& next)
  : T_value_t<double>(value), T_next_t<single_link_ptr>(std::move(next))
{}

/**
 * Return number of links next in the chain of nodes after `this`.
 */
std::size_t single_link::n_next() const
{
  return count_links<single_link>(this->next());
}

/**
 * Return number of links accessible in the chain of nodes, including `this`.
 */
std::size_t single_link::n_links() const { return n_next() + 1; }

/**
 * Insert a link between `head` and its next link.
 *
 * @param head `const single_link_ptr&` linked list head
 * @param value `double` value of node to insert between `head`, `head->next()`
 * @returns `single_link_ptr` pointing to the inserted node
 */
single_link_ptr single_link::insert_next(
  const single_link_ptr& head, double value)
{
  assert(!std::isnan(value));
  single_link_ptr new_link = std::make_shared<single_link>(value);
  if (head->next()) {
    new_link->set_next(head->next());
  }
  head->set_next(new_link);
  return new_link;
}

/**
 * Insert multiple links between `head` and its next link.
 *
 * @param head `const single_link_ptr&` linked list head
 * @param values `const double_vector&` with values of the nodes to insert
 *    between node `head` and node `head->next()`
 * @returns `single_link_ptr_pair` giving the first and last nodes inserted
 */
single_link_ptr_pair single_link::insert_next(
  const single_link_ptr& head, const double_vector& values)
{
  return insert_links<single_link>(head, values);
}

/**
 * `double_link` constructor through copy.
 *
 * @param value `double` value to assign to node
 * @param prev `const double_link_ptr&` giving pointer to prev node
 * @param next `const double_link_ptr&` giving pointer to next node
 */
double_link::double_link(
  double value, const double_link_ptr& prev, const double_link_ptr& next)
  : T_value_t<double>(value),
    T_prev_t<double_link_ptr>(prev),
    T_next_t<double_link_ptr>(next)
{}

/**
 * `double_link` constructor through move.
 *
 * @param value `double` value to assign to node
 * @param prev `double_link_ptr&&` giving pointer to prev node
 * @param next `double_link_ptr&&` giving pointer to next node
 */
double_link::double_link(
  double value, double_link_ptr&& prev, double_link_ptr&& next)
  : T_value_t<double>(value),
    T_prev_t<double_link_ptr>(std::move(prev)),
    T_next_t<double_link_ptr>(std::move(next))
{}

/**
 * Return the number of links previous in the chain of nodes.
 */
std::size_t double_link::n_prev() const
{
  double_link_ptr cur = this->prev();
  std::size_t n_prev = 0;
  while (cur) {
    cur = cur->prev();
    n_prev++;
  }
  return n_prev;
}

/**
 * Return number of links next in the chain of nodes after `this`.
 */
std::size_t double_link::n_next() const
{
  return count_links<double_link>(this->next());
}

/**
 * Return number of links accessible in the chain of nodes, including `this`.
 */
std::size_t double_link::n_links() const { return n_prev() + n_next() + 1; }

/**
 * Insert a link between `head` and its previous link.
 *
 * @param head `const double_link_ptr&` linked list head
 * @param value `double` value of node to insert between `head->prev()`, `head`
 * @returns `double_link_ptr` pointing to the inserted node
 */
double_link_ptr double_link::insert_prev(
  const double_link_ptr& head, double value)
{
  assert(!std::isnan(value));
  double_link_ptr new_link = std::make_shared<double_link>(value);
  if (head->prev()) {
    new_link->set_prev(head->prev());
    head->prev()->set_next(new_link);
  }
  head->set_prev(new_link);
  new_link->set_next(head);
  return new_link;
}

/**
 * Insert multiple links between `head` and its previous link.
 *
 * @param head `const double_link_ptr&` linked list head
 * @param values `const double_vector&` with values of the nodes to insert
 *    between node `head->prev()` and node `head`
 * @returns `double_link_ptr_pair` giving the first and last nodes inserted
 */
double_link_ptr_pair double_link::insert_prev(
  const double_link_ptr& head, const double_vector& values)
{
  if (!values.size()) {
    return double_link_ptr_pair();
  }
  double_link_ptr last = head;
  double_link_ptr first;
  for (double value : values) {
    last = insert_prev(head, value);
    if (!first) {
      first = last;
    }
  }
  return std::make_pair(first, last);
}


/**
 * Insert a link between `head` and its next link.
 *
 * @param head `const double_link_ptr&` linked list head
 * @param value `double` value of node to insert between `head`, `head->next()`
 * @returns `double_link_ptr` pointing to the inserted node
 */
double_link_ptr double_link::insert_next(
  const double_link_ptr& head, double value)
{
  assert(!std::isnan(value));
  double_link_ptr new_link = std::make_shared<double_link>(value);
  if (head->next()) {
    new_link->set_next(head->next());
    head->next()->set_prev(new_link);
  }
  head->set_next(new_link);
  new_link->set_prev(head);
  return new_link;
}

/**
 * Insert multiple links between `head` and its next link.
 *
 * @param head `const double_link_ptr&` linked list head
 * @param values `const double_vector&` with values of the nodes to insert
 *    between node `head` and node `head->next()`
 * @returns `double_link_ptr_pair` giving the first and last nodes inserted
 */
double_link_ptr_pair double_link::insert_next(
  const double_link_ptr& head, const double_vector& values)
{
  return insert_links<double_link>(head, values);
}

}  // namespace pdcip
