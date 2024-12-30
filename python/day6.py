from enum import Enum

import numpy as np
import numpy.typing as npt


class Direction(Enum):
    Up = 0
    Down = 1
    Right = 2
    Left = 3


turn = (Direction.Right, Direction.Left, Direction.Down, Direction.Up)


def read_board(fname: str) -> tuple[npt.NDArray, tuple[int, int], Direction]:
    with open(fname, "r") as file:
        lines = [line.strip() for line in file.readlines()]

    pos = (-1, -1)
    dir = Direction.Down
    board = np.zeros((len(lines), len(lines[0])), np.int8)
    for i, line in enumerate(lines):
        for j, char in enumerate(line):
            if char == "#":
                board[i, j] = 1
            if char in {"<", ">", "^", "v"}:
                pos = (i, j)
                match char:
                    case "<":
                        dir = Direction.Left
                    case ">":
                        dir = Direction.Right
                    case "^":
                        dir = Direction.Up
                    case "v":
                        dir = Direction.Down

    return board, pos, dir


def hash_pos(pos: tuple[int, int], n_cols: int) -> int:
    return pos[0] * n_cols + pos[1]


def hash_pos_dir(pos: tuple[int, int], dir: Direction, n_cols: int) -> int:
    return pos[0] * n_cols * 4 + pos[1] * 4 + dir.value


def next_pos(pos: tuple[int, int], dir: Direction) -> tuple[int, int]:
    match dir:
        case Direction.Up:
            return (pos[0] - 1, pos[1])
        case Direction.Down:
            return (pos[0] + 1, pos[1])
        case Direction.Left:
            return (pos[0], pos[1] - 1)
        case Direction.Right:
            return (pos[0], pos[1] + 1)


def is_oob(pos: tuple[int, int], board: npt.NDArray) -> bool:
    if pos[0] < 0 or pos[0] >= board.shape[0]:
        return True
    if pos[1] < 0 or pos[1] >= board.shape[1]:
        return True

    return False


def check_loop(pos: tuple[int, int], dir: Direction, board: npt.NDArray) -> int:
    visited = set()
    while True:
        hpd = hash_pos_dir(pos, dir, board.shape[1])
        if hpd in visited:
            return 1
        visited.add(hpd)

        candidate = next_pos(pos, dir)
        if is_oob(candidate, board):
            return 0
        if board[candidate[0], candidate[1]] == 1:
            dir = turn[dir.value]
            continue

        pos = candidate


def day6a() -> None:
    board, pos, dir = read_board("../inputs/day6.txt")

    visited = set()
    while True:
        visited.add(hash_pos(pos, board.shape[1]))
        candidate = next_pos(pos, dir)
        if is_oob(candidate, board):
            break
        if board[candidate[0], candidate[1]] == 1:
            dir = turn[dir.value]
            continue

        pos = candidate

    print(len(visited))


def day6b() -> None:
    board, pos, dir = read_board("../inputs/day6.txt")

    num_loops = 0
    blocks_placed = set()
    while True:
        candidate = next_pos(pos, dir)
        if is_oob(candidate, board):
            break
        if board[candidate[0], candidate[1]] == 1:
            dir = turn[dir.value]
            continue

        hash_candidate = hash_pos(candidate, board.shape[0])
        if hash_candidate not in blocks_placed:
            blocks_placed.add(hash_candidate)
            board[candidate[0], candidate[1]] = 1
            num_loops += check_loop(pos, dir, board)
            board[candidate[0], candidate[1]] = 0

        pos = candidate

    print(num_loops)


day6b()
