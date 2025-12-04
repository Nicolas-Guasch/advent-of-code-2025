from pathlib import Path


def parse_intervals(line):
    return [tuple(map(int, interval.split("-"))) for interval in line.split(",")]


MAX_ID = 10000017929
MAX_DIGITS = 11
MAX_PATTERN = 99999 + 1


def included(pid, intervals):
    return any(a <= pid <= b for a, b in intervals)


def repeat_digits(pid, times):
    return int(str(pid) * times)


def part1(line):
    intervals = parse_intervals(line)
    result = sum(
        invalid_id for i in range(MAX_PATTERN)
        if included(invalid_id := repeat_digits(i, 2), intervals)
    )
    return f"Sum of invalid IDs: {result}"


def part2(line):
    intervals = parse_intervals(line)
    invalid_ids = {
        repeat_digits(i, t)
        for t in range(2, MAX_DIGITS)
        for i in range(MAX_PATTERN)
        if included(repeat_digits(i, t), intervals)
    }
    return f"Sum of invalid IDs: {sum(invalid_ids)}"


file_name = "day2.in"

line = Path(file_name).read_text().split()[0]
print(part1(line))
print(part2(line))
