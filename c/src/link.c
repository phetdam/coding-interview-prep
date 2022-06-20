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
 * @param next `single_link *` next node
 */
single_link *
single_link_malloc(double value, single_link *next)
{
  single_link *head = (single_link *) malloc(sizeof(single_link));
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
  single_link *cur = head->next;
  single_link *next = NULL;
  while (cur) {
    next = cur->next;
    single_link_free(cur);
    cur = next;
  }
  head->next = NULL;
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
  single_link *cur = head->next;
  size_t n_next = 0;
  while (cur) {
    n_next++;
  }
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
