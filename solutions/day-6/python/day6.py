from math import prod
from pathlib import Path
from itertools import groupby


class Problem:
    def __init__(self, op, operands):
        self.op = op
        self.operands = operands

    def evaluate(self):
        return sum(self.operands) if self.op == '+' else prod(self.operands)


def partition_problems(input_data):
    lines = [line.split() for line in input_data.splitlines()]
    operators = lines[-1]
    operand_rows = lines[:-1]
    operand_columns = zip(*operand_rows)
    return [Problem(op, [int(operand) for operand in column]) for op, column in zip(operators, operand_columns)]


def part1(input_data):
    problems = partition_problems(input_data)
    return f'The grand total for this worksheet is {sum(problem.evaluate() for problem in problems)}.'


def partition_problems_vertical(input_data):
    lines = input_data.splitlines()
    columns = list(zip(*lines))
    problem_data = [list(group) for is_whitespace, group in groupby(columns, lambda col: all(
        c.isspace() for c in col)) if not is_whitespace]
    return [Problem(data[0][-1], [int(''.join(column[:-1])) for column in data]) for data in problem_data]


def part2(input_data):
    return f'The grand total for this worksheet with column evaluation is {sum(problem.evaluate() for problem in partition_problems_vertical(input_data))}.'


input_data = Path('day6.in').read_text()
# print(list(problem.op for problem in partition_problems(input_data)))
print(part1(input_data))
print(part2(input_data))
