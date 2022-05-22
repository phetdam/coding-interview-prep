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

namespace pdcip {

template <class tree_t>
using tree_ptr_t = std::shared_ptr<tree_t>;
template <class tree_t>
using tree_children_t = std::vector<tree_ptr_t<tree_t>>;
template <class tree_t>
using tree_children_ptr_t = std::shared_ptr<tree_children_t<tree_t>>;

class tree;
using tree_ptr = tree_ptr_t<tree>;
using tree_children = tree_children_t<tree>;
using tree_children_ptr = tree_children_ptr_t<tree>;

class binary_tree;
using binary_tree_ptr = tree_ptr_t<binary_tree>;

/**
 * A general multi-child tree for numeric data.
 */
class tree {
public:
  tree(double = NAN, const tree_children_ptr& = nullptr);
  tree(double, tree_children_ptr&&);
  double value() const;
  const tree_children_ptr& children() const;
  std::size_t n_children() const;
  void set_value(double);
  virtual void set_children(const tree_children_ptr&);
  virtual void set_children(tree_children_ptr&&);
  static tree_children_ptr make_children(const std::vector<double>&);

private:
  double value_;
  tree_children_ptr children_;
};

/**
 * A binary tree for numeric data.
 */
class binary_tree: public tree {
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
  void insert(double);
  std::vector<double> sorted_values();
};

/**
 * Convenience templated function to generate tree children for any tree type.
 *
 * @param values `std::vector<double>` of values to supply the children
 */
template <class tree_t = tree>
tree_children_ptr_t<tree_t> make_tree_children(
  const std::vector<double>& values)
{
  tree_children_ptr_t<tree_t>
  children = std::make_shared<tree_children_t<tree_t>>(
    tree_children_t<tree_t>(values.size())
  );
  for (std::size_t i = 0; i < values.size(); i++) {
    (*children)[i] = std::make_shared<tree_t>(tree_t(values[i]));
  }
  return children;
}

}  // namespace pdcip

#endif  // PDCIP_CPP_TREE_H_
