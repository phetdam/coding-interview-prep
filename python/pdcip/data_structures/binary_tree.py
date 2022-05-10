"""Binary tree implementation.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

from enum import Enum
from dataclasses import dataclass
from typing import List, Union


class SearchStrategy(Enum):
    """Enum dictating how search matches should be conducted."""

    # match exactly for success
    EXACT = 0
    # match nearest upper bound for success
    FROM_ABOVE = 1
    # match nearest lower bound for success
    FROM_BELOW = 2


@dataclass
class BinaryTree:
    """Simple binary tree implementation."""

    value: Union[float, None] = None
    left: Union["BinaryTree", None] = None
    right: Union["BinaryTree", None] = None

    def insert(self, value: float) -> float:
        """Insert a value into the binary tree.

        If value already exists in the tree, does nothing.

        Parameters
        ----------
        value : float
            Value to insert into the tree

        Returns
        -------
        float
            Value inserted into the tree
        """
        if self.value is None:
            self.value = value
            return self.value
        if value == self.value:
            return value
        if value < self.value:
            if not self.left:
                self.left = BinaryTree(value=value)
                return value
            return self.left.insert(value)
        if not self.right:
            self.right = BinaryTree(value=value)
            return value
        return self.right.insert(value)

    def search(
        self,
        value: float,
        strategy: SearchStrategy = SearchStrategy.EXACT
    ) -> Union["BinaryTree", None]:
        """Search for a value in the tree.

        If no match is found, return None. Can return approximate matches by
        passing a SearchStrategy to the strategy parameter.

        Parameters
        ----------
        value : float
            Value to search for in the tree
        strategy : SearchStrategy, default=SearchStrategy.EXACT
            Search matching strategy, defaults to exact match. Using matching
            with SearchStrategy.FROM_ABOVE attempts to return the nearest
            upper bound in the tree, while SearchStrategy.FROM_BELOW attempts
            to return the nearest lower bound.

        Returns
        -------
        BinaryTree or None
            Node containing the matched value, else None if no match
        """
        # for nearest search, need to track the "closest" guess
        if self.value is None:
            return None
        if value == self.value:
            return self
        if value < self.value:
            if not self.left:
                if strategy == SearchStrategy.FROM_ABOVE:
                    return self
                return None
            return self.left.search(value, strategy=strategy)
        if not self.right:
            if strategy == SearchStrategy.FROM_BELOW:
                return self
            return None
        return self.right.search(value, strategy=strategy)

    def sorted_values(self) -> List[float]:
        """Return tree values sorted in a list."""
        if self.value is None:
            return []
        if self.left:
            left_values = self.left.sorted_values()
        else:
            left_values = []
        if self.right:
            right_values = self.right.sorted_values()
        else:
            right_values = []
        return left_values + [self.value] + right_values
