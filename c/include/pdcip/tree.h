/**
 * @file tree.h
 * @author Derek Huang
 * @brief C header for general tree implementations
 * @copyright MIT License
 */

#ifndef PDCIP_TREE_H_
#define PDCIP_TREE_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * A general multi-child tree for numeric data.
 */
typedef struct gen_tree_ {
  double value;
  size_t n_children;
  struct gen_tree_ **children;
} gen_tree;

gen_tree *
gen_tree_malloc(double, size_t, gen_tree **);

/**
 * Allocate a `gen_tree` instance on the heap with zero children.
 *
 * @param v `double` value to set the `gen_tree` node with
 * @returns `gen_tree *`
 */
#define gen_tree_malloc_default(v) gen_tree_malloc(v, 0, NULL)

/**
 * Free a `gen_tree` instance on the heap, but not its children.
 *
 * @param t `gen_tree *` pointer to `gen_tree` to free
 * @returns `void`
 */
#define gen_tree_free(t) free(t)

void
gen_tree_set_children(gen_tree *, size_t, gen_tree **);

/**
 * Reset the children of a `gen_tree *` without freeing the existing children.
 *
 * `n_children` is set to zero and the children are set to `NULL`.
 *
 * @param t `gen_tree *` pointer to `gen_tree` whose children we reset
 */
#define get_tree_reset_children(t) gen_tree_set_children(t, 0, NULL)

void
gen_tree_free_children_(gen_tree *, bool);

/**
 * Free the direct children of a `gen_tree *`.
 *
 * @param t `gen_tree *` whose direct children we wish to free
 * @returns `void`
 */
#define gen_tree_free_children(t) gen_tree_free_children_(t, false)

/**
 * Free the entire child subtree of a `gen_tree *`.
 *
 * @param t `gen_tree *` whose child subtree we wish to free
 * @returns `void`
 */
#define gen_tree_free_children_deep(t) gen_tree_free_children_(t, true)

void
gen_tree_free_deep(gen_tree *);

gen_tree **
gen_tree_make_children(size_t, double *);

/**
 * A binary tree for numeric data.
 */
typedef struct binary_tree_ {
  double value;
  struct binary_tree_ *left;
  struct binary_tree_ *right;
} binary_tree;

/*
binary_tree *
binary_tree_insert(binary_tree *, double);
*/

#endif  /* PDCIP_TREE_H_ */
