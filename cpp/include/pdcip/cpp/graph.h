/**
 * @file graph.h
 * @author Derek Huang
 * @brief C++ header for general graph implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_GRAPH_H_
#define PDCIP_CPP_GRAPH_H_

#include <cmath>
#include <cstddef>
#include <unordered_map>
#include <utility>

#include "pdcip/cpp/types.h"

namespace pdcip {

using graph_vertex_map = std::unordered_map<vertex_ptr, std::nullptr_t>;
using graph_edge_map = std::unordered_map<
  std::pair<vertex_ptr, vertex_ptr>,
  std::shared_ptr<std::unordered_map<double, std::nullptr_t>>
>;

/**
 * Graph vertex class for holding numeric data.
 *
 * @note `vertex` instances that have the same value are distinct.
 */
class vertex : public T_value_t<double> {
public:
  vertex(double = NAN);
};

/**
 * Graph directed edge with optional weight.
 *
 * @note The `edge` implementation allows loops, i.e. for the start and end
 *    `vertex_ptr` instances to be the same `shared_ptr`.
 *
 * @note `edge` instances that have the same vertex pointers and the same
 *     weight are considered equal under `==`.
 */
class edge : public T_value_t<double> {
public:
  edge(const vertex_ptr&, const vertex_ptr&, double = 1);
  edge(vertex_ptr&&, vertex_ptr&&, double = 1);
  const vertex_ptr& start() const;
  const vertex_ptr& end() const;
  double weight() const;
  void set_start(const vertex_ptr&);
  void set_start(vertex_ptr&&);
  void set_end(const vertex_ptr&);
  void set_end(vertex_ptr&&);
  void set_weight(double);
  bool connects(const vertex_ptr&, const vertex_ptr&, bool = false);
private:
  vertex_ptr start_;
  vertex_ptr end_;
};

bool operator==(const edge&, const edge&);
bool operator!=(const edge&, const edge&);

/**
 * General graph implementation.
 *
 * @note Does not support duplicated edges in the graph, i.e. edges that have
 *     identical start, end vertices and edge weight.
 *
 * Uses `std::unordered_map` to allow constant time checking of edge
 * connectivity to emulate adjacency matrix lookup performance while minimizing
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
private:
  graph_vertex_map vertices_;
  graph_edge_map edges_;
};

}  // namespace pdcip

#endif  // PDCIP_CPP_GRAPH_H_
