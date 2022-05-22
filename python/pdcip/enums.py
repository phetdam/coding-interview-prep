"""Enumerated types used throughout the project.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

from enum import Enum


class AlgoLoopType(Enum):
    """Enum type indicating if algorithm runs recursively or iteratively."""

    RECURSIVE = 0
    ITERATIVE = 1


class SearchStrategy(Enum):
    """Enum dictating how search matches should be conducted."""

    # match exactly for success
    EXACT = 0
    # match nearest upper bound for success
    FROM_ABOVE = 1
    # match nearest lower bound for success
    FROM_BELOW = 2
