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


// class tree {
// public:
//   tree(double = nan);
//   virtual ~tree() = 0;
//   double value() const;
//   virtual const std::vector<std::shared_ptr<tree>>& children() const = 0;
//   virtual std::uintmax_t n_children() const = 0;

// protected:
//   double value_;
// };

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
class binary_tree : public tree {
public:
  binary_tree(
    double = nan,
    const std::shared_ptr<binary_tree>& = nullptr,
    const std::shared_ptr<binary_tree>& = nullptr
  );
  binary_tree(
    double,
    std::shared_ptr<binary_tree>&&,
    std::shared_ptr<binary_tree>&&
  );
  std::shared_ptr<binary_tree> left() const;
  std::shared_ptr<binary_tree> right() const;
  void set_left(const std::shared_ptr<binary_tree>&);
  void set_left(const binary_tree&);
  void set_right(const std::shared_ptr<binary_tree>&);
  void set_right(const binary_tree&);
  void insert(double);
};

}  // namespace pdcip::structures
}  // namespace pdcip

#endif  // PDCIP_CPP_STRUCTURES_TREE_H_
