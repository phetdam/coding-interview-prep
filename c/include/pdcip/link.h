/**
 * @file link.h
 * @author Derek Huang
 * @brief C header for linked list implementations
 * @copyright MIT License
 */

#ifndef PDCIP_LINK_H_
#define PDCIP_LINK_H_

#include <stddef.h>

/**
 * A simple single-linked list node implementation.
 */
typedef struct single_link {
  double value;
  struct single_link *next;
} single_link;

size_t
single_link_n_next(const single_link *);

size_t
single_link_n_links(const single_link *);

/**
 * A simple double-linked list node implementation.
 */
typedef struct double_link {
  double value;
  struct double_link *prev;
  struct double_link *next;
} double_link;

#endif  /* PDCIP_LINK_H_ */
