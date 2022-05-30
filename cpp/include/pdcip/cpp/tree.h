/**
 * @file tree.h
 * @author Derek Huang
 * @brief C++ header for general tree implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_TREE_H_
#define PDCIP_CPP_TREE_H_

#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <vector>

#include "pdcip/cpp/types.h"

namespace pdcip {

template <class tree_t>
using tree_ptr_t = std::shared_ptr<tree_t>;
template <class tree_t>
using tree_ptr_vector_t = std::vector<tree_ptr_t<tree_t>>;
template <class tree_t>
using tree_ptr_vector_ptr_t = std::shared_ptr<tree_ptr_vector_t<tree_t>>;

class tree;
using tree_ptr = tree_ptr_t<tree>;
using tree_ptr_vector = tree_ptr_vector_t<tree>;
using tree_ptr_vector_ptr = tree_ptr_vector_ptr_t<tree>;

class binary_tree;
using binary_tree_ptr = tree_ptr_t<binary_tree>;
using binary_tree_ptr_vector = tree_ptr_vector_t<binary_tree>;
using binary_tree_ptr_vector_ptr = tree_ptr_vector_ptr_t<binary_tree>;

/**
 * A general multi-child tree for numeric data.
 */
class tree {
public:
  tree(
    double = NAN,
    const tree_ptr_vector_ptr& = std::make_shared<tree_ptr_vector>()
  );
  tree(double, tree_ptr_vector_ptr&&);
  double value() const;
  const tree_ptr_vector_ptr& children() const;
  std::size_t n_children() const;
  void set_value(double);
  virtual void set_children(const tree_ptr_vector_ptr&);
  virtual void set_children(tree_ptr_vector_ptr&&);
  static tree_ptr_vector_ptr make_children(const std::vector<double>&);
  static tree_ptr_vector_ptr dfs(const tree_ptr&);
  static tree_ptr_vector_ptr bfs(const tree_ptr&);

private:
  double value_;
  tree_ptr_vector_ptr children_;
};

/**
 * A binary tree for numeric data.
 */
class binary_tree : public tree {
public:
  binary_tree(
    double = NAN,
    const binary_tree_ptr& = nullptr,
    const binary_tree_ptr& = nullptr
  );
  binary_tree(double, binary_tree_ptr&&, binary_tree_ptr&&);
  binary_tree_ptr left() const;
  binary_tree_ptr right() const;
  void set_left(const binary_tree_ptr&);
  void set_left(binary_tree_ptr&&);
  void set_right(const binary_tree_ptr&);
  void set_right(binary_tree_ptr&&);
  const binary_tree* insert(double);
  double_vector_ptr sorted_values();
};

/**
 * Convenience templated function to generate tree children for any tree type.
 *
 * @param values `std::vector<double>` of values to supply the children
 */
template <class tree_t = tree>
tree_ptr_vector_ptr_t<tree_t> make_tree_ptr_vector(
  const std::vector<double>& values)
{
  tree_ptr_vector_ptr_t<tree_t>
  children = std::make_shared<tree_ptr_vector_t<tree_t>>(
    tree_ptr_vector_t<tree_t>(values.size())
  );
  for (std::size_t i = 0; i < values.size(); i++) {
    (*children)[i] = std::make_shared<tree_t>(tree_t(values[i]));
  }
  return children;
}

}  // namespace pdcip

#endif  // PDCIP_CPP_TREE_H_
