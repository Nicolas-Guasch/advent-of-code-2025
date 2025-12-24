from collections import defaultdict
from dataclasses import dataclass
from itertools import product
from pathlib import Path


@dataclass
class Grid:
    data: list[str]

    def in_bounds(self, i: int, j: int) -> bool:
        return 0 <= i < len(self.data) and 0 <= j < len(self.data[i])

    def positions(self) -> list[tuple[int, int]]:
        return product(range(len(self.data)), range(len(self.data[0])))

    def neighbors(self, i: int, j: int) -> list[tuple[int, int]]:
        return [(ii, jj) for ii, jj in product(range(i-1, i+2), range(j-1, j+2)) if self.in_bounds(ii, jj) and (ii, jj) != (i, j)]


class Graph:
    edges: dict[int, list[int]]
    width: int
    diagram_skyline: dict[int]

    def __init__(self, width: int):
        self.width = width
        self.diagram_skyline = defaultdict(lambda: -1)
        self.edges = defaultdict(list)

    def add_node(self, j: int):
        u = len(self.edges)
        self.diagram_skyline[j] = u
        self.edges[u] = [adjacent for adjacent in [
            self.diagram_skyline[j-1], self.diagram_skyline[j+1]] if adjacent != -1]

    def dfs(self, start_column: int) -> set[int]:
        visited = set[int]()
        root = self.diagram_skyline[start_column]
        self._dfs(root, visited)
        return visited

    def _dfs(self, j: int, visited: set[int]):
        visited.add(j)
        for adjacent in self.edges[j]:
            if adjacent not in visited:
                self._dfs(adjacent, visited)

    def dag_sum(self, start_column: int) -> int:
        sub_tree_size = dict[int, int]()
        root = self.diagram_skyline[start_column]
        self._dag_sum(root, sub_tree_size)
        return sub_tree_size[root]

    def _dag_sum(self, j: int, sub_tree_size: dict[int, int]):
        sub_tree_size[j] = 2 - len(self.edges[j])
        for adjacent in self.edges[j]:
            if adjacent not in sub_tree_size:
                self._dag_sum(adjacent, sub_tree_size)
            sub_tree_size[j] += sub_tree_size[adjacent]


def parse(input_data: str) -> tuple[Graph, int]:
    grid = Grid(input_data.splitlines())
    graph = Graph(len(grid.data[0]))
    for i, j in reversed(list(grid.positions())):
        if grid.data[i][j] == '^':
            graph.add_node(j)
    start_column = next(j for i, j in grid.positions()
                        if grid.data[i][j] == 'S')
    return graph, start_column


def part1(input_data: str) -> str:
    graph, start_column = parse(input_data)
    return f"The beam will be split {len(graph.dfs(start_column))} times in the manifold diagram."


def part2(input_data: str) -> str:
    graph, start_column = parse(input_data)
    return f"The total number of nodes in the DAG is {graph.dag_sum(start_column)}."


def main() -> None:
    input_data = Path("day7.in").read_text()
    print(part1(input_data))
    print(part2(input_data))


if __name__ == "__main__":
    main()
