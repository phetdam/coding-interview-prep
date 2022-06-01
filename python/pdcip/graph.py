"""General graph implementations.

.. codeauthor:: Derek Huang <djh458@stern.nyu.edu>
"""

# pylint: disable=bad-continuation

from dataclasses import dataclass
from typing import Iterable, List

# from pdcip.enums import SearchStrategy


@dataclass
class Vertex:
    """General graph vertex implementation."""

    value: float

    def __hash__(self) -> int:
        return id(self)


@dataclass(eq=True)
class Edge:
    """General directed graph edge implementation with optional weight.

    Edge direction is specified as start -> end. Two Edge objects are
    considered equal if their start, end, and weight all are equal.
    """

    start: Vertex
    end: Vertex
    weight: float = 1.

    def __post_init__(self):
        self.vertices = [self.start, self.end]

    def connects(
        self,
        vert_a: Vertex,
        vert_b: Vertex,
        directed: bool = True
    ) -> bool:
        """True if both vertices are connected by this edge.

        Parameters
        ----------
        directed : bool, default=True
            If True, then return True only if vert_a is the start vertex and
            vert_b is the end vertex to emulate edge in directed graph.
        """
        if directed:
            return vert_a == self.start and vert_b == self.end
        return vert_a in self.vertices and vert_b in self.vertices


@dataclass
class Graph:
    """General graph implementation.

    .. note::

       Does not support duplicated edges in the graph, i.e. edges that have
       identical start vertex, end vertex, and edge weight.

    Uses dicts to allow constant time checking of edge connection to emulate
    adjacency matrix lookup performance. Edge and vertex membership checking
    is also constant time because of this implementation detail.
    """

    def __init__(self, vertices: Iterable[Vertex], edges: Iterable[Edge]):
        self._vertex_map = {vertex: None for vertex in vertices}
        # note the two levels of mapping are there to support weights. if no
        # weights, {(edge.start, edge.end): None for edge in edges} is enough.
        self._edge_map = {}
        for edge in edges:
            self.add_edge(edge)

    @property
    def vertices(self) -> List[Vertex]:
        """Return list of vertices in the graph."""
        return [vertex for vertex in self._vertex_map]

    @property
    def edges(self) -> List[Edge]:
        """Return list of edges in the graph.

        .. note::

           Since the graph does not actually store Edge objects, unlike
           self.vertices, these Edge instances are all fresh instances.
        """
        return [
            Edge(start, end, weight=weight)
            for (start, end) in self._edge_map
            for weight in self._edge_map[(start, end)]
        ]

    @property
    def n_vertices(self) -> int:
        """Return number of vertices in graph."""
        return len(self._vertex_map)

    @property
    def n_edges(self) -> int:
        """Return number of edges in graph.

        .. note::

           This operation has time complexity linear in the number of edges.
        """
        return len(self.edges)

    def add_vertex(self, vertex: Vertex) -> None:
        """Adds a vertex to the graph.

        .. note::

           If this Vertex reference is already in the graph, it will be
           silently overwritten. No duplicate vertex will be added.
        """
        self._vertex_map[vertex] = None

    def add_vertices(self, vertices: Iterable[Vertex]) -> None:
        """Adds several vertices to the graph.

        See self.add_vertex for how duplicate vertices are handled.
        """
        for vertex in vertices:
            self.add_vertex(vertex)

    def add_edge(self, edge: Edge) -> None:
        """Adds an edge to the graph."""
        edge_tuple = (edge.start, edge.end)
        # note the two levels of mapping are there to support weights. if no
        # weights, (edge.start, edge.end) set of keys is enough.
        if edge_tuple not in self._edge_map:
            self._edge_map[edge_tuple] = {edge.weight: None}
        elif edge.weight not in self._edge_map[edge_tuple]:
            self._edge_map[edge_tuple][edge.weight] = None
        else:
            raise ValueError(
                f"Cannot add duplicate edge {edge} to {self.__class__.__name__}"
            )

    def add_edges(self, edges: Iterable[Edge]) -> None:
        """Adds several edges to the graph."""
        for edge in edges:
            self.add_edge(edge)

    def has_vertex(self, vertex: Vertex) -> bool:
        """True if vertex is in the graph."""
        return True if vertex in self._vertex_map else False

    def has_edge(self, edge: Edge) -> bool:
        """True if edge is in the graph."""
        edge_tuple = (edge.start, edge.end)
        if edge_tuple in self._edge_map:
            if edge.weight in self._edge_map[edge_tuple]:
                return True
        return False

    def connects(
        self,
        vert_a: Vertex,
        vert_b: Vertex,
        directed: bool = True
    ) -> bool:
        """True if the nodes are connected, False otherwise.

        See Edge.connects for parameter documentation.
        """
        # don't even need to look at _edge_map if not in _vertex_map
        if vert_a not in self._vertex_map or vert_b not in self._vertex_map:
            return False
        if (vert_a, vert_b) in self._edge_map:
            return True
        if not directed and (vert_b, vert_a) in self._edge_map:
            return True
        return False
