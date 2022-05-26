/**
 * @file tree_test.c
 * @author Derek Huang
 * @brief Tests for the `gen_tree` and `binary_tree`
 * @copyright MIT License
 */

#include <stdlib.h>

#include <check.h>

#include "pdcip/tree.h"

// commonly reused gen_tree root, number of and values for direct children
static gen_tree *gen_root;
static const size_t n_direct_children = 5;
static const double direct_values[] = {4.5, 1.7, 8.9, 2, 6.5};

// macro to make direct children
#define GEN_TREE_MAKE_DIRECT_CHILDREN() \
  gen_tree_make_children(n_direct_children, direct_values);

/**
 * Setup function for `gen_root` fixture.
 *
 * Also checks that the `gen_root` instance is initialized correctly.
 */
static void
gen_root_setup(void)
{
  gen_root = gen_tree_malloc_default(1.7);
  ck_assert_double_eq(1.7, gen_root->value);
  ck_assert_int_eq(0, gen_root->n_children);
  ck_assert_ptr_null(gen_root->children);
}

/**
 * Teardown function for `gen_root` fixture.
 */
static void
gen_root_teardown(void)
{
  gen_tree_free(gen_root);
}

/**
 * Test that making and freeing direct tree children work as intended.
 */
START_TEST(test_gen_tree_make_free_children)
{
  gen_tree **children = GEN_TREE_MAKE_DIRECT_CHILDREN();
  for (unsigned int i = 0; i < n_direct_children; i++) {
    ck_assert_double_eq(direct_values[i], children[i]->value);
    ck_assert_int_eq(0, children[i]->n_children);
    ck_assert_ptr_null(children[i]->children);
  }
  gen_tree_set_children(gen_root, n_direct_children, children);
  gen_tree_free_children(gen_root);
  ck_assert_int_eq(0, gen_root->n_children);
  ck_assert_ptr_null(gen_root->children);
}
END_TEST

/**
 * Test that making and freeing an entire subtree works as intended.
 */
START_TEST(test_gen_tree_make_free_children_deep)
{
  gen_tree **first_children = GEN_TREE_MAKE_DIRECT_CHILDREN();
  gen_tree **second_children = GEN_TREE_MAKE_DIRECT_CHILDREN();
  gen_tree_set_children(gen_root, n_direct_children, first_children);
  gen_tree_set_children(first_children[1], n_direct_children, second_children);
  gen_tree_free_children_deep(gen_root);
  ck_assert_int_eq(0, gen_root->n_children);
  ck_assert_ptr_null(gen_root->children);
}
END_TEST

/**
 * Return a Check `Suite *` for the tree tests.
 *
 * @note Tests are `static`, but this function is not. Tests are run in order.
 */
Suite *
make_tree_test_suite(void)
{
  Suite *ts_tree = suite_create("tree");
  TCase *tc_memory = tcase_create("memory");
  // add checked fixtures
  tcase_add_checked_fixture(tc_memory, gen_root_setup, gen_root_teardown);
  // add test cases
  tcase_add_test(tc_memory, test_gen_tree_make_free_children);
  tcase_add_test(tc_memory, test_gen_tree_make_free_children_deep);
  suite_add_tcase(ts_tree, tc_memory);
  return ts_tree;
}

int
main(void)
{
  int n_failed;
  Suite *tree_suite = make_tree_test_suite();
  SRunner *runner = srunner_create(tree_suite);
  /**
   * by using CK_ENV, if we want to change the verbosity level, we can just
   * set the CK_VERBOSITY environment variable, ex. "silent", "minimal",
   * "normal" (default if not specified), "verbose".
   */
  srunner_run_all(runner, CK_ENV);
  n_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (!n_failed) ? EXIT_SUCCESS : EXIT_FAILURE;
}
