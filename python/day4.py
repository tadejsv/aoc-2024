def find_xmas(x: int, y: int, dx: int, dy: int, lines: list[list[str]]) -> bool:
    word = "XMAS"

    x_max, y_max = x + 3 * dx, y + 3 * dy
    if x_max >= len(lines[0]) or x_max < 0:
        return False
    if y_max >= len(lines) or y_max < 0:
        return False

    for i in range(4):
        if lines[y + i * dy][x + i * dx] != word[i]:
            return False

    return True


def find_mas(
    x: int, y: int, dx1: int, dy1: int, dx2: int, dy2: int, lines: list[list[str]]
) -> bool:
    if x + 1 >= len(lines[0]) or x - 1 < 0:
        return False
    if y + 1 >= len(lines) or y - 1 < 0:
        return False

    if lines[y][x] == "A":
        if lines[y + dy1][x + dx1] == "M" and lines[y + dy2][x + dx2] == "M":
            if lines[y - dy1][x - dx1] == "S" and lines[y - dy2][x - dx2] == "S":
                return True

    return False


def day4a() -> None:
    lines = []
    with open("../inputs/day4.txt", "r") as file:
        for line in file:
            lines.append(line.strip())

    dirs = [
        (0, 1),
        (0, -1),
        (1, 1),
        (1, 0),
        (1, -1),
        (-1, 1),
        (-1, 0),
        (-1, -1),
    ]

    num_xmas = 0
    for y in range(len(lines)):
        for x in range(len(lines[0])):
            for dir in dirs:
                if find_xmas(x, y, dir[0], dir[1], lines):
                    num_xmas += 1

    print(num_xmas)


def day4b() -> None:
    lines = []
    with open("../inputs/day4.txt", "r") as file:
        for line in file:
            lines.append(line.strip())

    dirs = [
        (1, 1, -1, 1),
        (-1, 1, -1, -1),
        (-1, -1, 1, -1),
        (1, -1, 1, 1),
    ]

    num_mas = 0
    for y in range(len(lines)):
        for x in range(len(lines[0])):
            for dir in dirs:
                if find_mas(x, y, dir[0], dir[1], dir[2], dir[3], lines):
                    num_mas += 1

    print(num_mas)


day4b()
