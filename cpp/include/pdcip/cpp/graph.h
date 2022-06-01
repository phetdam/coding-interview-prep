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
 *
 * @note `vertex` instances that have the same value are distinct.
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
 *
 * @note `edge` instances that have the same vertex pointers and the same
 *     weight are considered equal under `==`.
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
  bool connects(const vertex_ptr&, const vertex_ptr&, bool = true);

private:
  vertex_ptr start_;
  vertex_ptr end_;
  double weight_;
};

bool operator==(const edge&, const edge&);

/**
 * General graph implementation.
 *
 * @note Does not support duplicated edges in the graph, i.e. edges that have
 *     identical start, end vertices and edge weight.
 *
 * Uses `std::unordered_map` to allow constant time checking of edge
 * conectivity to emulate adjacency matrix lookup performance while minimizing
 * memory use. Edge + vertex membership checking is also constant due to this.
 */
class graph {
public:
  graph(const vertex_ptr_vector&, const edge_ptr_vector&);
  graph(const vertex_ptr_vector_ptr&, const edge_ptr_vector_ptr&);
  graph(vertex_ptr_vector_ptr&&, edge_ptr_vector_ptr&&);
  vertex_ptr_vector_ptr vertices() const;
  edge_ptr_vector_ptr edges() const;
  std::size_t n_vertices() const;
  std::size_t n_edges() const;
  void add_vertex(const vertex_ptr&);
  void add_vertex(vertex_ptr&&);
  void add_vertices(const vertex_ptr_vector&);
  void add_vertices(const vertex_ptr_vector_ptr&);
  void add_edge(const edge_ptr&);
  void add_edge(edge_ptr&&);
  void add_edges(const edge_ptr_vector&);
  void add_edges(const edge_ptr_vector_ptr&);
  bool has_vertex(const vertex_ptr&);
  bool has_edge(const edge_ptr&);
  bool has_edge(edge_ptr&&);
  bool has_edge(const edge&);
  bool has_edge(edge&&);
  bool connects(const vertex_ptr&, const vertex_ptr&, bool = true);
};

}  // namespace pdcip

#endif  // PDCIP_CPP_GRAPH_H_
