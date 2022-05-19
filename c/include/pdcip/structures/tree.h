/**
 * @file tree.h
 * @author Derek Huang
 * @brief C header for general tree implementations
 * @copyright MIT License
 */

#ifndef PDCIP_STRUCTURES_TREE_H_
#define PDCIP_STRUCTURES_TREE_H_

#include <stdlib.h>

/**
 * A general multi-child tree for numeric data.
 */
typedef struct gen_tree_ {
  double value;
  size_t n_children;
  struct gen_tree_ **children;
} gen_tree;

/**
 * A binary tree for numeric data.
 */
typedef struct binary_tree_ {
  double value;
  struct binary_tree_ *left;
  struct binary_tree_ *right;
} binary_tree;

void
gen_tree_set_children(gen_tree *tree, size_t n_children, gen_tree **children);

binary_tree *
binary_tree_insert(binary_tree *tree, double value);

#endif  /* PDCIP_STRUCTURES_TREE_H_ */
