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

#include "pdcip/helpers.h"

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
 * @param deep `bool` where if `true` we free all child subtrees, while if
 *    `false` we only free the direct children of `tree`.
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
  gen_tree_free_children_deep(tree);
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
  map_func(gen_tree_malloc_default, values, children, n);
  return children;
}

/**
 * Frees an array of `gen_tree *` allocated by `gen_tree_make_children`.
 *
 * @note To [deep] free the children of a `gen_tree *`, it is preferred to use
 *    macros `gen_tree_free_children`, `gen_tree_free_children_deep` instead
 *    of this function or its macros on the `gen_tree` children.
 *
 * @param children `gen_tree **` to contiguous `gen_tree *`
 * @param n_children `size_t` giving number of `gen_tree *` to free
 * @param deep `bool` where if `true`, the entire child subtree of each
 *    `gen_tree *` will also be freed. If `false`, child subtrees left alone.
 */
void
gen_tree_free_children_array_(gen_tree **children, size_t n_children, bool deep)
{
  assert(children && n_children);
  for (size_t i = 0; i < n_children; i++) {
    if (deep) {
      gen_tree_free_children_deep(children[i]);
    }
    gen_tree_free(children[i]);
  }
  free(children);
}

/**
 * Perform DFS on a `gen_tree`.
 *
 * @param tree `gen_tree *` giving the root of the tree
 * @param n_nodes_p `size_t *` giving address to a writable `size_t` which
 *    will be assigned the number of `gen_tree *` in the specified `tree`.
 * @returns `gen_tree **` to array of `gen_tree *` of the nodes in the tree
 *    with `tree` as the root. Can be `NULL` if tree is `NULL`. Memory used
 *    by returned `gen_tree **` to hold `gen_tree *` must be freed.
 */
gen_tree **
gen_tree_dfs(const gen_tree *tree, size_t *n_nodes_p)
{
  assert(n_nodes_p);
  // if NULL, write 0 and return NULL
  if (!tree) {
    *n_nodes_p = 0;
    return NULL;
  }
  // if no children, we write 1 and return array of just tree
  if (!tree->n_children) {
    *n_nodes_p = 1;
    gen_tree **nodes = (gen_tree **) malloc(sizeof(gen_tree *));
    // silence warning about discarding const qualifier
    *nodes = (gen_tree *) tree;
    return nodes;
  }
  // array of gen_tree **, each of which points to array of gen_tree *
  gen_tree ***nodes_ar = (gen_tree ***) malloc(
    tree->n_children * sizeof(gen_tree **)
  );
  // holds number of nodes in each subtree
  size_t *n_nodes_ar = (size_t *) malloc(tree->n_children * sizeof(size_t));
  // call recursively to populate
  for (size_t i = 0; i < tree->n_children; i++) {
    nodes_ar[i] = gen_tree_dfs(tree->children[i], n_nodes_ar + i);
  }
  // get total number of nodes in tree rooted at tree
  array_sum_offset(n_nodes, size_t, n_nodes_ar, tree->n_children, 1);
  // number of nodes (gen_tree *) we have copied so far from nodes_ar
  size_t n_copied = 0;
  // construct new children array we want to return
  gen_tree **nodes = (gen_tree **) malloc(n_nodes * sizeof(gen_tree *));
  for (size_t i = 0; i < tree->n_children; i++) {
    for (size_t j = 0; j < n_nodes_ar[i]; j++) {
      nodes[n_copied + j] = nodes_ar[i][j];
    }
    n_copied += n_nodes_ar[i];
  }
  // silence warning about discarding const qualifier
  nodes[n_nodes - 1] = (gen_tree *) tree;
  // don't need these anymore
  for (size_t i = 0; i < tree->n_children; i++) {
    // nodes_ar[i] is NULL when n_nodes_ar[i] is 0
    if (nodes_ar[i]) {
      free(nodes_ar[i]);
    }
  }
  free(nodes_ar);
  free(n_nodes_ar);
  return nodes;
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
 * Free the children of a `binary_tree`.
 *
 * @param tree `binary_tree *` whose children we will free
 * @param deep `bool` where if `true` we free all child subtrees, while if
 *    `false` we only free the direct children of `tree`.
 */
void
binary_tree_free_children_(binary_tree *tree, bool deep)
{
  assert(tree);
  if (tree->left) {
    if (deep) {
      binary_tree_free_children_deep(tree->left);
    }
    binary_tree_free(tree->left);
  }
  if (tree->right) {
    if (deep) {
      binary_tree_free_children_deep(tree->right);
    }
    binary_tree_free(tree->right);
  }
  tree->left = NULL;
  tree->right = NULL;
}

/**
 * Free a `binary_tree` instance on the heap and frees its child subtrees.
 *
 * Use with caution if the children are dereferenced somewhere else!
 *
 * @param t `binary_tree *` pointer to `binary_tree` to free
 * @returns `void`
 */
void
binary_tree_free_deep(binary_tree *tree)
{
  binary_tree_free_children_deep(tree);
  binary_tree_free(tree);
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

/**
 * Return the values stored in the `binary_tree` in ascending order.
 *
 * In the special case that value of the tree is `NAN`, then `NULL` is returned
 * and `n_values_p` will have zero written to it.
 *
 * @param tree `const binary_tree *` binary tree we want sorted values from
 * @param n_values_p `size_t *` address to a `size_t` which will be assigned
 *    the number of values that were retrieved from the `binary_tree`.
 */
double *
binary_tree_sorted_values(const binary_tree *tree, size_t *n_values_p)
{
  assert(tree && n_values_p);
  if (isnan(tree->value)) {
    *n_values_p = 0;
    return NULL;
  }
  size_t n_values_left = 0;
  size_t n_values_right = 0;
  double *values_left = NULL;
  double *values_right = NULL;
  if (tree->left){
    values_left = binary_tree_sorted_values(tree->left, &n_values_left);
  }
  if (tree->right) {
    values_right = binary_tree_sorted_values(tree->right, &n_values_right);
  }
  size_t n_values = n_values_left + n_values_right + 1;
  double *values = (double *) malloc(n_values * sizeof(double));
  for (size_t i = 0; i < n_values_left; i++) {
    values[i] = values_left[i];
  }
  values[n_values_left] = tree->value;
  for (size_t i = 0; i < n_values_right; i++) {
    values[n_values_left + i + 1] = values_right[i];
  }
  free(values_left);
  free(values_right);
  *n_values_p = n_values;
  return values;
}
