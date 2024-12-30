#include <array>
#include <cstddef>
#include <cstdint>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <print>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using Pos = std::pair<Eigen::Index, Eigen::Index>;

enum Direction : uint8_t { Up, Down, Left, Right };

constexpr std::array<Direction, 4> turn = { Right, Left, Up, Down };

inline auto
next_pos(const Pos& current, const Direction dir) -> Pos {
    Pos next{ current };
    switch (dir) {
        case Up:
            next.first -= 1;
            break;
        case Down:
            next.first += 1;
            break;
        case Left:
            next.second -= 1;
            break;
        case Right:
            next.second += 1;
            break;
    }

    return next;
};

inline auto
is_oob(const Pos& pos, Eigen::Index nrows, Eigen::Index ncols) -> bool {
    if (pos.first < 0 || pos.first >= nrows) {
        return true;
    }
    if (pos.second < 0 || pos.second >= ncols) {
        return true;
    }
    return false;
}

inline auto
flatten_coord(const Direction dir, const Pos& pos, const Eigen::Index ncols) -> int32_t {
    return static_cast<int32_t>(((pos.first * ncols + pos.second) * 4) + dir);
}

auto
has_loop(Pos pos, Direction dir, const Eigen::MatrixXi& board) -> bool {
    std::unordered_set<int32_t> visited{};
    while (true) {
        if (!visited.insert(flatten_coord(dir, pos, board.cols())).second) {
            return true;
        }
        const auto candidate{ next_pos(pos, dir) };
        if (is_oob(candidate, board.rows(), board.cols())) {
            break;
        }
        if (board(candidate.first, candidate.second) == 1) {
            dir = turn.at(dir);
            continue;
        }

        pos = candidate;
    }
    return false;
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day6.txt") };

    Pos pos{ -1, -1 };
    Direction dir{ Up };
    Eigen::MatrixXi board(lines.size(), lines[0].size());
    for (Eigen::Index i{ 0 }; i < board.rows(); ++i) {
        for (Eigen::Index j{ 0 }; j < board.cols(); ++j) {
            const char chr{ lines[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] };
            if (chr == '#') {
                board(i, j) = 1;
            } else {
                board(i, j) = 0;
                if (chr == '>' || chr == '<' || chr == '^' || chr == 'v') {
                    pos = { i, j };
                    switch (chr) {
                        case '>':
                            dir = Right;
                            break;
                        case '<':
                            dir = Left;
                            break;
                        case '^':
                            dir = Up;
                            break;
                        case 'v':
                            dir = Down;
                            break;
                    }
                }
            }
        }
    }

    int32_t num_loops{ 0 };
    std::unordered_set<int32_t> blocks_placed{};
    while (true) {
        const auto candidate{ next_pos(pos, dir) };
        if (is_oob(candidate, board.rows(), board.cols())) {
            break;
        }
        if (board(candidate.first, candidate.second) == 1) {
            dir = turn.at(dir);
            continue;
        }

        const auto block_coord{ flatten_coord(Up, candidate, board.cols()) };
        if (blocks_placed.insert(block_coord).second) {
            board(candidate.first, candidate.second) = 1;
            if (has_loop(pos, dir, board)) {
                num_loops += 1;
            }
            board(candidate.first, candidate.second) = 0;
        }
        pos = candidate;
    }


    std::println("Loops: {}", num_loops);
}