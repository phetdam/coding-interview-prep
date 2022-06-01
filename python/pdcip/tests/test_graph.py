"""Unit tests for the Graph and related classes.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

import unittest

from pdcip.graph import Edge, Graph, Vertex


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


class GraphTestCase(unittest.TestCase):
    """Tests that the Graph class functions as expected.

    The vertices, edges, and graph fixtures represent the graph

         5 < + + > 4.1
         + +        ^
         +   + + +   +
         V        V  +
        7.6 + + > 9 <

    This graph is unweighted and directed, with no duplicate edges.
    """

    vertices = [Vertex(5), Vertex(4.1), Vertex(7.6), Vertex(9)]
    edges = [
        Edge(vertices[0], vertices[2]),
        Edge(vertices[0], vertices[1]),
        Edge(vertices[1], vertices[0]),
        Edge(vertices[0], vertices[3]),
        Edge(vertices[2], vertices[3]),
        Edge(vertices[1], vertices[3]),
        Edge(vertices[3], vertices[1])
    ]
    graph = Graph(vertices, edges)

    def test_graph_properties(self):
        """Test that the Graph properties work as expected."""
        self.assertEqual(self.vertices, self.graph.vertices)
        self.assertEqual(self.edges, self.graph.edges)
        self.assertEqual(len(self.vertices), self.graph.n_vertices)
        self.assertEqual(len(self.edges), self.graph.n_edges)
