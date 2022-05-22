"""General tree implementations.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

# pylint: disable=bad-continuation

from copy import deepcopy
from dataclasses import dataclass, field
from typing import List, Union

from pdcip.enums import AlgoLoopType, SearchStrategy


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

    @staticmethod
    def dfs(
        tree: "Tree",
        method: AlgoLoopType = AlgoLoopType.RECURSIVE
    ) -> List[float]:
        """Depth-first search.

        Parameters
        ----------
        tree : Tree
            Tree we want to traverse using depth-first search
        method : AlgoLoopType, default=AlgoLoopType.RECURSIVE
            Whether to run recursively or iteratively

        Returns
        -------
        values : list
            List of float values from the Tree
        """
        if not tree:
            return []
        if method == AlgoLoopType.RECURSIVE:
            child_values = []
            for child in tree.children:
                if child:
                    child_values.extend(Tree.dfs(child))
            return child_values + [tree.value]
        # so we can mark nodes as visited
        tree = deepcopy(tree)
        stack = [tree]
        values = []
        while stack:
            cur = stack[-1]
            # some Tree subclasses may encode missing children as being None
            for child in cur.children:
                if child and child.value is not None:
                    stack.append(child)
                    break
            # of cur != stack[-1], a new node was added, so continue
            if cur != stack[-1]:
                continue
            values.append(cur.value)
            # prevent infinite looping
            cur.value = None
            stack.pop()
        return values

    @staticmethod
    def bfs(tree: "Tree") -> List[float]:
        """Breadth-first search.

        Parameters
        ----------
        tree : Tree
            Tree we want to traverse using breadth-first search

        Returns
        -------
        values : list
            List of float values from the Tree
        """
        if not tree or tree.value is None:
            return []
        queue = [tree]
        values = []
        while queue:
            cur = queue.pop(0)
            # children can be None for trees like BinaryTree
            queue.extend([child for child in cur.children if child])
            values.append(cur.value)
        return values


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
