/**
 * @file link.c
 * @author Derek Huang
 * @brief C source for linked list implementations
 * @copyright MIT License
 */

#include "pdcip/link.h"

#include <stddef.h>
#include <stdlib.h>

/**
 * Allocate a `single_link *`.
 *
 * @param value `double` value the `single_link *` should take, can be `NAN`
 * @param next `single_link *` next node, can be `NULL`
 */
single_link *
single_link_malloc(double value, single_link *next)
{
  single_link *head = malloc(sizeof *head);
  head->value = value;
  head->next = next;
  return head;
}

/**
 * Free all the subsequent links starting with the next link.
 *
 * The `next` link of `head` will then be set to `NULL`.
 *
 * @param head `single_link *` head of the linked list
 */
void
single_link_free_next_links(single_link *head)
{
  /*
  single_link *cur = head->next;
  single_link *next = NULL;
  while (cur) {
    next = cur->next;
    single_link_free(cur);
    cur = next;
  }
  head->next = NULL;
  */
 free_next_links(single_link, head);
}

/**
 * Free a `single_link *` and its subsequent links.
 *
 * @param head `single_link *` head of the linked list
 */
void
single_link_free_deep(single_link *head)
{
  single_link_free_next_links(head);
  single_link_free(head);
}

/**
 * Return number of links that come next after `head`.
 *
 * @param head `single_link *` head of the linked list
 */
size_t
single_link_n_next(single_link *head)
{
  /*
  single_link *cur = head->next;
  size_t n_next = 0;
  while (cur) {
    n_next++;
  }
  */
  count_n_next(single_link, head);
  return n_next;
}

/**
 * Return number of links in linked list starting at `head`.
 *
 * @param head `single_link *` head of the linked list
 */
size_t
single_link_n_links(single_link *head)
{
  return 1 + single_link_n_next(head);
}

/**
 * Allocate a `double_link *`.
 *
 * @param value `double` value the `double_link *` should take, can be `NAN`
 * @param prev `double_link *` prev node, can be `NULL`
 * @param next `double_link *` next node, can be `NULL`
 */
double_link *
double_link_malloc(double value, double_link *prev, double_link *next)
{
  double_link *head = malloc(sizeof *head);
  head->value = value;
  head->prev = prev;
  head->next = next;
  return head;
}

/**
 * Free all the prev links in the linked list starting with `link->prev`.
 *
 * The `prev` link of `link` will then be set to `NULL`.
 *
 * @param link `double_link *` in a of the linked list
 */
void
double_link_free_prev_links(double_link *link)
{
 free_prev_links(double_link, link);
}

/**
 * Free all the next links in the linked list starting with `link->next`.
 *
 * The `next` link of `link` will then be set to `NULL`.
 *
 * @param head `double_link *` head of the linked list
 */
void
double_link_free_next_links(double_link *link)
{
 free_next_links(double_link, link);
}

/**
 * Free a `double_link *` and all its prev and next links.
 *
 * @param head `double_link *` link of the linked list
 */
void
double_link_free_deep(double_link *link)
{
  double_link_free_prev_links(link);
  double_link_free_next_links(link);
  double_link_free(link);
}

/**
 * Return number of `prev` links that come before `link`.
 *
 * @param link `double_link *` head of the linked list
 */
size_t
double_link_n_prev(double_link *link)
{
  count_n_prev(double_link, link);
  return n_prev;
}

/**
 * Return number of `next` links that come after `link`.
 *
 * @param link `double_link *` head of the linked list
 */
size_t
double_link_n_next(double_link *link)
{
  count_n_next(double_link, link);
  return n_next;
}

/**
 * Allocate `void_single_link *` holding `int` data.
 *
 * @param value `int` to store in `*data`
 * @param next `void_single_link *` next node
 */
void_single_link *
void_single_link_int_malloc(int value, void_single_link *next)
{
  void_single_link_partial_create(head, value, int, 0, next);
  head->data_type = PDCIP_INT;
  return head;
}

/**
 * Allocate `void_single_link *` holding `int *` pointing to `int` array.
 *
 * @param values `int *` pointing to contiguous `int` values
 * @param n_values `size_t` number of contiguous `int` values
 * @param next `void_single_link *` next node
 */
void_single_link *
void_single_link_int_array_malloc(
  int *values, size_t n_values, void_single_link *next)
{
  int *copy_values = malloc(n_values * (sizeof *copy_values));
  for (size_t i = 0; i < n_values; i++) {
    copy_values[i] = values[i];
  }
  void_single_link_partial_create(head, copy_values, int *, n_values, next);
  head->data_type = PDCIP_INT_ARRAY;
  return head;
}

/**
 * Allocate `void_single_link *` holding `double` data.
 *
 * @param value `double` to store in `*data`
 * @param next `void_single_link *` next node
 */
void_single_link *
void_single_link_double_malloc(double value, void_single_link *next)
{
  void_single_link_partial_create(head, value, double, 0, next);
  head->data_type = PDCIP_DOUBLE;
  return head;
}

/**
 * Allocate `void_single_link *` holding `double *` pointing to `double` array.
 *
 * @param values `double *` pointing to contiguous `double` values
 * @param n_values `size_t` number of contiguous `double` values
 * @param next `void_single_link *` next node
 */
void_single_link *
void_single_link_double_array_malloc(
  double *values, size_t n_values, void_single_link *next)
{
  double *copy_values = malloc(n_values * (sizeof *copy_values));
  for (size_t i = 0; i < n_values; i++) {
    copy_values[i] = values[i];
  }
  void_single_link_partial_create(head, copy_values, double *, n_values, next);
  head->data_type = PDCIP_DOUBLE_ARRAY;
  return head;
}
