"""Core code used by the pdcip.algorithms subpackage.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

from enum import Enum


class AlgoLoopType(Enum):
    """Enum type indicating if algorithm runs recursively or iteratively."""

    RECURSIVE = 0
    ITERATIVE = 1
