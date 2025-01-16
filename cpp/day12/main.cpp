#include <cstddef>
#include <cstdint>
#include <deque>
#include <Eigen/Core>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

using MatrixB = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;
using Coords = std::pair<size_t, size_t>;

constexpr std::array<Coords, 4> neighbor_offsets{ { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } } };

auto
is_oob(size_t row, size_t col, size_t n_rows, size_t n_cols) -> bool {
    return row < 0 || col < 0 || row >= n_rows || col >= n_cols;
};

auto
cost_region(const std::vector<std::string>& map, MatrixB& visited, size_t row, size_t col)
    -> int32_t {
    const char rtype{ map[row][col] };

    std::deque<Coords> queue{};
    queue.emplace_back(row, col);
    visited(static_cast<Eigen::Index>(row), static_cast<Eigen::Index>(col)) = true;

    int32_t area{ 0 };
    int32_t perimeter{ 0 };

    while (!queue.empty()) {
        auto [row, col] = queue.front();
        queue.pop_front();
        ++area;

        for (const auto& offset : neighbor_offsets) {
            const size_t n_row{ row + offset.first };
            const size_t n_col{ col + offset.second };

            if (is_oob(n_row, n_col, map.size(), map[0].size()) || map[n_row][n_col] != rtype) {
                ++perimeter;
                continue;
            }
            if (!visited(static_cast<Eigen::Index>(n_row), static_cast<Eigen::Index>(n_col))) {
                visited(static_cast<Eigen::Index>(n_row), static_cast<Eigen::Index>(n_col)) = true;
                queue.emplace_back(n_row, n_col);
            }
        }
    }
    return area * perimeter;
};

int
main() {  // NOLINT
    const auto& map{ utils::read_lines_from_file("day12.txt") };
    MatrixB visited(map.size(), map[0].size());
    visited.setConstant(false);

    int32_t total_cost{ 0 };
    for (size_t i{ 0 }; i < map.size(); ++i) {
        for (size_t j{ 0 }; j < map.size(); ++j) {
            if (!visited(static_cast<Eigen::Index>(i), static_cast<Eigen::Index>(j))) {
                total_cost += cost_region(map, visited, i, j);
            }
        }
    }

    std::println("{}", total_cost);
}