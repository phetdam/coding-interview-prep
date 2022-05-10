"""Unit tests for tree search methods.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

import unittest

# pylint: disable=missing-function-docstring,relative-beyond-top-level
from ...data_structures.tests.test_binary_tree import BinaryTreeTestCaseBase
from ..tree_search import AlgoLoopType, bfs, dfs


class BinaryTreeSearchTestCase(BinaryTreeTestCaseBase):
    """Validate that tree search methods work correctly for binary trees."""

    # the expected values we want to get
    dfs_values = [1.2, 2.2, -3, 8.9, 6.7, 5.6, 9., 4.]
    bfs_values = [4., -3., 9., 2.2, 5.6, 1.2, 6.7, 8.9]

    def test_dfs_recursive(self):
        self.assertEqual(self.dfs_values, dfs(self.tree))

    def test_dfs_iterative(self):
        self.assertEqual(
            self.dfs_values,
            dfs(self.tree, method=AlgoLoopType.ITERATIVE)
        )

    def test_bfs_iterative(self):
        self.assertEqual(self.bfs_values, bfs(self.tree))


if __name__ == "__main__":
    unittest.main()
