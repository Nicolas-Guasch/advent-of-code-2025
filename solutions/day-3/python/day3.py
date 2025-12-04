from itertools import accumulate
from functools import reduce
from pathlib import Path


def part1(lines):
    def max_joltage(bank):
        joltages = [int(d) for d in bank]
        max_prefixes = list(accumulate(joltages, max))
        return max(p * 10 + d for p, d in zip(max_prefixes, joltages[1:]))

    return f"The total output joltage with two batteries per bank is: {sum(max_joltage(bank) for bank in lines)}"


def max_joltage_position(values, pos, window):
    return max(range(pos, pos + window), key=values.__getitem__)


def greedy(battery_bank, digits=12):
    joltages = [int(d) for d in battery_bank]

    def pick_next_digit(state, remaining_digits):
        pos, current_joltage = state
        window = len(joltages) - pos - remaining_digits + 1
        next_digit = max_joltage_position(joltages, pos, window)
        return (next_digit + 1, current_joltage * 10 + joltages[next_digit])

    return reduce(pick_next_digit, range(digits, 0, -1), (0, 0))[1]


def part2(banks):
    return f"The total output joltage with twelve batteries per bank is: {sum(greedy(bank) for bank in banks)}"


file_name = "day3.in"

lines = Path(file_name).read_text().splitlines()
print(part1(lines))
print(part2(lines))
