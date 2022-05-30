/**
 * @file enums.h
 * @author Derek Huang
 * @brief C++ header for enum class types
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_ENUMS_H_
#define PDCIP_CPP_ENUMS_H_

namespace pdcip {

/**
 * Enum type indicating if algorithm runs recursively or iteratively.
 */
enum class algo_loop_type {recursive, iterative};

/**
 * Enum type dictating how search matches should be conducted.
 *
 * `exact` for exact search match, `from_above` to match nearest upper bound if
 * no exact match, `from_below` to match nearest lower bound if no exact match.
 */
enum class search_strategy {exact, from_above, from_below};

}  // namespace pdcip

#endif  // PDCIP_CPP_ENUMS_H_
