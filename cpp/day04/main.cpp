#include <array>
#include <cstddef>
#include <cstdint>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <print>
#include <string>

#include "utils/utils.h"

constexpr std::array<char, 4> xmas{ 'X', 'M', 'A', 'S' };

auto
find_xmas(
    const Eigen::Matrix<char, -1, -1>& board,
    Eigen::Index x,
    Eigen::Index y,
    Eigen::Index dx,
    Eigen::Index dy
) -> bool {
    const Eigen::Index height{ board.rows() };
    const Eigen::Index width{ board.rows() };

    if (x + 3 * dx < 0 || x + 3 * dx >= height) {
        return false;
    }
    if (y + 3 * dy < 0 || y + 3 * dy >= width) {
        return false;
    }

    for (const auto& letter : xmas) {
        if (board(x, y) != letter) {
            return false;
        }
        x += dx;
        y += dy;
    }
    return true;
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day4.txt") };

    Eigen::Matrix<char, Eigen::Dynamic, Eigen::Dynamic> board(lines.size(), lines[0].size());

    for (std::size_t i{ 0 }; i < lines.size(); ++i) {
        for (std::size_t j{ 0 }; j < lines[0].size(); ++j) {
            board(static_cast<Eigen::Index>(i), static_cast<Eigen::Index>(j)) = lines[i][j];
        }
    }

    const std::array<std::pair<Eigen::Index, Eigen::Index>, 8> dirs{
        {
            { 1, 1 },
            { 1, 0 },
            { 1, -1 },
            { 0, 1 },
            { 0, -1 },
            { -1, 1 },
            { -1, 0 },
            { -1, -1 },
        },
    };

    std::int32_t count{ 0 };
    for (Eigen::Index i{ 0 }; i < board.rows(); ++i) {
        for (Eigen::Index j{ 0 }; j < board.cols(); ++j) {
            if (board(i, j) != 'X') {
                continue;
            }
            for (const auto& dir : dirs) {
                if (find_xmas(board, i, j, dir.first, dir.second)) {
                    count++;
                }
            }
        }
    }

    std::println("Count: {0}", count);
}