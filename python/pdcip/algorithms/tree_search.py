"""Implementation of standard tree search methods.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

from copy import deepcopy
from enum import Enum
from typing import List

# pylint: disable=relative-beyond-top-level
from ..data_structures.binary_tree import BinaryTree


class AlgoLoopType(Enum):
    """Enum type indicating if algorithm runs recursively or iteratively."""

    RECURSIVE = 0
    ITERATIVE = 1


# allow this to use a generic tree later on
def dfs(
    tree: BinaryTree,
    method: AlgoLoopType = AlgoLoopType.RECURSIVE
) -> List[float]:
    """Depth-first search.

    Parameters
    ----------
    tree : BinaryTree
        Tree we want to traverse using depth-first search
    method : AlgoLoopType, default=AlgoLoopType.RECURSIVE
        Whether to run recursively or iteratively

    Returns
    -------
    values : list
        List of float values from the BinaryTree
    """
    if not tree:
        return []
    if method == AlgoLoopType.RECURSIVE:
        if tree.left:
            left_values = dfs(tree.left)
        else:
            left_values = []
        if tree.right:
            right_values = dfs(tree.right)
        else:
            right_values = []
        return left_values + right_values + [tree.value]
    # so we can mark nodes as visited
    tree = deepcopy(tree)
    stack = [tree]
    values = []
    while stack:
        cur = stack[-1]
        if cur.left and cur.left.value is not None:
            stack.append(cur.left)
            continue
        if cur.right and cur.right.value is not None:
            stack.append(cur.right)
            continue
        values.append(cur.value)
        # prevent infinite looping
        cur.value = None
        stack.pop()
    return values


def bfs(tree: BinaryTree) -> List[float]:
    """Breadth-first search.

    Parameters
    ----------
    tree : BinaryTree
        Tree we want to traverse using breadth-first search

    Returns
    -------
    values : list
        List of float values from the BinaryTree
    """
    if not tree or tree.value is None:
        return []
    queue = [tree]
    values = []
    while queue:
        cur = queue.pop(0)
        if cur.left:
            queue.append(cur.left)
        if cur.right:
            queue.append(cur.right)
        values.append(cur.value)
    return values
