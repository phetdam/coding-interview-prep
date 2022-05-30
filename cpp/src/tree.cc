/**
 * @file tree.cc
 * @author Derek Huang
 * @brief C++ source for general tree implementations
 * @copyright MIT License
 */

#include "pdcip/cpp/tree.h"

#include <cassert>
#include <cmath>
#include <queue>
#include <memory>
#include <vector>
#include <utility>

#include "pdcip/cpp/types.h"

namespace pdcip {

/**
 * `tree` constructor.
 *
 * @param value `double` value of `tree` node
 * @param children `const tree_ptr_vector_ptr&` with children
 */
tree::tree(double value, const tree_ptr_vector_ptr& children) : value_(value)
{
  assert(children && "children cannot be nullptr");
  children_ = children;
}

/**
 * `tree` move constructor.
 *
 * @param value `double` value of `tree` node
 * @param children `tree_ptr_vector_ptr&&` with children
 */
tree::tree(double value, tree_ptr_vector_ptr&& children) : value_(value)
{
  assert(children && "children cannot be nullptr");
  children_ = std::move(children);
}

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
 * @param values `double_vector` of values to supply the children
 * @returns `tree_ptr_vector_ptr` of `tree` children
 */
tree_ptr_vector_ptr tree::make_children(const double_vector& values)
{
  return make_tree_ptr_vector<tree>(values);
}

/**
 * Return nodes in the tree by depth-first search.
 *
 * @param root `const tree_ptr&` giving the root of the tree
 * @returns `tree_ptr_vector_ptr` with all the nodes in the tree
 */
tree_ptr_vector_ptr tree::dfs(const tree_ptr& root)
{
  assert(root);
  // if no children, then root is the only one
  if (!root->children_) {
    return std::make_shared<tree_ptr_vector>(tree_ptr_vector({root}));
  }
  // recursive collection of all subtree nodes
  auto nodes_vec = std::make_shared<std::vector<tree_ptr_vector_ptr>>();
  // ignore any children that are nullptr; some subclasses, ex. binary_tree,
  // may have nullptr children. binary_tree does that so left, right methods
  // will return nullptr if missing the left and/or right child[ren]
  for (tree_ptr child : *root->children_) {
    if (child) {
      nodes_vec->push_back(tree::dfs(child));
    }
  }
  auto nodes = std::make_shared<tree_ptr_vector>();
  for (tree_ptr_vector_ptr subnodes : *nodes_vec) {
    nodes->insert(nodes->end(), subnodes->begin(), subnodes->end());
  }
  nodes->push_back(root);
  return nodes;
}

/**
 * Return nodes in the tree by breadth-first search.
 *
 * @param root `const tree_ptr&` giving the root of the tree
 * @returns `tree_ptr_vector_ptr` with all the nodes in the tree
 */
tree_ptr_vector_ptr tree::bfs(const tree_ptr& root)
{
  assert(root);
  tree_ptr_vector_ptr nodes = std::make_shared<tree_ptr_vector>();
  // queue is a wrapped deque, which makes it slightly slower
  std::queue<tree_ptr> node_queue({root});
  while (node_queue.size()) {
    tree_ptr cur_node = node_queue.front();
    node_queue.pop();
    for (tree_ptr child : *(cur_node->children())) {
      if (child) {
        node_queue.push(child);
      }
    }
    nodes->push_back(cur_node);
  }
  return nodes;
}

/**
 * Return `double_vector_ptr` with `tree` values from a `tree_ptr` vector.
 *
 * @param tree_nodes `const tree_ptr_vector_ptr&` with `tree_ptr` instances
 */
double_vector_ptr tree::value_vector(const tree_ptr_vector_ptr& tree_nodes)
{
  double_vector_ptr
  node_values = std::make_shared<double_vector>(tree_nodes->size());
  for (std::size_t i = 0; i < tree_nodes->size(); i++) {
    (*node_values)[i] = tree_nodes->at(i)->value();
  }
  return node_values;
}

/**
 * `binary_tree` constructor.
 *
 * @param value `double` value of `binary_tree` node
 * @param left `const binary_tree_ptr&` for left child
 * @param right `const binary_tree_ptr&` for right child
 */
binary_tree::binary_tree(
  double value, const binary_tree_ptr& left, const binary_tree_ptr& right)
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
double_vector_ptr binary_tree::sorted_values()
{
  if (!children()) {
    return std::make_shared<std::vector<double>>();
  }
  double_vector_ptr left_values(
    left() ? left()->sorted_values() : std::make_shared<std::vector<double>>()
  );
  double_vector_ptr right_values(
    right() ? right()->sorted_values() : std::make_shared<std::vector<double>>()
  );
  if (!std::isnan(value())) {
    left_values->push_back(value());
  }
  left_values->insert(
    left_values->end(), right_values->begin(), right_values->end()
  );
  return left_values;
}

}  // namespace pdcip
