#include <array>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "utils/utils.h"

constexpr int32_t periods{ 100 };
constexpr int32_t width{ 101 };
constexpr int32_t height{ 103 };

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

    for (size_t i{ 0 }; i < positions.size(); ++i) {
        positions[i].first += (periods * velocities[i].first) % width;
        positions[i].second += (periods * velocities[i].second) % width;
        positions[i].first = (positions[i].first + width) % width;
        positions[i].second = (positions[i].second + height) % height;
    }

    std::array<int32_t, 4> qudrant_counts{};
    for (const auto& pos : positions) {
        if (pos.first > width / 2) {
            if (pos.second > height / 2) {
                qudrant_counts[0] += 1;
            } else if (pos.second < height / 2) {
                qudrant_counts[1] += 1;
            }
        } else if (pos.first < width / 2) {
            if (pos.second > height / 2) {
                qudrant_counts[2] += 1;
            } else if (pos.second < height / 2) {
                qudrant_counts[3] += 1;
            }
        }
    }

    std::println("{}", qudrant_counts[0] * qudrant_counts[1] * qudrant_counts[2] * qudrant_counts[3]);
}