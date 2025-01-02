import math
from collections import defaultdict


def cell2index(y: int, x: int, n_cols: int) -> int:
    return y * n_cols + x


def is_oob(x: int, y: int, n_cols: int, n_rows: int) -> bool:
    return x < 0 or x >= n_cols or y < 0 or y >= n_rows


def add_antinode(antinodes: set[int], y: int, x: int, n_cols: int, n_rows: int) -> None:
    if not is_oob(x, y, n_cols, n_rows):
        antinodes.add(cell2index(y, x, n_cols))


def add_antinodes_line(
    antinodes: set[int], y: int, x: int, dy: int, dx: int, n_cols: int, n_rows: int
) -> None:
    i = 0
    while not is_oob(x + i * dx, y + i * dy, n_cols, n_rows):
        antinodes.add(cell2index(y + i * dy, x + i * dx, n_cols))
        i += 1

    i = 0
    while not is_oob(x + i * dx, y + i * dy, n_cols, n_rows):
        antinodes.add(cell2index(y + i * dy, x + i * dx, n_cols))
        i -= 1


def day8a() -> None:
    rows = []
    with open("../inputs/day8.txt", "r") as file:
        for line in file.readlines():
            rows.append(line.strip())

    antennas = defaultdict(list)
    for i, row in enumerate(rows):
        for j, chr in enumerate(row):
            if chr != ".":
                antennas[chr].append((i, j))

    antinodes = set()
    for ants in antennas.values():
        for i in range(len(ants)):
            for j in range(i + 1, len(ants)):
                dy = ants[i][0] - ants[j][0]
                dx = ants[i][1] - ants[j][1]
                add_antinode(
                    antinodes, ants[i][0] + dy, ants[i][1] + dx, len(rows), len(rows[0])
                )
                add_antinode(
                    antinodes, ants[j][0] - dy, ants[j][1] - dx, len(rows), len(rows[0])
                )
    print(len(antinodes))


def day8b() -> None:
    rows = []
    with open("../inputs/day8.txt", "r") as file:
        for line in file.readlines():
            rows.append(line.strip())

    antennas = defaultdict(list)
    for i, row in enumerate(rows):
        for j, chr in enumerate(row):
            if chr != ".":
                antennas[chr].append((i, j))

    antinodes = set()
    for ants in antennas.values():
        for i in range(len(ants)):
            for j in range(i + 1, len(ants)):
                dy = ants[i][0] - ants[j][0]
                dx = ants[i][1] - ants[j][1]
                gcd = math.gcd(dy, dx)
                dy /= gcd
                dx /= gcd

                add_antinodes_line(
                    antinodes, ants[i][0], ants[i][1], dy, dx, len(rows), len(rows[0])
                )

    print(len(antinodes))


day8b()
