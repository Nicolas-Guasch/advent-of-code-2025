from pathlib import Path
DIALLEN = 100


def parse_rotation(s):
    return (-1 if s[0] == "L" else 1) * int(s[1:])


def part1(lines):
    position, password = 50, 0
    for rotation in map(parse_rotation, lines):
        position = (position + rotation) % DIALLEN
        password += (position == 0)
    return f"Door password: {password}"


def part2(lines):
    position, password = 50, 0
    for rotation in map(parse_rotation, lines):
        password += (position != 0) - int((position + rotation) /
                                          DIALLEN) if position + rotation <= 0 else (position + rotation) // DIALLEN
        position = (position + rotation) % DIALLEN
    return f"Door password using password method 0x434C49434B: {password}"


file_name = "day1.in"

lines = Path(file_name).read_text().splitlines()
print(part1(lines))
print(part2(lines))
