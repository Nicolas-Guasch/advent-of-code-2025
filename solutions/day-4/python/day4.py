from pathlib import Path
from itertools import product


class Grid:
    def __init__(self, data):
        self.data = data

    def __getitem__(self, pos):
        i, j = pos
        return self.data[i][j]

    def __setitem__(self, pos, value):
        i, j = pos
        self.data[i][j] = value

    def in_bounds(self, i, j):
        return 0 <= i < len(self.data) and 0 <= j < len(self.data[i])

    def positions(self):
        return product(range(len(self.data)), range(len(self.data[0])))

    def neighbors(self, i, j):
        return ((ii, jj) for ii, jj in product(range(i-1, i+2), range(j-1, j+2)) if (self.in_bounds(ii, jj)) and not (ii == i and jj == j))


def count_adjacent_rolls(i, j, diagram: Grid):
    return sum(1 for (ii, jj) in diagram.neighbors(i, j) if diagram[ii, jj] == '@')


def accessible(i, j, diagram):
    return count_adjacent_rolls(i, j, diagram) < 4


def just_unlocked(i, j, diagram):
    return count_adjacent_rolls(i, j, diagram) == 3


def part1(diagram):
    diagram = Grid(diagram)
    return f"There are {sum(1 for i, j in diagram.positions() if diagram[i,j] == '@' and accessible(i, j, diagram))} rolls of paper accessible by forklift."


def part2(diagram):
    diagram = Grid(diagram)
    queue = [(i, j) for i, j in diagram.positions()
             if diagram[i, j] == '@' and accessible(i, j, diagram)]

    accessibleRolls = 0
    while (len(queue)):
        accessibleRolls += 1
        (i, j) = queue.pop(0)
        diagram[i, j] = '.'
        queue += [(ii, jj)
                  for ii, jj in diagram.neighbors(i, j) if diagram[ii, jj] == '@' and just_unlocked(ii, jj, diagram)]

    return f'There are {accessibleRolls} rolls of paper accessible by forklift with removal.'


file_name = "day4.in"

diagram = [list(s) for s in Path(file_name).read_text().splitlines()]
print(part1(diagram))
print(part2(diagram))
