"""General graph implementations.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

# pylint: disable=bad-continuation,missing-function-docstring

from dataclasses import dataclass
from typing import List, Union

import numpy as np

# from pdcip.enums import SearchStrategy


@dataclass
class GraphVertex:
    """General graph vertex implementation.

    node_index is useful for graphs that use an adjacency matrix to determine
    adjacency by providing a way to number the nodes. Starts from 0.
    """

    value: float
    node_index: Union[int, None] = None


@dataclass(init=False)
class GraphEdge:
    """General graph edge implementation for an adjacency list graph.

    Edge direction is specified as head_vertex -> tail_vertex.

    Parameters
    ----------
    bidirectional : bool, default=False
        True to represent a bidirectional edge between the vertices.
    """

    def __init__(self, head_vertex: GraphVertex, tail_vertex: GraphVertex):
        self.head_vertex = head_vertex
        self.tail_vertex = tail_vertex
        self.vertices = [self.head_vertex, self.tail_vertex]

    def connects(
        self,
        vert_a: GraphVertex,
        vert_b: GraphVertex,
        directed: bool = False
    ) -> bool:
        """True if both vertices are connected by this edge.

        Parameters
        ----------
        directed : bool, default=False
            If True, then return True only if vert_a is the head vertex and
            vert_b is the tail vertex to emulate edge in directed graph.
        """
        if directed:
            return vert_a == self.head_vertex and vert_b == self.tail_vertex
        return vert_a in self.vertices and vert_b in self.vertices


@dataclass
class GraphBase:
    """Base class for graph implementations."""

    vertices: List[GraphVertex]

    @property
    def n_vertices(self) -> int:
        """Return number of vertices in graph."""
        return len(self.vertices)

    def add_vertex(self, vert: GraphVertex) -> GraphVertex:
        """Append new vertex to the vertex list."""
        self.vertices.append(vert)
        return vert

    def add_edge(self, edge: GraphEdge) -> GraphEdge:
        """Append new edge to the edge list."""
        raise NotImplementedError

    def connects(
        self,
        vert_a: GraphVertex,
        vert_b: GraphVertex,
        directed: bool = True
    ) -> bool:
        """True if the nodes are connected, False otherwise.

        See GraphEdge.connects for parameter documentation.
        """
        raise NotImplementedError


@dataclass
class AdjacencyListGraph(GraphBase):
    """General graph implementation using adjacency lists."""

    edges: List[GraphEdge]

    @property
    def n_edges(self) -> int:
        """Return number of edges in graph."""
        return len(self.edges)

    def add_edge(self, edge: GraphEdge) -> GraphEdge:
        """Append new edge to the edge list."""
        self.edges.append(edge)
        return edge

    def connects(
        self,
        vert_a: GraphVertex,
        vert_b: GraphVertex,
        directed: bool = True
    ) -> bool:
        """True if the nodes are connected, False otherwise.

        See GraphEdge.connects for parameter documentation.
        """
        vertex_set = set(self.vertices)
        if vert_a not in vertex_set or vert_b not in vertex_set:
            return False
        for edge in self.edges:
            if edge.connects(vert_a, vert_b, directed=directed):
                return True
        return False


@dataclass(init=False)
class AdjacencyMatrixGraph(GraphBase):
    """General implementation using an adjacency matrix.

    All vertices must either have node_index set or have copy_with_indices=True
    to get a copy of the vertices with node_index set by list order.

    Adjacency list has nonnegative integer values, with values greater than
    one representing duplicate edges between two nodes.

    Parameters
    ----------
    copy_with_indices : bool, default=True
        True to copy the passed vertices, setting each's node_index value.
        False to not make a copy and overwrite each's node_index value.
    """

    def __init__(
        self,
        vertices: List[GraphVertex],
        copy_with_indices: bool = True
    ):
        if copy_with_indices:
            self.vertices = [
                GraphVertex(vert.value, node_index=i)
                for i, vert in enumerate(vertices)
            ]
        else:
            self.vertices = vertices
            for i, vert in enumerate(self.vertices):
                vert.node_index = i

    # pylint: disable=no-member

    def add_edge(self, edge: GraphEdge) -> GraphEdge:
        """Add new edge to the graph.

        The node_index properties of the vertices must be set already.

        Note the edge will not be physically stored.
        """
        self.adjacency_matrix[
            edge.head_vertex.node_index, edge.tail_vertex.node_index
        ] += 1
        return edge

    def connects(
        self,
        vert_a: GraphVertex,
        vert_b: GraphVertex,
        directed: bool = True
    ) -> bool:
        """True if the nodes are connected, False otherwise.

        The node_index properties of the vertices must be set already.

        See GraphEdge.connects for parameter documentation.
        """
        ab_connects = (
            self.adjacency_matrix[vert_a.node_index, vert_b.node_index] > 0
        )
        if directed:
            return ab_connects
        return (
            ab_connects or
            self.adjacenecy_matrix[vert_b.node_index, vert_a.node_index] > 0
        )

    # pylint: enable=no-member

    def __setattr__(self, name: str, value: object):
        """Custom __setattr__ to auto-update adjacency matrix with vertices."""
        if name == "adjacency_matrix":
            raise AttributeError("Cannot set adjacency_matrix manually")
        super().__setattr__(name, value)
        # by setting vertices first, we can use n_vertices
        if name == "vertices":
            super().__setattr__(
                "adjacency_matrix",
                np.zeros((self.n_vertices, self.n_vertices))
            )
