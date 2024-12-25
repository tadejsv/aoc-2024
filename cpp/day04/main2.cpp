#include <array>
#include <cstddef>
#include <cstdint>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <print>
#include <string>
#include <tuple>

#include "utils/utils.h"

auto
find_xmas(
    const Eigen::Matrix<char, -1, -1>& board,
    Eigen::Index x,
    Eigen::Index y,
    Eigen::Index dx1,
    Eigen::Index dy1,
    Eigen::Index dx2,
    Eigen::Index dy2
) -> bool {
    const Eigen::Index height{ board.rows() };
    const Eigen::Index width{ board.rows() };

    if (x - 1 < 0 || x + 1 >= height) {
        return false;
    }
    if (y - 1 < 0 || y + 1 >= width) {
        return false;
    }

    if (board(x + dx1, y + dy1) != 'M' || board(x + dx2, y + dy2) != 'M') {
        return false;
    }

    return board(x - dx1, y - dy1) == 'S' && board(x - dx2, y - dy2) == 'S';
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

    const std::array<std::tuple<Eigen::Index, Eigen::Index, Eigen::Index, Eigen::Index>, 4> dirs{
        {
            { 1, 1, -1, 1 },
            { -1, 1, -1, -1 },
            { -1, -1, 1, -1 },
            { 1, -1, 1, 1 },
        },
    };

    std::int32_t count{ 0 };
    for (Eigen::Index i{ 0 }; i < board.rows(); ++i) {
        for (Eigen::Index j{ 0 }; j < board.cols(); ++j) {
            if (board(i, j) != 'A') {
                continue;
            }
            for (const auto& dir : dirs) {
                if (find_xmas(
                        board,
                        i,
                        j,
                        std::get<0>(dir),
                        std::get<1>(dir),
                        std::get<2>(dir),
                        std::get<3>(dir)
                    )) {
                    count++;
                }
            }
        }
    }

    std::println("Count: {0}", count);
}