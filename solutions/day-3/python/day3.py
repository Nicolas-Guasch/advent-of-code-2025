def part1(lines):
    total_joltage = 0

    for battery_bank in lines:
        max_joltage = 0
        highest_battery = int(battery_bank[0])
        for battery in battery_bank[1:]:
            battery_joltage = int(battery)
            produced_joltage = highest_battery * 10 + battery_joltage
            max_joltage = max(max_joltage, produced_joltage)
            highest_battery = max(highest_battery, battery_joltage)
        total_joltage += max_joltage

    return f"The total output joltage with two batteries per bank is: {total_joltage}"


def max_joltage_position(battery_bank, pos, window):
    return max(range(pos, pos + window), key=lambda i: int(battery_bank[i]))


DIGITS = 12


def greedy(battery_bank):
    max_joltage = 0
    pos = 0
    for digits in range(DIGITS, 0, -1):
        window = len(battery_bank) - pos - digits + 1
        pos = max_joltage_position(battery_bank, pos, window)
        max_joltage = max_joltage * 10 + int(battery_bank[pos])
        pos += 1
    return max_joltage


def part2(banks):
    return f"The total output joltage with twelve batteries per bank is: {sum(greedy(bank) for bank in banks)}"


file_name = "day3.in"

with open(file_name, "r") as input_file:
    lines = input_file.read().splitlines()
    print(part1(lines))
    print(part2(lines))
