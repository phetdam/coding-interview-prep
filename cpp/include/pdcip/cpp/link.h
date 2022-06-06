/**
 * @file link.h
 * @author Derek Huang
 * @brief C++ header for linked list implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_LINK_H_
#define PDCIP_CPP_LINK_H_

#include <cassert>
#include <cmath>
#include <cstddef>
#include <utility>

#include "pdcip/cpp/types.h"

namespace pdcip {

/**
 * A simple single-linked list node implementation.
 */
class single_link
  : public T_value_t<double>, public T_next_t<single_link_ptr> {
public:
  single_link(double = NAN, const single_link_ptr& = nullptr);
  single_link(double, single_link_ptr&&);
  std::size_t n_next() const;
  std::size_t n_links() const;
  static single_link_ptr insert_next(const single_link_ptr&, double);
  static single_link_ptr_pair insert_next(
    const single_link_ptr&, const double_vector&
  );
};

/**
 * A simple double-linked list node implementation.
 */
class double_link
  : public T_value_t<double>,
    public T_prev_t<double_link_ptr>,
    public T_next_t<double_link_ptr> {
public:
  double_link(
    double = NAN,
    const double_link_ptr& = nullptr,
    const double_link_ptr& = nullptr
  );
  double_link(double, double_link_ptr&&, double_link_ptr&&);
  std::size_t n_prev() const;
  std::size_t n_next() const;
  std::size_t n_links() const;
  static double_link_ptr insert_prev(const double_link_ptr&, double);
  static double_link_ptr_pair insert_prev(
    const double_link_ptr&, const double_vector&
  );
  static double_link_ptr insert_next(const double_link_ptr&, double);
  static double_link_ptr_pair insert_next(
    const double_link_ptr&, const double_vector&
  );
};

/**
 * Template function returning number of nodes in a chain of nodes.
 *
 * Uses `next` to discover more nodes until `nullptr` is reached. The returned
 * count includes the current `head` node passed as a parameter.
 *
 * @note On circular linked lists, this function will loop forever!
 *
 * @tparam link_t linked list node type
 * @param head `const T_ptr_t<link_t>&` linked list head
 */
template <class link_t>
std::size_t count_links(const T_ptr_t<link_t>& head)
{
  T_ptr_t<link_t> cur = head;
  std::size_t n_links = 0;
  while (cur) {
    cur = cur->next();
    n_links++;
  }
  return n_links;
}

/**
 * Template function returning number of nodes in a chain of nodes.
 *
 * Version of the above that uses a raw pointer instead.
 *
 * @see count_links(const link_ptr_t)
 *
 * @tparam link_t linked list node type
 * @param head `const link_t*` linked list head
 */
template <class link_t>
std::size_t count_links(const link_t* head)
{
  const link_t* cur = head;
  std::size_t n_links = 0;
  while (cur) {
    cur = cur->next().get();
    n_links++;
  }
  return n_links;
}

/**
 * Inserts a "next" node between current node and its next node.
 *
 * @note Requires that the `link_t` type implements `insert_next`.
 *
 * @tparam link_t linked list node type
 * @param head `const T_ptr_t<link_t>&` linked list head
 * @param value `double` value of node to insert between `head`, `head->next()`
 * @returns `T_ptr_t<link_t>` pointing to the inserted node
 */
template <class link_t>
T_ptr_t<link_t> insert_link(const T_ptr_t<link_t>& head, double value)
{
  return link_t::insert_next(head, value);
}

/**
 * Insert multiple "next" nodes between current nodes and its next node.
 *
 * @note Requires that the `link_t` type implements `insert_next`.
 *
 * @tparam link_t linked list node type
 * @param head `const T_ptr_t<link_t>&` linked list head
 * @param values `const double_vector&` values of nodes to insert between
 *    `head` node and its `head->next()` next node
 * @returns `T_ptr_pair_t<link_t>` giving pointer to the first of the inserted
 *    nodes and the last of inserted nodes as a pair
 */
template <class link_t>
T_ptr_pair_t<link_t> insert_links(
  const T_ptr_t<link_t>& head, const double_vector& values)
{
  T_ptr_t<link_t> last = head;
  T_ptr_t<link_t> first;
  for (double value : values) {
    last = link_t::insert_next(last, value);
    if (!first) {
      first = last;
    }
  }
  return std::make_pair(first, last);
}

/**
 * Append a "next" node after the last node in a chain with head `head`.
 *
 * @note Requires that the `link_t` type implements `insert_next`.
 *
 * @tparam link_t linked list node type
 * @param head `const T_ptr_t<link_t>&` linked list head
 * @param value `double` value of node to insert after last node in chain
 * @returns `T_ptr_t<link_t>` giving the appended node
 */
template <class link_t>
T_ptr_t<link_t> append_link(const T_ptr_t<link_t>& head, double value)
{
  T_ptr_t<link_t> cur = head;
  while (cur->next()) {
    cur = cur->next();
  }
  return insert_link<link_t>(cur, value);
}

/**
 * Appends multiple "next" nodes after last node in a chain with head `head`.
 *
 * @note Requires that the `link_t` type implements `insert_next`.
 *
 * @tparam link_t linked list node type
 * @param head `const T_ptr_t<link_t>&` linked list head
 * @param values `const double_vector&` values of nodes to insert at end
 * @returns `T_ptr_pair_t<link_t>` giving pointer to the first of the appended
 *    nodes and the last of appended nodes as a pair
 */
template <class link_t>
T_ptr_pair_t<link_t> append_links(
  const T_ptr_t<link_t>& head, const double_vector& values)
{
  T_ptr_t<link_t> last = head;
  T_ptr_t<link_t> first;
  for (double value : values) {
    last = append_link(last, value);
    if (!first) {
      first = last;
    }
  }
  return std::make_pair(first, last);
}

}  // namespace pdcip

#endif  // PDCIP_CPP_LINK_H_
