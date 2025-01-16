#include <cstddef>
#include <cstdint>
#include <deque>
#include <Eigen/Core>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

using MatrixB = Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic>;
using Coords = std::pair<Eigen::Index, Eigen::Index>;

constexpr std::array<Coords, 8> neighbor_offsets{
    { { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, 1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } }
};
constexpr std::array<Coords, 4> corners{ { { 2, 2 }, { 2, 0 }, { 0, 2 }, { 0, 0 } } };

auto
is_neighbor(Eigen::Index row, Eigen::Index col, const std::vector<std::string>& map, char rtype)
    -> bool {
    if (row < 0 || col < 0 || static_cast<size_t>(row) >= map.size()
        || static_cast<size_t>(col) >= map[0].size()) {
        return false;
    }

    if (map[static_cast<size_t>(row)][static_cast<size_t>(col)] != rtype) {
        return false;
    }
    return true;
};

auto
cost_region(const std::vector<std::string>& map, MatrixB& visited, size_t row, size_t col)
    -> int32_t {
    const char rtype{ map[row][col] };

    std::deque<Coords> queue{};
    queue.emplace_back(row, col);
    visited(static_cast<Eigen::Index>(row), static_cast<Eigen::Index>(col)) = true;

    int32_t area{ 0 };
    int32_t sides{ 0 };

    while (!queue.empty()) {
        auto [row, col] = queue.front();
        queue.pop_front();
        ++area;

        // Check neighbors in 3x3 area around current position
        Eigen::Matrix3i neighbors{};
        neighbors.setOnes();
        for (const auto& offset : neighbor_offsets) {
            const Eigen::Index n_row{ row + offset.first };
            const Eigen::Index n_col{ col + offset.second };

            if (!is_neighbor(n_row, n_col, map, rtype)) {
                neighbors(1 + offset.first, 1 + offset.second) = 0;
                continue;
            }

            // Only add non-diagonal unvisited neighbors to the queue
            if (!visited(n_row, n_col) && offset.first * offset.second == 0) {
                visited(n_row, n_col) = true;
                queue.emplace_back(n_row, n_col);
            }
        }

        // Check for inner and outer corners
        for (const auto& offset : corners) {
            // Check inner corner
            if (neighbors(offset.first, 1) == 0 && neighbors(1, offset.second) == 0) {
                ++sides;
            }
            // Check outer corner
            if (neighbors(offset.first, 1) == 1 && neighbors(1, offset.second) == 1
                && neighbors(offset.first, offset.second) == 0) {
                ++sides;
            }
        }
    }
    return area * sides;
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