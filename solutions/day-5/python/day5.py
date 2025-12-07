from pathlib import Path
from bisect import bisect_left, bisect_right


def parse_input(input_data):
    ranges, ids = input_data.split('\n\n')
    ranges = [(int(l), int(r))
              for irange in ranges.splitlines()
              for l, r in [irange.split('-')]]
    ids = [int(iid) for iid in ids.splitlines()]
    ranges.sort()
    ids.sort()
    return ranges, ids


def merge_ranges(ranges):
    merged_ranges = []
    for l, r in ranges:
        if not merged_ranges or merged_ranges[-1][1]+1 < l:
            merged_ranges.append((l, r))
        else:
            merged_ranges[-1] = (merged_ranges[-1][0],
                                 max(merged_ranges[-1][1], r))
    return merged_ranges


def part1(input_data):
    ranges, ids = parse_input(input_data)

    fresh = sum(bisect_right(ids, r) - bisect_left(ids, l)
                for l, r in merge_ranges(ranges))
    return f'{fresh} of the available ingredient IDs are fresh'


def part2(input_data):
    ranges, _ = parse_input(input_data)

    fresh_count = sum(r - l + 1 for l, r in merge_ranges(ranges))

    return f'There are {fresh_count} fresh ingredient IDs'


file_name = "day5.in"

input_data = Path(file_name).read_text()
print(part1(input_data))
print(part2(input_data))
