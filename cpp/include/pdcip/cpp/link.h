/**
 * @file link.h
 * @author Derek Huang
 * @brief C++ header for linked list implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_LINK_H_
#define PDCIP_CPP_LINK_H_

#include <cmath>

#include "pdcip/cpp/types.h"

namespace pdcip {

/**
 * A simple single-linked list node implementation.
 */
class single_link : public T_value_t<double>, public T_next_t<single_link_ptr> {
public:
  single_link(double = NAN, const single_link_ptr& = nullptr);
  single_link(double, single_link_ptr&&);
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
};

}  // namespace pdcip

#endif  // PDCIP_CPP_LINK_H_
