def parse_intervals(line):
    return [tuple(map(int, interval.split("-"))) for interval in line.split(",")]

MAXID = 10000017929
MAXDIGITS = 11
MAXPATTERN = 99999 + 1

def included(id, intervals):
    return any(id >= interval[0] and id <= interval[1] for interval in intervals)

def repeat_digits(id, times):
    return int(str(id) * times)

def part1(lines):
    intervals = parse_intervals(line)
    invalid_sum = 0
    for i in range(MAXPATTERN):
        invalid_id = repeat_digits(i, 2)
        if included(invalid_id, intervals):
            invalid_sum += invalid_id
    return f"Sum of invalid IDs: {invalid_sum}"

def part2(lines):
    intervals = parse_intervals(line)
    invalid_ids = set()
    for t in range(2, MAXDIGITS):
        for i in range(MAXPATTERN):
            invalid_id = repeat_digits(i, t)
            if included(invalid_id, intervals):
                invalid_ids.add(invalid_id)
    return f"Sum of invalid IDs: {sum(invalid_ids)}"

fileName = "day2.in"

with open(fileName, "r") as input:
    line = input.read().split()[0]
    print(part1(line))
    print(part2(line))