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
vertex::vertex(double value) : value_(value) {}

/**
 * Returns the value assigned to the `vertex`.
 */
double vertex::value() const { return value_; }

/**
 * Assigns a new value to the `vertex`.
 *
 * @param value `double` new value to assign to the `vertex`
 */
void vertex::set_value(double value) { value_ = value; }

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
  : weight_(weight)
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
  : weight_(weight)
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
 */
double edge::weight() const { return weight_; }

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
  weight_ = weight;
}

/**
 * Determine if the `edge` connects two specified vertices.
 *
 * @param start `const vertex_ptr&` starting vertex
 * @param end `const vertex_ptr&` ending vertex
 * @param directed `bool` where if `true`, the `edge` acts like a directed
 *    edge (the default), while if `false`, the `edge` acts undirected.
 */
bool edge::connects(
  const vertex_ptr& start, const vertex_ptr& end, bool directed)
{
  assert(start && end);
  if (this->start() == start && this->end() == end) {
    return true;
  }
  if (!directed && this->end() == start && this->start() == end) {
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

}  // namespace pdcip
