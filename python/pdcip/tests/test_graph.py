"""Unit tests for the Graph and related classes.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

import unittest

from pdcip.graph import Edge, Vertex


class EdgeTestCase(unittest.TestCase):
    """Tests that the Edge class functions as expected."""

    def test_edge_eq(self):
        """Test that the edge __eq__ is generated correctly.

        Indirectly tests Vertex equality too.
        """
        vert_a, vert_b = Vertex(1.), Vertex(5.)
        self.assertEqual(Edge(vert_a, vert_b), Edge(vert_a, vert_b))
        self.assertNotEqual(Edge(vert_a, vert_b), Edge(vert_b, vert_a))
        self.assertNotEqual(
            Edge(vert_a, vert_b, weight=4.), Edge(vert_a, vert_b)
        )
