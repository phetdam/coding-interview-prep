/**
 * @file tree.h
 * @author Derek Huang
 * @brief C++ header for general tree implementations
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_STRUCTURES_TREE_H_
#define PDCIP_CPP_STRUCTURES_TREE_H_

#include <cstdint>
#include <limits>
#include <memory>
#include <vector>

namespace pdcip {
namespace structures {

namespace {

const double nan = std::numeric_limits<double>::quiet_NaN();

}  // namespace

class tree;
using tree_ptr = std::shared_ptr<tree>;
using tree_children = std::vector<tree_ptr>;
using tree_children_ptr = std::shared_ptr<tree_children>;

class binary_tree;
using binary_tree_ptr = std::shared_ptr<binary_tree>;

/**
 * A general multi-child tree for numeric data.
 */
class tree {
public:
  tree(double = nan, const tree_children_ptr& = nullptr);
  tree(double, tree_children_ptr&&);
  double value() const;
  const tree_children_ptr& children() const;
  std::size_t n_children() const;
  void set_value(double);
  virtual void set_children(const tree_children_ptr&);
  virtual void set_children(tree_children_ptr&&);

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
    double = nan,
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
};

}  // namespace pdcip::structures
}  // namespace pdcip

#endif  // PDCIP_CPP_STRUCTURES_TREE_H_
