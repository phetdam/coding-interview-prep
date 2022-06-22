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

#include "pdcip/types.h"

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
#define single_link_malloc_default(v) single_link_malloc(v, NULL)

/**
 * Allocate a `single_link *` with `NAN` value and no next links.
 *
 * @returns `single_link *`
 */
#define single_link_malloc_empty() single_link_malloc(NAN, NULL)

/**
 * Free a `single_link *` without freeing any next links.
 *
 * @param l `single_link *` to free
 * @returns void
 */
#define single_link_free(l) free(l)

/**
 * Free the `p` links of a linked object.
 *
 * That is, if a linked object has the `p` attribute, keep freeing the `p`
 * links until the last `p` link is seen to be `NULL`.
 *
 * @param t link type, ex. `single_link`
 * @param h `t *` head of the linked list
 * @param p attribute name, ex. `next`
 * @returns void
 */
#define free_p_links(t, h, p) \
  { \
    t *cur = h->p; \
    t *p = NULL; \
    while (cur) { \
      p = cur->p; \
      t ## _free(cur); \
      cur = p; \
    } \
  }

/**
 * Free the `prev` links of a linked object.
 *
 * @param t link type, ex. `double_link`
 * @param h `t *`head of the linked list
 * @returns void
 */
#define free_prev_links(t, h) free_p_links(t, h, prev)

/**
 * Free the `next` links of a linked object.
 *
 * @param t link type, ex. `single_link`
 * @param h `t *` head of the linked list
 * @returns void
 */
#define free_next_links(t, h) free_p_links(t, h, next)

void
single_link_free_next_links(single_link *);

void
single_link_free_deep(single_link *);

/**
 * Count `p` links starting from head link and bind to name `n_p`.
 *
 * @param t link type, ex. `double_link`
 * @param h `t *` head or link in linked list
 * @param p attribute name, ex. `next`, so bound name is `n_next`
 */
#define count_n_p(t, h, p) \
  t *cur = h->p; \
  size_t n_ ## p = 0; \
  while (cur) { \
    (n_ ## p)++; \
  }

/**
 * Count `prev` links starting from head link and bind to name `n_prev`.
 *
 * @param t link type, ex. `double_link`
 * @param h `t *` head or link in linked list
 */
#define count_n_prev(t, h) count_n_p(t, h, prev)

/**
 * Count `next` links starting from head link and bind to name `n_next`.
 *
 * @param t link type, ex. `double_link`
 * @param h `t *` head or link in linked list
 */
#define count_n_next(t, h) count_n_p(t, h, next)

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

double_link *
double_link_malloc(double, double_link *, double_link *);

/**
 * Allocate a `double_link *` with no next or prev links.
 *
 * @param v `double` value the `double_link *` should take
 * @returns `double_link *`
 */
#define double_link_malloc_default(v) double_link_malloc(v, NULL, NULL);

/**
 * Allocate a `double_link *` with `NAN` value and no next or prev links.
 *
 * @returns `double_link *`
 */
#define double_link_malloc_empty() double_link_malloc(NAN, NULL, NULL);

/**
 * Free a `double_link *` without freeing any next or prev links.
 *
 * @param l `double_link *` to free
 * @returns void
 */
#define double_link_free(l) free(l)

void
double_link_free_prev_links(double_link *);

void
double_link_free_next_links(double_link *);

void
double_link_free_deep(double_link *);

size_t
double_link_n_next(double_link *);

size_t
double_link_n_prev(double_link *);

size_t
double_link_n_links(double_link *);

/**
 * A more generic single-linked list node implementation.
 *
 * Able to hold a few different types, with the data accessed by dereferencing
 * the `data` pointer, which may point to something like a double or pointer.
 *
 * The `n_data` field indicates how many contiguous elements `*data` points to,
 * and is usually `0` unless `data_type` is a `PDCIP_<TYPE>_ARRAY` type, in
 * which case `n_data` gives the number of `<TYPE>` elements that are
 * contiguously allocated starting at `(*data)[0]`.
 *
 * When `data_type` is `PDCIP_STRING`, then `*data` can be expected to point to
 * a properly null-terminated string, and `n_data` gives the length of said
 * string. `PDCIP_CHAR_ARRAY` omits the null terminator.
 *
 * @note `void_single_link` objects **own** their data. The `*_array_malloc`
 *     functions copy the specified number of elements from the data pointer.
 */
typedef struct void_single_link {
  void *data;
  pdcip_type data_type;
  size_t n_data;
  struct void_single_link *next;
} void_single_link;

/**
 * Partially init an existing `void_single_link *` with name `link`.
 *
 * To complete the `void_single_link *` initialization, set `link->data_type`
 * with the appropriate `pdcip_type` enumeration value.
 *
 * @param link name of existing `void_single_link *`
 * @param data_ `data_type` data that `link->data` will point to
 * @param data_type C type of `data`
 * @param n_data_ `size_t` number of data elements if `data` will point to a
 *     pointer to an array, ex. `data` points to `double *`.
 * @param next `void_single_link *` next node
 */
#define void_single_link_partial_init(link, data_, data_type, n_data_, next) \
  link->data = malloc(sizeof data_); \
  *((data_type *) link->data) = data_; \
  link->n_data = n_data_; \
  link->next = next

/**
 * Create and partially init a new `void_single_link *` with name `link`.
 *
 * See `void_single_link_partial_init` for parameter documentation.
 */
#define void_single_link_partial_create(link, data_, data_type, n_data_, next) \
  void_single_link *link = malloc(sizeof *link); \
  void_single_link_partial_init(link, data_, data_type, n_data_, next)

void_single_link *
void_single_link_int_malloc(int, void_single_link *);

void_single_link *
void_single_link_int_array_malloc(int *, size_t, void_single_link *);

void_single_link *
void_single_link_double_malloc(double, void_single_link *);

void_single_link *
void_single_link_double_array_malloc(double *, size_t, void_single_link *);

/*
void_single_link *
void_single_link_void_ptr_malloc(void *, void_single_link *);

void_single_link *
void_single_link_void_ptr_array_malloc(void **, void_single_link *);
*/

#endif  /* PDCIP_LINK_H_ */
