#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "utils/utils.h"

constexpr int32_t width{ 101 };
constexpr int32_t height{ 103 };
constexpr int32_t periods{ 10000 };

auto
draw_map(const std::vector<std::pair<int32_t, int32_t>>& positions, int32_t period) -> void {
    std::array<std::array<bool, height>, width> table{};

    for (const auto& pos : positions) {
        table.at(static_cast<size_t>(pos.first)).at(static_cast<size_t>(pos.second)) = true;
    }

    int32_t max_line{ 0 };
    for (size_t i{ 0 }; i < height; ++i) {
        int32_t line{ 0 };
        for (size_t j{ 0 }; j < width; ++j) {
            if (table[j][i]) {  // NOLINT
                ++line;
            } else {
                line = 0;
            }
            max_line = std::max(line, max_line);
        }
    }

    if (max_line > 10) {  // NOLINT
        std::println("heeeeeey {}", period);
        for (size_t i{ 0 }; i < height; ++i) {
            for (size_t j{ 0 }; j < width; ++j) {
                if (table[j][i]) { // NOLINT
                    std::print("*");
                } else {
                    std::print(" ");
                }
            }
            std::print("\n");
        }
    }
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day14.txt") };

    std::vector<std::pair<int32_t, int32_t>> positions{};
    std::vector<std::pair<int32_t, int32_t>> velocities{};

    for (const auto& line : lines) {
        const auto parts{ utils::split_string(line, " ") };
        const auto pos_parts{ utils::split_string(parts[0], "=") };
        const auto pos_nums{ utils::split_string(pos_parts[1], ",") };

        const auto vel_parts{ utils::split_string(parts[1], "=") };
        const auto vel_nums{ utils::split_string(vel_parts[1], ",") };

        positions.emplace_back(std::stoi(pos_nums[0]), std::stoi(pos_nums[1]));
        velocities.emplace_back(std::stoi(vel_nums[0]), std::stoi(vel_nums[1]));
    }

    for (int32_t period{ 0 }; period < periods; ++period) {
        for (size_t i{ 0 }; i < positions.size(); ++i) {
            positions[i].first += velocities[i].first % width;
            positions[i].second += velocities[i].second % height;
            positions[i].first = (positions[i].first + width) % width;
            positions[i].second = (positions[i].second + height) % height;
        }
        draw_map(positions, period);
    }
}