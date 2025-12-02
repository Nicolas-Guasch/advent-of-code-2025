DIALLEN = 100
def part1(lines):
    position, password = 50, 0
    for rotation in lines:
        delta = (-1 if rotation[0] == "L" else 1) * int(rotation[1:])
        position = (position + delta) % DIALLEN
        if position == 0:
            password += 1
    return f"Door password: {password}"

def part2(lines):
    position, password = 50, 0
    for rotation in lines:
        delta = (-1 if rotation[0] == "L" else 1) * int(rotation[1:])
        password += (position != 0) - int((position + delta) / DIALLEN) if position + delta <= 0 else (position + delta) // DIALLEN
        position = (position + delta) % DIALLEN
    return f"Door password using password method 0x434C49434B: {password}"

fileName = "day1.in"

with open(fileName, "r") as input:
    lines = input.read().split()
    print(part1(lines))
    print(part2(lines))