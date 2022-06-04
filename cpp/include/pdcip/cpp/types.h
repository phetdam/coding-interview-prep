/**
 * @file types.h
 * @author Derek Huang
 * @brief Extra convenience types
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_TYPES_H_
#define PDCIP_CPP_TYPES_H_

#include <memory>
#include <utility>
#include <vector>

namespace pdcip {

template <class T>
using vector_ptr_t = std::shared_ptr<std::vector<T>>;

using double_vector = std::vector<double>;
using double_vector_ptr = vector_ptr_t<double>;
using double_pair = std::pair<double, double>;

template <class T>
using T_ptr_t = std::shared_ptr<T>;
template <class T>
using T_ptr_vector_t = std::vector<T_ptr_t<T>>;
template <class T>
using T_ptr_vector_ptr_t = std::shared_ptr<T_ptr_vector_t<T>>;

class tree;
using tree_ptr = T_ptr_t<tree>;
using tree_ptr_vector = T_ptr_vector_t<tree>;
using tree_ptr_vector_ptr = T_ptr_vector_ptr_t<tree>;

class binary_tree;
using binary_tree_ptr = T_ptr_t<binary_tree>;
using binary_tree_ptr_vector = T_ptr_vector_t<binary_tree>;
using binary_tree_ptr_vector_ptr = T_ptr_vector_ptr_t<binary_tree>;

class vertex;
using vertex_ptr = T_ptr_t<vertex>;
using vertex_ptr_vector = T_ptr_vector_t<vertex>;
using vertex_ptr_vector_ptr = T_ptr_vector_ptr_t<vertex>;

class edge;
using edge_ptr = T_ptr_t<edge>;
using edge_ptr_vector = T_ptr_vector_t<edge>;
using edge_ptr_vector_ptr = T_ptr_vector_ptr_t<edge>;

class single_link;
using single_link_ptr = T_ptr_t<single_link>;

class double_link;
using double_link_ptr = T_ptr_t<double_link>;

/**
 * Base class template for any object that holds a mutable `T` value.
 *
 * The `T` `value` can be either copy set or moved.
 */
template <class T>
class T_value_t {
public:
  T_value_t(const T& value) : value_(value) {}
  T_value_t(T&& value) : value_(std::move(value)) {}
  virtual ~T_value_t() = default;
  const T& value() const { return value_; };
  void set_value(const T& value) { value_ = value; }
  void set_value(T&& value) { value_ = std::move(value); }
private:
  T value_;
};

/**
 * Base class template for any object that holds a mutable `T` next value.
 *
 * The `T` `next` can be either copy set or moved.
 */
template <class T>
class T_next_t {
public:
  T_next_t(const T& next) : next_(next) {}
  T_next_t(T&& next) : next_(std::move(next)) {}
  virtual ~T_next_t() = default;
  const T& value() const { return next_; }
  void set_next(const T& next) { next_ = next; }
  void set_next(T&& next) { next_ = std::move(next); }
private:
  T next_;
};

}  // namespace pdcip

#endif  // PDCIP_CPP_TYPES_H_
