/**
 * @file link.h
 * @author Derek Huang
 * @brief C header for linked list implementations
 * @copyright MIT License
 */

#ifndef PDCIP_LINK_H_
#define PDCIP_LINK_H_

#include <stddef.h>
#include <stdlib.h>

/**
 * A simple single-linked list node implementation.
 */
typedef struct single_link {
  double value;
  struct single_link *next;
} single_link;

single_link *
single_link_malloc(double, single_link *);

/**
 * Allocate a `single_link *` with no next links.
 *
 * @param v `double` value the `single_link *` should take
 * @returns `single_link *`
 */
#define single_link_malloc_default(v) single_link_malloc(v, NULL);

/**
 * Allocate a `single_link *` with `NAN` value and no next links.
 *
 * @returns `single_link *`
 */
#define single_link_malloc_empty() single_link_malloc(NAN, NULL);

/**
 * Free a `single_link *` without freeing any next links.
 *
 * @param l `single_link *` to free
 * @returns void
 */
#define single_link_free(l) free(l)

void
single_link_free_next_links(single_link *);

void
single_link_free_deep(single_link *);

size_t
single_link_n_next(single_link *);

size_t
single_link_n_links(single_link *);

/**
 * A simple double-linked list node implementation.
 */
typedef struct double_link {
  double value;
  struct double_link *prev;
  struct double_link *next;
} double_link;

#endif  /* PDCIP_LINK_H_ */
