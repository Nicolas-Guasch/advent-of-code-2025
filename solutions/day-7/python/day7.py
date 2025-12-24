from collections import defaultdict
from dataclasses import dataclass, field
from functools import cache
from itertools import product
from pathlib import Path
from typing import Iterator


@dataclass
class Grid:
    data: list[str]

    def __post_init__(self):
        self.height = len(self.data)
        self.width = len(self.data[0]) if self.data else 0

    def __getitem__(self, pos: tuple[int, int]) -> str:
        return self.data[pos[0]][pos[1]]

    def in_bounds(self, i: int, j: int) -> bool:
        return 0 <= i < self.height and 0 <= j < self.width

    def positions(self) -> Iterator[tuple[int, int]]:
        return product(range(len(self.data)), range(len(self.data[0])))

    def find(self, char: str) -> tuple[int, int]:
        return next((i, j) for i, j in self.positions() if self[i, j] == char)


@dataclass
class Graph:
    edges: dict[int, list[int]] = field(
        default_factory=lambda: defaultdict(list))
    diagram_skyline: dict[int] = field(
        default_factory=lambda: defaultdict(lambda: -1))

    def add_node(self, j: int):
        u = len(self.edges)
        self.diagram_skyline[j] = u
        self.edges[u] = [adjacent for adjacent in (
            self.diagram_skyline[j-1], self.diagram_skyline[j+1]) if adjacent != -1]

    def dfs(self, start_column: int) -> set[int]:
        visited = set[int]()
        root = self.diagram_skyline[start_column]

        def _dfs(j: int):
            visited.add(j)
            for adjacent in self.edges[j]:
                if adjacent not in visited:
                    _dfs(adjacent)
        _dfs(root)
        return visited

    def dag_sum(self, start_column: int) -> int:
        root = self.diagram_skyline[start_column]

        @cache
        def compute(j: int):
            return 2 - len(self.edges[j]) + sum(compute(adj) for adj in self.edges[j])
        return compute(root)


def parse(input_data: str) -> tuple[Graph, int]:
    grid = Grid(input_data.splitlines())
    graph = Graph()
    for i, j in reversed(list(grid.positions())):
        if grid[i, j] == '^':
            graph.add_node(j)
    _, start_column = grid.find('S')
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
