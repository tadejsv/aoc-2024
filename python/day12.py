from collections import deque

import numpy as np


def is_oob(i: int, j: int, grid: list[str]) -> bool:
    return i < 0 or j < 0 or i >= len(grid) or j >= len(grid[0])


def explore(i: int, j: int, grid: list[str], visited: np.ndarray) -> int:
    area, perimeter = 0, 0

    queue = deque()
    queue.append((i, j))
    visited[i, j] = True

    while len(queue) > 0:
        i, j = queue.pop()
        area += 1

        for di, dj in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            ni, nj = i + di, j + dj
            if is_oob(ni, nj, grid) or grid[i][j] != grid[ni][nj]:
                perimeter += 1
                continue
            if not visited[ni, nj]:
                visited[ni, nj] = 1
                queue.append((ni, nj))

    return area * perimeter


surround_offsets = (
    (1, 0),
    (1, 1),
    (1, -1),
    (0, 1),
    (0, -1),
    (-1, 0),
    (-1, 1),
    (-1, -1),
)

corners = ((2, 0), (0, 2), (2, 2), (0, 0))


def explore_b(i: int, j: int, grid: list[str], visited: np.ndarray) -> int:
    area, sides = 0, 0

    queue = deque()
    queue.append((i, j))
    visited[i, j] = True

    while len(queue) > 0:
        i, j = queue.pop()
        area += 1

        surround = np.zeros((3, 3), dtype=bool)
        for di, dj in surround_offsets:
            ni, nj = i + di, j + dj
            if not (is_oob(ni, nj, grid) or grid[i][j] != grid[ni][nj]):
                surround[di + 1, dj + 1] = 1
                if not visited[ni, nj] and (di == 0 or dj == 0):
                    visited[ni, nj] = 1
                    queue.append((ni, nj))

        for corner in corners:
            # outer corner
            if (
                surround[1, corner[1]]
                and surround[corner[0], 1]
                and (not surround[corner[0], corner[1]])
            ):
                sides += 1

            # inner corner
            if not surround[1, corner[1]] and not surround[corner[0], 1]:
                sides += 1

    return area * sides


def day12a() -> None:
    grid = []
    with open("../inputs/day12.txt", "r") as file:
        for line in file:
            grid.append(line.strip())

    visited = np.zeros((len(grid), len(grid[0])), dtype=bool)

    cost = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if not visited[i, j]:
                cost += explore(i, j, grid, visited)

    print(cost)


def day12b() -> None:
    grid = []
    with open("../inputs/day12.txt", "r") as file:
        for line in file:
            grid.append(line.strip())

    visited = np.zeros((len(grid), len(grid[0])), dtype=bool)

    cost = 0
    for i in range(len(grid)):
        for j in range(len(grid[0])):
            if not visited[i, j]:
                cost += explore_b(i, j, grid, visited)

    print(cost)


day12b()
