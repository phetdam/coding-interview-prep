"""General tree implementations.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

# pylint: disable=bad-continuation

from dataclasses import dataclass, field
from typing import List, Union

from pdcip.algorithms.search.core import SearchStrategy


@dataclass(order=True)
class Tree:
    """General tree implementation using adjacency lists."""

    value: Union[float, None] = None
    # don't include in repr string; repr can be very long
    children: Union[List["Tree"], None] = field(default=None, repr=False)

    @property
    def n_children(self) -> int:
        """Return number of direct children in tree node."""
        return len(self.children)


@dataclass(init=False)
class BinaryTree(Tree):
    """Simple binary tree implementation."""

    def __init__(
        self,
        value: Union[float, None] = None,
        left: Union["BinaryTree", None] = None,
        right: Union["BinaryTree", None] = None
    ):
        super().__init__(value, [left, right])

    @property
    def left(self) -> "BinaryTree":
        """Return left child of the BinaryTree."""
        return self.children[0]

    @left.setter
    def left(self, new_left: "BinaryTree"):
        self.children[0] = new_left

    @property
    def right(self) -> "BinaryTree":
        """Return right child of the BinaryTree."""
        return self.children[1]

    @right.setter
    def right(self, new_right: "BinaryTree"):
        self.children[1] = new_right

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

    @staticmethod
    def _closest(
        target: float,
        closest: Union["BinaryTree", None],
        candidate: "BinaryTree",
        strategy: SearchStrategy = SearchStrategy.EXACT
    ) -> Union["BinaryTree", None]:
        """Return tree node has closest value to the target.

        None may be returned if neither closest nor candidate are suitable.

        Parameters
        ----------
        target : float
            Target value to search for
        closest : BinaryTree or None
            Current tree node with closest value (can be None)
        candidate : BinaryTree
            Candidate tree node we want to compare in "closeness" to closest
        strategy : SearchStrategy, default=SearchStrategy.EXACT
            Search strategy, which affects the the criteria of "closeness"
        """
        if target == candidate.value or closest and target == closest.value:
            return closest
        if strategy == SearchStrategy.FROM_BELOW:
            # if candidate is invalid, use valid closest
            if target - candidate.value < 0:
                return closest
            # if candidate is valid and closest is None, automatically wins
            if not closest:
                return candidate
            # else choose better amongst closest and target
            return candidate if candidate >= closest else closest
        elif strategy == SearchStrategy.FROM_ABOVE:
            if target - candidate.value > 0:
                return closest
            if not closest:
                return candidate
            return candidate if candidate <= closest else closest
        # if candidate is invalid or cannot beat closest, keep using closest
        return closest

    def search(
        self,
        value: float,
        strategy: SearchStrategy = SearchStrategy.EXACT,
        _closest: float = None
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
        # update closest value to target value, by search strategy, using the
        # new value we are testing (self.value). can be None if self.value
        # is not closest in the sense defined by the search strategy.
        _closest = self._closest(
            value,
            _closest,
            self,
            strategy=strategy
        )
        # for nearest search, need to track the "closest" guess
        if self.value is None:
            return None
        if value == self.value:
            return self
        if value < self.value:
            if not self.left:
                if strategy == SearchStrategy.FROM_ABOVE:
                    return self
                # note that _closest may also be None in this case
                return _closest
            return self.left.search(value, strategy=strategy, _closest=_closest)
        if not self.right:
            if strategy == SearchStrategy.FROM_BELOW:
                return self
            # _closest may also be None in this case
            return _closest
        return self.right.search(value, strategy=strategy, _closest=_closest)

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
