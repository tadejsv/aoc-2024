#include <cstddef>
#include <cstdint>
#include <numeric>
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
add_antinodes(
    int32_t x,
    int32_t y,
    int32_t dx,
    int32_t dy,
    int32_t width,
    int32_t height,
    std::unordered_set<int32_t>& antinodes
) -> void {
    for (int32_t i{ 0 }; !is_oob(x + (i * dx), y + (i * dy), width, height); ++i) {
        antinodes.insert(cell_index(x + (i * dx), y + (i * dy), width));
    }
    for (int32_t i{ 0 }; !is_oob(x + (i * dx), y + (i * dy), width, height); --i) {
        antinodes.insert(cell_index(x + (i * dx), y + (i * dy), width));
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
                int32_t dx{ points[i].x - points[j].x };
                int32_t dy{ points[i].y - points[j].y };
                const int32_t gcd = std::gcd(dx, dy);
                dx /= gcd;
                dy /= gcd;

                add_antinodes(points[i].x, points[i].y, dx, dy, width, height, antinodes);
            }
        }
    }

    std::println("{}", antinodes.size());
}