/**
 * @file graph.h
 * @author Derek Huang
 * @brief C++ header for general graph implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_GRAPH_H_
#define PDCIP_CPP_GRAPH_H_

#include <cmath>
#include <utility>

#include "pdcip/cpp/types.h"

namespace pdcip {

/**
 * Graph vertex class for holding numeric data.
 */
class vertex {
public:
  vertex(double = NAN);
  double value() const;
  void set_value(double);

private:
  double value_;
};

/**
 * Graph directed edge with optional weight.
 */
class edge {
public:
  edge(const vertex_ptr&, const vertex_ptr&, double weight = 1);
  const vertex_ptr& start() const;
  const vertex_ptr& end() const;
  double weight() const;
  void set_start(const vertex_ptr&);
  void set_start(vertex_ptr&&);
  void set_end(const vertex_ptr&);
  void set_end(vertex_ptr&&);
  void set_weight(double);

private:
  vertex_ptr start_;
  vertex_ptr end_;
  double weight_;
};

bool operator==(const edge&, const edge&);

/**
 * Abstract general graph base class.
 *
 * Guarantees that heap-allocated vectors of `vertex` and `edge` shared
 * pointers representing vertices and edges can be accessed.
 */
class graph {
public:
  virtual ~graph() = default;
  virtual vertex_ptr_vector_ptr vertices() const = 0;
  virtual edge_ptr_vector_ptr edges() const = 0;
};

/**
 * Graph implemented using an adjacency list.
 */
// class adj_list_graph : public graph {

// };

/**
 * Graph implemented using an adjacency matrix.
 */
// class adj_matrix_graph : public graph {

// };

}  // namespace pdcip

#endif  // PDCIP_CPP_GRAPH_H_
