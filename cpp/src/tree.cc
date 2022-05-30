/**
 * @file tree.cc
 * @author Derek Huang
 * @brief C++ source for general tree implementations
 * @copyright MIT License
 */

#include "pdcip/cpp/tree.h"

#include <cmath>
#include <memory>
#include <vector>
#include <utility>

namespace pdcip {

/**
 * `tree` constructor.
 *
 * @param value `double` value of `tree` node
 * @param children `const tree_ptr_vector_ptr&` with children
 */
tree::tree(double value, const tree_ptr_vector_ptr& children)
  : value_(value), children_(children)
{}

/**
 * `tree` move constructor.
 *
 * @param value `double` value of `tree` node
 * @param children `tree_ptr_vector_ptr&&` with children
 */
tree::tree(double value, tree_ptr_vector_ptr&& children)
  : value_(value), children_(children)
{}

/**
 * Getter for the `tree` value.
 */
double tree::value() const { return value_; }

/**
 * Getter for the `tree` children.
 */
const tree_ptr_vector_ptr& tree::children() const { return children_; }

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
 * @param children `const tree_ptr_vector_ptr&` with children
 */
void tree::set_children(const tree_ptr_vector_ptr& children)
{
  children_ = children;
}

/**
 * Move setter for the `tree` children.
 *
 * @param children `tree_ptr_vector_ptr&&` to move new children from
 */
void tree::set_children(tree_ptr_vector_ptr&& children)
{
  children_ = std::move(children);
}

/**
 * Convenience method to generate `tree` children.
 *
 * @param values `std::vector<double>` of values to supply the children
 */
tree_ptr_vector_ptr tree::make_children(const std::vector<double>& values)
{
  return make_tree_ptr_vector<tree>(values);
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
      std::make_shared<tree_ptr_vector>(
        tree_ptr_vector(
          { std::static_pointer_cast<tree>(left),
            std::static_pointer_cast<tree>(right)})))
{}

/**
 * Return `std::shared_ptr` to left `binary_tree` child.
 */
std::shared_ptr<binary_tree> binary_tree::left() const
{
  return std::dynamic_pointer_cast<binary_tree>(children()->at(0));
}

/**
 * Return `std::shared_ptr` to right `binary_tree` child.
 */
std::shared_ptr<binary_tree> binary_tree::right() const
{
  return std::dynamic_pointer_cast<binary_tree>(children()->at(1));
}

/**
 * Set the left node of the `binary_tree` with `std::shared_ptr` copy.
 *
 * @param new_left `const std::shared_ptr<binary_tree>&` new left node
 */
void binary_tree::set_left(const std::shared_ptr<binary_tree>& new_left)
{
  set_children(
    std::make_shared<tree_ptr_vector>(
      tree_ptr_vector(
        {std::static_pointer_cast<tree>(new_left), std::move(children()->at(1))}
      )
    )
  );
}

/**
 * Set the left node of the `binary_tree` with `std::shared_ptr` move.
 *
 * @param new_left `std::shared_ptr<binary_tree>&&` new left node
 */
void binary_tree::set_left(std::shared_ptr<binary_tree>&& new_left)
{
  set_children(
    std::make_shared<tree_ptr_vector>(
      tree_ptr_vector(
        {std::static_pointer_cast<tree>(new_left), std::move(children()->at(1))}
      )
    )
  );
}

/**
 * Set the right node of the `binary_tree` with `std::shared_ptr` copy.
 *
 * @param new_right `const std::shared_ptr<binary_tree>&` new right node
 */
void binary_tree::set_right(const std::shared_ptr<binary_tree>& new_right)
{
  set_children(
    std::make_shared<tree_ptr_vector>(
      tree_ptr_vector(
        {
          std::move(children()->at(0)),
          std::static_pointer_cast<tree>(new_right)
        }
      )
    )
  );
}

/**
 * Set the right node of the `binary_tree` with `std::shared_ptr` move.
 *
 * @param new_right `std::shared_ptr<binary_tree>&&` new right node
 */
void binary_tree::set_right(std::shared_ptr<binary_tree>&& new_right)
{
  set_children(
    std::make_shared<tree_ptr_vector>(
      tree_ptr_vector(
        {
          std::move(children()->at(0)),
          std::static_pointer_cast<tree>(new_right)
        }
      )
    )
  );
}

/**
 * Insert a value into the `binary_tree`.
 *
 * If the value already exists in the tree, the tree is left unmodified. If the
 * instance itself has `NAN` as a value, then its value is updated to `value`.
 *
 * @param value `double` to insert into the tree
 * @returns `binary_tree_ptr` to the node `value` was inserted in.
 */
const binary_tree* binary_tree::insert(double value)
{
  if (std::isnan(this->value())) {
    set_value(value);
    return this;
  }
  if (value == this->value()) {
    return this;
  }
  if (value < this->value()) {
    if (!left()) {
      set_left(std::make_shared<binary_tree>());
    }
    return left()->insert(value);
  }
  if (!right()) {
    set_right(std::make_shared<binary_tree>());
  }
  return right()->insert(value);
}

/**
 * Return values in the `binary_tree` in ascending order.
 */
std::vector<double> binary_tree::sorted_values()
{
  if (!children()) {
    return std::vector<double>();
  }
  std::vector<double> left_values(
    left() ? left()->sorted_values() : std::vector<double>()
  );
  std::vector<double> right_values(
    right() ? right()->sorted_values() : std::vector<double>()
  );
  if (!std::isnan(value())) {
    left_values.push_back(value());
  }
  left_values.insert(
    left_values.end(), right_values.begin(), right_values.end()
  );
  return left_values;
}

}  // namespace pdcip
