from collections import deque

import numpy as np


def is_oob(i: int, j: int, n_rows: int, n_cols: int) -> bool:
    return i < 0 or j < 0 or i >= n_rows or j >= n_cols


def day10a() -> None:
    rows = []
    with open("../inputs/day10.txt", "r") as file:
        for line in file:
            rows.append(line.strip())

    trailheads = []
    n_rows, n_cols = len(rows), len(rows[0])
    grid = np.zeros((len(rows), len(rows[0])), dtype=np.int32)
    for i, row in enumerate(rows):
        for j, char in enumerate(row):
            grid[i, j] = int(char)
            if grid[i, j] == 0:
                trailheads.append((i, j))

    trails = 0
    for trailhead in trailheads:
        queue = deque()
        visited = set()
        queue.append(trailhead)
        visited.add(trailhead)
        while len(queue):
            i, j = queue.popleft()
            if grid[i, j] == 9:
                trails += 1

            for neighbor_offset in ((0, 1), (1, 0), (-1, 0), (0, -1)):
                ni, nj = i + neighbor_offset[0], j + neighbor_offset[1]
                if (
                    not is_oob(ni, nj, n_rows, n_cols)
                    and grid[ni, nj] == grid[i, j] + 1
                ):
                    if (ni, nj) not in visited:
                        visited.add((ni, nj))
                        queue.append((ni, nj))

    print(trails)


def day10b() -> None:
    rows = []
    with open("../inputs/day10.txt", "r") as file:
        for line in file:
            rows.append(line.strip())

    trailheads = []
    n_rows, n_cols = len(rows), len(rows[0])
    grid = np.zeros((len(rows), len(rows[0])), dtype=np.int32)
    for i, row in enumerate(rows):
        for j, char in enumerate(row):
            grid[i, j] = int(char)
            if grid[i, j] == 0:
                trailheads.append((i, j))

    trails = 0
    for trailhead in trailheads:
        queue = deque()
        queue.append(trailhead)
        while len(queue):
            i, j = queue.popleft()
            if grid[i, j] == 9:
                trails += 1

            for neighbor_offset in ((0, 1), (1, 0), (-1, 0), (0, -1)):
                ni, nj = i + neighbor_offset[0], j + neighbor_offset[1]
                if (
                    not is_oob(ni, nj, n_rows, n_cols)
                    and grid[ni, nj] == grid[i, j] + 1
                ):
                    queue.append((ni, nj))

    print(trails)


day10b()
