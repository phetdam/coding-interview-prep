/**
 * @file graph.cc
 * @author Derek Huang
 * @brief C++ source for a general graph implementation
 * @copyright MIT License
 */

#include "pdcip/cpp/graph.h"

#include <cassert>
#include <cmath>
#include <utility>

#include "pdcip/cpp/types.h"

namespace pdcip {

/**
 * `vertex` constructor.
 *
 * @param value `double` value to assign to the `vertex`
 */
vertex::vertex(double value) : T_mutable_t<double>(value) {}

/**
 * `edge` constructor.
 *
 * @note The `edge` vertices cannot point to null pointers.
 *
 * @param start `const vertex_ptr&` giving the starting vertex
 * @param end `const vertex_ptr&` giving the ending vertex
 * @param weight `double` optional edge weight
 */
edge::edge(const vertex_ptr& start, const vertex_ptr& end, double weight)
  : T_mutable_t<double>(weight)
{
  assert(start && end);
  start_ = start;
  end_ = end;
}

/**
 * `edge` constructor that moves from the `vertex` instances specified.
 *
 * @note The `edge` vertices cannot point to null pointers.
 *
 * @param start `vertex_ptr&&` giving the starting vertex
 * @param end `vertex_ptr&&` giving the ending vertex
 * @param weight `double` optional edge weight
 */
edge::edge(vertex_ptr&& start, vertex_ptr&& end, double weight)
  : T_mutable_t<double>(weight)
{
  assert(start && end);
  start_ = std::move(start);
  end_ = std::move(end);
}

/**
 * Returns a `const` ref to the start `vertex` of the `edge`.
 */
const vertex_ptr& edge::start() const { return start_; }

/**
 * Returns a `const` ref to the end `vertex` of the `edge`.
 */
const vertex_ptr& edge::end() const { return end_; }

/**
 * Returns the weight of the `edge`.
 *
 * @note Calls the standard `value` function of the `T_mutable_t`.
 */
double edge::weight() const { return value(); }

/**
 * Sets the start `vertex` by copy.
 *
 * @param vert `const vertex_ptr&` pointing to new start `vertex`
 */
void edge::set_start(const vertex_ptr& vert)
{
  assert(vert);
  start_ = vert;
}

/**
 * Sets the start `vertex` by move.
 *
 * @param vert `vertex_ptr&&` pointing to new start `vertex`
 */
void edge::set_start(vertex_ptr&& vert)
{
  assert(vert);
  start_ = std::move(vert);
}

/**
 * Sets the end `vertex` by copy.
 *
 * @param vert `const vertex_ptr&` pointing to new end `vertex`
 */
void edge::set_end(const vertex_ptr& vert)
{
  assert(vert);
  end_ = vert;
}

/**
 * Sets the end `vertex` by move.
 *
 * @param vert `vertex_ptr&&` pointing to new end `vertex`
 */
void edge::set_end(vertex_ptr&& vert)
{
  assert(vert);
  end_ = std::move(vert);
}

/**
 * Sets the `edge` weight.
 *
 * @note The new `edge` weight cannot be `NAN`.
 *
 * @param weight `double` giving the new edge weight
 */
void edge::set_weight(double weight)
{
  assert(!std::isnan(weight));
  set_value(weight);
}

/**
 * Determine if the `edge` connects two specified vertices.
 *
 * @param start `const vertex_ptr&` starting vertex
 * @param end `const vertex_ptr&` ending vertex
 * @param undirected `bool` where if `false`, the `edge` acts like a directed
 *    edge (the default), while if `true`, the `edge` acts as if undirected.
 */
bool edge::connects(
  const vertex_ptr& start, const vertex_ptr& end, bool undirected)
{
  assert(start && end);
  if (this->start() == start && this->end() == end) {
    return true;
  }
  if (undirected && this->end() == start && this->start() == end) {
    return true;
  }
  return false;
}

/**
 * Test two `edge` instances for equality.
 *
 * @param first `const edge&` the first edge
 * @param second `const edge&` the second edge
 */
bool operator==(const edge& first, const edge& second)
{
  return
    first.start() == second.start() &&
    first.end() == second.end() &&
    first.weight() == second.weight();
}

/**
 * Test two `edge` instances for inequality.
 *
 * @param first `const edge&` the first edge
 * @param second `const edge&` the second edge
 */
bool operator!=(const edge& first, const edge& second)
{
  return !(first == second);
}

/**
 * `graph` copy from object constructor.
 *
 * @param vertices `const vertex_ptr_vector&` with graph vertices
 * @param edges `const edge_ptr_vector&` with edge vertices
 */
/*
graph::graph(const vertex_ptr_vector& vertices, const edge_ptr_vector& edges)
{

}
*/

}  // namespace pdcip
