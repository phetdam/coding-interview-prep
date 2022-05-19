/**
 * @file tree.c
 * @author Derek Huang
 * @brief C source for general tree implementation
 * @copyright MIT License
 */

#include "pdcip/structures/tree.h"

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
  gen_tree *tree = malloc(sizeof(gen_tree));
  tree->value = value;
  tree->n_children = n_children;
  tree->children = children;
  return tree;
}

/**
 * Free a `gen_tree` instance on heap, optionally also freeing its children.
 *
 * @param tree `gen_tree *` pointer to `gen_tree` to free
 * @param deep `bool` where `true` also frees children, `false` does not.
 */
void
gen_tree_free_(gen_tree *tree, bool deep)
{
  if (deep) {
    for (size_t i = 0; i < tree->n_children; i++) {
      free(tree->children[i]);
    }
    free(tree->children);
  }
  free(tree);
}
