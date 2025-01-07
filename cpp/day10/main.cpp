#include <cstddef>
#include <cstdint>
#include <deque>
#include <Eigen/Core>
#include <print>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using MatrixI32 = Eigen::Matrix<int32_t, Eigen::Dynamic, Eigen::Dynamic>;

auto
cell_index(Eigen::Index row, Eigen::Index col, Eigen::Index num_cols) -> Eigen::Index {
    return (row * num_cols) + col;
}

auto
is_oob(Eigen::Index row, Eigen::Index col, Eigen::Index n_rows, Eigen::Index n_cols) -> bool {
    return (row < 0) || (col < 0) || (row >= n_rows) || (col >= n_cols);
};

auto
bfs(const MatrixI32& grid, Eigen::Index row, Eigen::Index col) -> int32_t {
    std::unordered_set<Eigen::Index> visited{};
    std::deque<std::pair<Eigen::Index, Eigen::Index>> queue{};
    queue.emplace_back(row, col);

    std::array<std::pair<Eigen::Index, Eigen::Index>, 4> offsets{
        { { 0, 1 }, { 1, 0 }, { -1, 0 }, { 0, -1 } }
    };

    auto n_rows{ grid.rows() };
    auto n_cols{ grid.cols() };

    int32_t num_trailheads{ 0 };
    while (!queue.empty()) {
        const auto [row, col] = queue.front();
        queue.pop_front();
        for (auto& neighbor_offset : offsets) {
            const auto neighbor_row{ row + neighbor_offset.first };
            const auto neighbor_col{ col + neighbor_offset.second };

            if (is_oob(neighbor_row, neighbor_col, n_rows, n_cols)) {
                continue;
            }
            if (grid(row, col) + 1 != grid(neighbor_row, neighbor_col)) {
                continue;
            }

            const auto neighbor_ind{ cell_index(neighbor_row, neighbor_col, grid.cols()) };
            if (!visited.contains(neighbor_ind)) {
                visited.insert(neighbor_ind);
                if (grid(neighbor_row, neighbor_col) == 9) {
                    ++num_trailheads;
                }
                queue.emplace_back(neighbor_row, neighbor_col);
            }
        }
    }
    return num_trailheads;
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day10.txt") };

    MatrixI32 grid(lines.size(), lines[0].size());

    for (Eigen::Index i{ 0 }; i < grid.rows(); ++i) {
        for (Eigen::Index j{ 0 }; j < grid.cols(); ++j) {
            grid(i, j) = lines[static_cast<size_t>(i)][static_cast<size_t>(j)] - '0';
        }
    }

    int32_t num_trailheads{ 0 };
    for (Eigen::Index i{ 0 }; i < grid.rows(); ++i) {
        for (Eigen::Index j{ 0 }; j < grid.cols(); ++j) {
            if (grid(i, j) == 0) {
                num_trailheads += bfs(grid, i, j);
            }
        }
    }

    std::println("Num trailheads: {}", num_trailheads);
}