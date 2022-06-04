/**
 * @file link.cc
 * @author Derek Huang
 * @brief C++ source for linked list implementations
 * @copyright MIT License
 */

#include "pdcip/cpp/link.h"

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

}  // namespac pdcip
