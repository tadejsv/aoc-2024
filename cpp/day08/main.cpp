#include <cstddef>
#include <cstdint>
#include <print>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils/utils.h"

struct Point {
    int32_t y;
    int32_t x;
};

auto
is_oob(int32_t x, int32_t y, int32_t width, int32_t height) -> bool {
    return x < 0 || x >= width || y < 0 || y >= height;
};

auto
cell_index(int32_t x, int32_t y, int32_t width) -> int32_t {
    return (y * width) + x;
}

auto
add_antinode(int32_t x, int32_t y, int32_t width, int32_t height, std::unordered_set<int32_t>& antinodes)
    -> void {
    if (!is_oob(x, y, width, height)) {
        antinodes.insert(cell_index(x, y, width));
    }
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day8.txt") };
    const int32_t width{ static_cast<int32_t>(lines[0].size()) };
    const int32_t height{ static_cast<int32_t>(lines.size()) };


    std::unordered_map<char, std::vector<Point>> antennas{};

    for (size_t i{ 0 }; i < lines.size(); ++i) {
        for (size_t j{ 0 }; j < lines[0].size(); ++j) {
            if (lines[i][j] != '.') {
                antennas[lines[i][j]].emplace_back(j, i);
            }
        }
    }

    std::unordered_set<int32_t> antinodes{};
    for (auto& [_, points] : antennas) {
        for (size_t i{ 0 }; i < points.size(); ++i) {
            for (size_t j{ i + 1 }; j < points.size(); ++j) {
                const int32_t dx{ points[i].x - points[j].x };
                const int32_t dy{ points[i].y - points[j].y };

                add_antinode(points[i].x + dx, points[i].y + dy, width, height, antinodes);
                add_antinode(points[j].x - dx, points[j].y - dy, width, height, antinodes);
            }
        }
    }

    std::println("{}", antinodes.size());
}