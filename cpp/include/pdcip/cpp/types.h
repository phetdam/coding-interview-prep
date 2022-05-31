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

}  // namespace pdcip

#endif  // PDCIP_CPP_TYPES_H_
