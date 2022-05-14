"""Implementation of standard tree search methods.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

from copy import deepcopy
from typing import List

# pylint: disable=bad-continuation
from pdcip.algorithms.core import AlgoLoopType
from pdcip.structures.tree import Tree


def dfs(
    tree: Tree,
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
                child_values.extend(dfs(child))
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


def bfs(tree: Tree) -> List[float]:
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
