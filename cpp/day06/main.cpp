#include <cstddef>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using Pos = std::pair<Eigen::Index, Eigen::Index>;

enum Direction : uint8_t { Up, Down, Left, Right };

const std::unordered_map<Direction, Direction> turn{
    { Up, Right },
    { Right, Down },
    { Down, Left },
    { Left, Up },
};

auto
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

auto
is_oob(const Pos& pos, Eigen::Index nrows, Eigen::Index ncols) -> bool {
    if (pos.first < 0 || pos.first >= nrows) {
        return true;
    }
    if (pos.second < 0 || pos.second >= ncols) {
        return true;
    }
    return false;
}

auto
flatten_coord(const Pos& pos, const Eigen::Index ncols) -> int32_t {
    return static_cast<int32_t>((ncols * pos.first) + pos.second);
}

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

    std::unordered_set<int32_t> visited{};

    while (true) {
        visited.insert(flatten_coord(pos, board.cols()));
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

    std::println("Size: {}", visited.size());
}