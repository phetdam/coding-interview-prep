"""Unit tests for tree implementations.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

from copy import deepcopy

import unittest

# pylint: disable=missing-function-docstring
from pdcip.algorithms.search.core import SearchStrategy
from pdcip.structures.tree import BinaryTree, Tree


class TreeDunderTestCase(unittest.TestCase):
    """Test that dunder methods behave as expected."""

    def test_repr(self):
        # children should not show up
        head = Tree(value=56.2)
        self.assertEqual(
            f"{Tree.__name__}(value={head.value})",
            repr(head)
        )

    def test_compare(self):
        # note if children are different, these won't be equal
        head_a = Tree(value=2.4)
        head_aa = deepcopy(head_a)
        head_b = Tree(value=5.6)
        self.assertEqual(head_a, head_aa)
        self.assertLessEqual(head_a, head_aa, msg=f"{head_a} > {head_aa}")
        self.assertGreaterEqual(head_a, head_aa, msg=f"{head_a} < {head_aa}")
        self.assertLess(head_a, head_b, msg=f"{head_a} >= {head_b}")
        self.assertGreater(head_b, head_a, msg=f"{head_b} <= {head_a}")


class BinaryTreeTestCaseBase(unittest.TestCase):
    """Base test case for BinaryTree testing."""

    values = [4., -3., 2.2, 9., 5.6, 6.7, 1.2, 8.9]
    # [-3.0, 1.2, 2.2, 4.0, 5.6, 6.7, 8.9, 9.0]
    sorted_values = sorted(values)

    @classmethod
    def setUpClass(cls):
        cls.tree = BinaryTree()
        for value in cls.values:
            cls.tree.insert(value)


class BinaryTreeTestCase(BinaryTreeTestCaseBase):
    """Test the BinaryTree class."""

    def test_insert_unique(self):
        self.assertEqual(self.sorted_values, self.tree.sorted_values())

    def test_search_exact(self):
        for value in self.values:
            self.assertEqual(value, self.tree.search(value).value)

    def test_search_above(self):
        # search for middle value
        self.assertEqual(
            5.6,
            self.tree.search(5, strategy=SearchStrategy.FROM_ABOVE).value
        )
        # search below lowest value
        self.assertEqual(
            -3.,
            self.tree.search(-4, strategy=SearchStrategy.FROM_ABOVE).value
        )
        # search above highest value
        self.assertIsNone(
            self.tree.search(10, strategy=SearchStrategy.FROM_ABOVE)
        )

    def test_search_below(self):
        # search for middle value
        self.assertEqual(
            4.,
            self.tree.search(5, strategy=SearchStrategy.FROM_BELOW).value
        )
        # search below lowest value
        self.assertIsNone(
            self.tree.search(-4, strategy=SearchStrategy.FROM_BELOW)
        )
        # search above highest value
        self.assertEqual(
            9.,
            self.tree.search(10, strategy=SearchStrategy.FROM_BELOW).value
        )


if __name__ == "__main__":
    unittest.main()
