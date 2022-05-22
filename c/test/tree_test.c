/**
 * @file tree_test.c
 * @author Derek Huang
 * @brief Tests for the `gen_tree` and `binary_tree`
 * @copyright MIT License
 */

#include <stdlib.h>

#include <check.h>

#include "pdcip/tree.h"

/**
 * Test that `gen_tree_malloc_default` works as expected.
 */
START_TEST(test_gen_tree_malloc_default)
{
  gen_tree *tree = gen_tree_malloc_default(1.2);
  ck_assert_double_eq(1.2, tree->value);
  ck_assert_int_eq(0, tree->n_children);
  ck_assert_ptr_null(tree->children);
  gen_tree_free(tree);
}
END_TEST

/**
 * Return a Check `Suite *` for the tree tests.
 *
 * @note Tests are `static`, but this function is not.
 */
Suite *
make_tree_test_suite(void)
{
  Suite *tts = suite_create("tree");
  TCase *tc_memory = tcase_create("memory");
  tcase_add_test(tc_memory, test_gen_tree_malloc_default);
  suite_add_tcase(tts, tc_memory);
  return tts;
}

int
main()
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
