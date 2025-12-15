from dataclasses import dataclass
from functools import reduce
from itertools import groupby
from operator import add, mul
from pathlib import Path
from typing import Callable, Literal

OPS: dict[str, Callable[[int, int], int]] = {'+': add, '*': mul}


@dataclass
class Problem:
    op: Literal['+', '*']
    operands: list[int]

    def evaluate(self) -> int:
        return reduce(OPS[self.op], self.operands)


def partition_problems(input_data: str) -> list[Problem]:
    *operand_rows, operators = [line.split()
                                for line in input_data.splitlines()]
    operand_columns = zip(*operand_rows)
    return [Problem(op, [int(operand) for operand in column]) for op, column in zip(operators, operand_columns)]


def part1(input_data: str) -> str:
    problems = partition_problems(input_data)
    return f'The grand total for this worksheet is {sum(problem.evaluate() for problem in problems)}.'


def partition_problems_vertical(input_data: str) -> list[Problem]:
    lines = input_data.splitlines()
    columns = list(zip(*lines))
    problem_groups = (list(group) for is_whitespace, group in groupby(columns, lambda col: all(
        c.isspace() for c in col)) if not is_whitespace)

    def parse_problem(column_group: list[tuple[str, ...]]) -> Problem:
        operator = column_group[0][-1]
        operands = [int(''.join(column[:-1])) for column in column_group]
        return Problem(operator, operands)

    return [parse_problem(group) for group in problem_groups]


def part2(input_data: str) -> str:
    return f'The grand total for this worksheet with column evaluation is {sum(problem.evaluate() for problem in partition_problems_vertical(input_data))}.'


input_data = Path('day6.in').read_text()
print(part1(input_data))
print(part2(input_data))
