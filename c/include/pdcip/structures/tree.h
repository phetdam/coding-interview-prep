/**
 * @file tree.h
 * @author Derek Huang
 * @brief C header for general tree implementations
 * @copyright MIT License
 */

#ifndef PDCIP_STRUCTURES_TREE_H_
#define PDCIP_STRUCTURES_TREE_H_

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

void
gen_tree_free_(gen_tree *, bool);

/**
 * Free a `gen_tree` instance on the heap, but not its children.
 *
 * @param t `gen_tree *` pointer to `gen_tree` to free
 */
#define gen_tree_free(t) gen_tree_free_(t, false)

/**
 * Free a `gen_tree` instance on the heap and free its direct children.
 *
 * Use with caution if the children are dereferenced somewhere else!
 *
 * @param t `gen_tree *` pointer to `gen_tree` to free
 */
#define gen_tree_free_deep(t) gen_tree_free_(t, true)

/**
 * Allocate a `gen_tree` instance on the heap with zero children.
 *
 * @param v `double` value to set the `gen_tree` node with
 */
#define gen_tree_malloc_default(v) gen_tree_malloc(v, 0, NULL)

/**
 * A binary tree for numeric data.
 */
typedef struct binary_tree_ {
  double value;
  struct binary_tree_ *left;
  struct binary_tree_ *right;
} binary_tree;

/*
void
gen_tree_set_children(gen_tree *, size_t, gen_tree **);

gen_tree **
gen_tree_make_children(size_t, double *);

binary_tree *
binary_tree_insert(binary_tree *, double);
*/

#endif  /* PDCIP_STRUCTURES_TREE_H_ */
