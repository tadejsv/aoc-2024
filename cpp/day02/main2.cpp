#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

auto
check_line(const std::vector<std::int32_t>& line_nums) -> int {
    const bool decreasing{ line_nums[0] > line_nums[1] };

    for (std::size_t i{ 1 }; i < line_nums.size(); i++) {
        const auto diff{ line_nums[i] - line_nums[i - 1] };
        if (abs(diff) < 1 || abs(diff) > 3) {
            return static_cast<int>(i-1);
        }
        if ((decreasing && diff > 0) || (!decreasing && diff < 0)) {
            return static_cast<int>(i-1);
        }
    }
    return -1;
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day2.txt") };

    std::vector<std::vector<std::int32_t>> nums{};

    for (const auto& line : lines) {
        const auto& parts = utils::split_string(line, " ");
        std::vector<std::int32_t> line_nums{};
        line_nums.reserve(parts.size());
        for (const auto& part : parts) {
            line_nums.push_back(std::stoi(part));
        }
        nums.push_back(line_nums);
    }

    std::int32_t good{ 0 };
    for (const auto& line_nums : nums) {
        const int err_ind = check_line(line_nums);
        if (err_ind == -1) {
            ++good;
            continue;
        }

        for (const auto remove_inc : { -1, 0, 1 }) {
            if (err_ind + remove_inc < 0) {
                continue;
            }

            const auto remove_ind{ err_ind + remove_inc };
            std::vector<int> removed_nums;
            removed_nums.reserve(line_nums.size() - 1);
            removed_nums.insert(removed_nums.end(), line_nums.begin(), line_nums.begin() + remove_ind);
            removed_nums.insert(removed_nums.end(), line_nums.begin() + remove_ind + 1, line_nums.end());

            if (check_line(removed_nums) == -1) {
                ++good;
                break;
            }
        }
    }

    std::println("Num good: {0}", good);
}