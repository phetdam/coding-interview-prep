/**
 * @file tree.cc
 * @author Derek Huang
 * @brief C++ source for general tree implementations
 * @copyright MIT License
 */

#include "pdcip/cpp/structures/tree.h"

#include <memory>
#include <vector>
#include <utility>

namespace pdcip {
namespace structures {

/**
 * `tree` constructor.
 * 
 * @param value `double` value of `tree` node
 * @param children `const tree_children_ptr&` with children
 */
tree::tree(double value, const tree_children_ptr& children)
  : value_(value), children_(children)
{}

/**
 * Getter for the `tree` value.
 */
double tree::value() const { return value_; }

/**
 * Getter for the `tree` children.
 */
const tree_children_ptr& tree::children() const { return children_; }

/**
 * Return number of direct children of the `tree`.
 */
std::size_t tree::n_children() const
{
  return static_cast<std::size_t>(children_->size());
}

/**
 * Setter for the `tree` value.
 * 
 * @param value `double` new value to assign the `tree` node
 */
void tree::set_value(double value) { value_ = value; }

/**
 * Copy setter for the `tree` children.
 * 
 * @param children `const tree_children_ptr&` with children
 */
void tree::set_children(const tree_children_ptr& children)
{
  children_ = children;
}

/**
 * Move setter for the `tree` children.
 * 
 * @param children `tree_children_ptr&&` to move new children from
 */
void tree::set_children(tree_children_ptr&& children)
{
  children_ = std::move(children);
}

/**
 * `binary_tree` constructor.
 * 
 * @param value `double` value of `binary_tree` node
 * @param left `const std::shared_ptr<binary_tree>&` for left child
 * @param right `const std::shared_ptr<binary_tree>&` for right child
 */
binary_tree::binary_tree(
  double value,
  const std::shared_ptr<binary_tree>& left,
  const std::shared_ptr<binary_tree>& right)
  : tree(
      value,
      std::make_shared<tree_children>(
        tree_children(
          { std::static_pointer_cast<tree>(left),
            std::static_pointer_cast<tree>(right) })))
{}

/**
 * Return `std::shared_ptr` to left `binary_tree` child.
 */
std::shared_ptr<binary_tree> binary_tree::left() const {
  return std::dynamic_pointer_cast<binary_tree>(children()->at(0));
}

/**
 * Return `std::shared_ptr` to right `binary_tree` child.
 */
std::shared_ptr<binary_tree> binary_tree::right() const {
  return std::dynamic_pointer_cast<binary_tree>(children()->at(1));
}

}  // namespace pdcip::structures
}  // namespace pdcip
