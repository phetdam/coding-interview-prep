/**
 * @file tree.c
 * @author Derek Huang
 * @brief C source for general tree implementation
 * @copyright MIT License
 */

#include "pdcip/tree.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Allocate a `gen_tree` instance on heap with given children.
 *
 * @param value `double` value for the tree node
 * @param n_children `size_t` number of children. If 0, make `children` `NULL`
 * @param children `gen_tree **` pointer to heap array of `gen_tree *`. Must be
 *    `NULL` when `n_children` is 0, otherwise fails.
 */
gen_tree *
gen_tree_malloc(double value, size_t n_children, gen_tree **children)
{
  // n_children can only be zero if children is NULL
  assert((n_children == 0 && !children) || (n_children > 0 && children));
  gen_tree *tree = (gen_tree *) malloc(sizeof(gen_tree));
  tree->value = value;
  tree->n_children = n_children;
  tree->children = children;
  return tree;
}

/**
 * Set the children of a `gen_tree *` without freeing the existing children.
 *
 * If `n_children` is zero, `children` must be `NULL` and `tree->children` will
 * be set to `NULL` in order to keep the `gen_tree` state consistent.
 *
 * @param tree `gen_tree *` tree whose children we will set
 * @param n_children `size_t` number of new children
 * @param children `gen_tree **` pointing to `gen_tree *` new children
 */
void
gen_tree_set_children(gen_tree *tree, size_t n_children, gen_tree **children)
{
  assert(tree && (children || (n_children == 0 && !children)));
  tree->n_children = n_children;
  tree->children = children;
}

/**
 * Free the children of a `gen_tree`.
 *
 * @param tree `gen_tree *` whose children we will free
 * @param deep `bool` where if `true` we free the entire child subtree, while
 *    if `false` we only free the direct children of `tree`.
 */
void
gen_tree_free_children_(gen_tree *tree, bool deep)
{
  assert(tree);
  if (tree->children) {
    for (size_t i = 0; i < tree->n_children; i++) {
      if (deep) {
        gen_tree_free_children_deep(tree->children[i]);
      }
      gen_tree_free(tree->children[i]);
    }
    free(tree->children);
  }
  tree->n_children = 0;
  tree->children = NULL;
}

/**
 * Free a `gen_tree` instance on the heap and free its child subtree.
 *
 * Use with caution if the children are dereferenced somewhere else!
 *
 * @param t `gen_tree *` pointer to `gen_tree` to free
 * @returns `void`
 */
void
gen_tree_free_deep(gen_tree *tree)
{
  gen_tree_free_children(tree);
  gen_tree_free(tree);
}

/**
 * Convenience function that allocates `gen_tree` children on the heap.
 *
 * If `n` is 0, then `values` is ignored and `NULL` is returned.
 *
 * @param n number of `gen_tree *` children in array
 * @param values `const double *` of values to populate children with
 * @returns `gen_tree **` to `n` `gen_tree *`
 */
gen_tree **
gen_tree_make_children(size_t n, const double *values)
{
  // hack to add a message to the assert
  assert(values && "values must not be NULL");
  if (!n) {
    return NULL;
  }
  gen_tree **children = (gen_tree **) malloc(n * sizeof(gen_tree *));
  for (size_t i = 0; i < n; i++) {
    children[i] = gen_tree_malloc_default(values[i]);
  }
  return children;
}

/**
 * Allocate a `binary_tree` instance on the heap.
 *
 * @param value `double` value to give the `binary_tree`, can be `NAN` if root
 * @param left `const binary_tree *` to set the left child as, can be `NULL`
 * @param right `const binary_tree *` to set the right child as, can be `NULL`
 */
binary_tree *
binary_tree_malloc(
  double value,
  const binary_tree *left,
  const binary_tree *right)
{
  binary_tree *tree = (binary_tree *) malloc(sizeof(binary_tree));
  tree->value = value;
  // forcible cast to suppress -Wdiscard-qualifiers. left, right are untouched
  tree->left = (binary_tree *) left;
  tree->right = (binary_tree *) right;
  return tree;
}

/**
 * Insert a value into the `binary_tree`.
 *
 * If the root has `NAN` as a value, the root's value is set to `value`, while
 * if `value` is equal to the root's value, then nothing is done.
 *
 * @param tree `binary_tree *` root of the binary tree
 * @param value `double` value to insert into the binary tree, cannot be `NAN`
 * @returns `binary_tree *` pointer to node `value` was inserted in
 */
binary_tree *
binary_tree_insert(binary_tree *tree, double value)
{
  assert(tree && !isnan(value));
  if (isnan(tree->value)) {
    tree->value = value;
    return tree;
  }
  if (value == tree->value) {
    return tree;
  }
  if (value < tree->value) {
    if (!tree->left) {
      tree->left = binary_tree_malloc_empty();
    }
    return binary_tree_insert(tree->left, value);
  }
  if (!tree->right) {
    tree->right = binary_tree_malloc_empty();
  }
  return binary_tree_insert(tree->right, value);
}
