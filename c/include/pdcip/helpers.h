/**
 * @file helpers.h
 * @author Derek Huang
 * @brief Useful helpers that make coding in C less repetitive.
 */

#ifndef PDCIP_HELPERS_H_
#define PDCIP_HELPERS_H_

#include <assert.h>
#include <stddef.h>

/**
 * Checks that index range specified by two endpoints is valid.
 *
 * @param start Leftmost index
 * @param end Rightmost index
 */
#define check_index_endpoints(start, end) \
  assert(start >= 0); \
  assert(end >= start)

/**
 * Binds [partial] sum of array elements to a name in current scope.
 *
 * @param name Name of variable to bind sum to
 * @param type Type of the final variable, ex. `size_t`, `double`
 * @param ar `type *` array we are looping through
 * @param start Leftmost index to start at
 * @param end Rightmost index to end at (not included)
 */
#define array_sum_range(name, type, ar, start, end) \
  check_index_endpoints(start, end); \
  type name = 0; \
  for (size_t i = start; i < end; i++) { name += ar[i]; }

/**
 * Binds [partial] sum of array elements to name in current scope with offset.
 *
 * @param name Name of variable to bind sum to
 * @param type Type of the final variable, ex. `size_t`, `double`
 * @param ar `type *` array we are looping through
 * @param start Leftmost index to start at
 * @param end Rightmost index to end at (not included)
 * @param offset `type` that will be added to `name`
 */
#define array_sum_range_offset(name, type, ar, start, end, offset) \
  array_sum_range(name, type, ar, start, end); \
  name += offset

/**
 * Binds sum of `n_items` in an array to a name in current scope.
 *
 * Starts from the leftmost item, i.e. `ar[0]`, of the array `ar`.
 *
 * @param name Name of variable to bind sum to
 * @param type Type of the final variable, ex. `size_t`, `double`
 * @param ar `type *` array we are looping through
 * @param n_items Number of elements in `ar` to process, from `*ar`
 */
#define array_sum(name, type, ar, n_items) \
  array_sum_range(name, type, ar, 0, n_items)

/**
 * Binds sum of `n_items` in an array to name in current scope with offset.
 *
 * @param name Name of variable to bind sum to
 * @param type Type of the final variable, ex. `size_t`, `double`
 * @param ar `type *` array we are looping through
 * @param n_items Number of elements in `ar` to process, from `*ar`
 * @param offset `type` that will be added to `name`
 */
#define array_sum_offset(name, type, ar, n_items, offset) \
  array_sum(name, type, ar, n_items); \
  name += offset

/**
 * Apply function to a range of array elements, discarding return values.
 *
 * Elements of the array `ar` are not overwritten.
 *
 * @param func Single-argument function/macro
 * @param ar Name of array we want to apply `func` to
 * @param start Leftmost index to start applying at
 * @param end Rightmost index to stop applying at (not included)
 */
#define map_call_func_range(func, ar, start, end) \
  check_index_endpoints(start, end); \
  for (size_t i = start, i < end; i++) { func(ar[i]); }

/**
 * Apply `void` function to an array's elements, discarding return values.
 *
 * Elements of the array `ar` are not overwritten.
 *
 * @param func Single-argument function/macro
 * @param ar Name of array we want to apply `func` to
 * @param n_items Number of items in the array
 */
#define map_call_func(func, ar, n_items) \
  map_call_func_range(func, ar, 0, n_items)

/**
 * Apply function to a range of array elements, writing to another array.
 *
 * @param func Single-argument function/macro
 * @param src Name of array we want to apply `func` to
 * @param dest Name of array overwritten with return values from `func` apply
 * @param start Leftmost index to start applying at
 * @param end Rightmost index to stop applying at (not included)
 */
#define map_func_range(func, src, dest, start, end) \
  check_index_endpoints(start, end); \
  for (size_t i = start; i < end; i++) { dest[i] = func(src[i]); }

/**
 * Apply function to array elements, writing to another array.
 *
 * @param func Single-argument function/macro
 * @param src Name of array we want to apply `func` to
 * @param dest Name of array overwritten with return values from `func` apply
 * @param n_items Number of items in the array `src`
 */
#define map_func(func, src, dest, n_items) \
  map_func_range(func, src, dest, 0, n_items)

/**
 * Apply function to a range of array elements, overwriting with return values.
 *
 * @param func Single-argument function/macro
 * @param ar Name of array we want to apply `func` to and overwrite
 * @param start Leftmost index to start applying at
 * @param end Rightmost index to stop applying at (not included)
 */
#define map_inplace_func_range(func, ar, start, end) \
  map_func_range(func, ar, ar, start, end)

/**
 * Apply function to array elements, overwriting with return values.
 *
 * @param func Single-argument function/macro
 * @param ar Name of array we want to apply `func` to and overwrite
 * @param n_items Number of items in the array
 */
#define map_inplace_func(func, ar, n_items) \
  map_inplace_func_range(func, ar, 0, n_items)

#endif  /* PDCIP_HELPERS_H_ */
