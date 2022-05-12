"""Unit tests for tree implementations.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

import unittest

# pylint: disable=missing-function-docstring
from pdcip.data_structures.tree import BinaryTree, SearchStrategy


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

    # def test_search_below(self):
    #     # search for middle value
    #     self.assertEqual(
    #         4.,
    #         self.tree.search(5, strategy=SearchStrategy.FROM_BELOW).value
    #     )
    #     # search below lowest value
    #     self.assertIsNone(
    #         self.tree.search(-4, strategy=SearchStrategy.FROM_BELOW)
    #     )
    #     # search above highest value
    #     self.assertEqual(
    #         9.,
    #         self.tree.search(10, strategy=SearchStrategy.FROM_BELOW).value
    #     )


if __name__ == "__main__":
    unittest.main()
