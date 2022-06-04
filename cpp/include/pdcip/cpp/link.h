/**
 * @file link.h
 * @author Derek Huang
 * @brief C++ header for linked list implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_LINK_H_
#define PDCIP_CPP_LINK_H_

#include <cmath>
#include <cstddef>

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

}  // namespace pdcip

#endif  // PDCIP_CPP_LINK_H_
