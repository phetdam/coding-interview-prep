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

    def setUp(self):
        self.graph = Graph(self.vertices, self.edges)

    def test_graph_properties(self):
        """Test that the Graph properties work as expected."""
        self.assertEqual(self.vertices, self.graph.vertices)
        self.assertEqual(self.edges, self.graph.edges)
        self.assertEqual(len(self.vertices), self.graph.n_vertices)
        self.assertEqual(len(self.edges), self.graph.n_edges)

    def test_graph_add_vertices(self):
        """Test that Graph add_vertices works as expected.

        Indirectly tests add_vertex, which is wrapped.
        """
        # these are not duplicates even though we used these values before
        self.graph.add_vertices([Vertex(5), Vertex(4.1)])
        self.assertEqual(len(self.vertices) + 2, self.graph.n_vertices)

    def test_graph_add_edges(self):
        """Test that Graph add_edges works as expected.

        Indirectly tests add_edge, which is wrapped.
        """
        # add new edges with default weight
        self.graph.add_edges(
            [
                Edge(self.vertices[2], self.vertices[0]),
                Edge(self.vertices[2], self.vertices[1])
            ]
        )
        self.assertEqual(len(self.edges) + 2, self.graph.n_edges)
        # add duplicate edges with different weight (not counted as duplicate)
        self.graph.add_edges(
            [
                Edge(self.vertices[0], self.vertices[2], weight=7),
                Edge(self.vertices[0], self.vertices[2], weight=11.2),
                Edge(self.vertices[0], self.vertices[1], weight=5.)
            ]
        )
        self.assertEqual(len(self.edges) + 5, self.graph.n_edges)
        # cannot add an exactly new duplicate edge
        with self.assertRaises(ValueError):
            self.graph.add_edge(Edge(self.vertices[0], self.vertices[2]))
        with self.assertRaises(ValueError):
            self.graph.add_edge(
                Edge(self.vertices[0], self.vertices[2], weight=7)
            )
