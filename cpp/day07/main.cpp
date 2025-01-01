#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

struct Row {
    int64_t target;
    std::vector<int64_t> nums;
};

auto
check_row(
    const std::vector<int64_t>& nums,
    const std::size_t index,
    const int64_t current_res,
    const int64_t target
) -> bool {
    if (index == nums.size()) {
        return target == current_res;
    }

    if (target < current_res) {
        return false;
    }

    if (check_row(nums, index + 1, current_res + nums[index], target)) {
        return true;
    }

    return check_row(nums, index + 1, std::max(current_res, int64_t(0)) * nums[index], target);
};

int
main() {  // NOLINT
    const auto lines{ utils::read_lines_from_file("day7.txt") };
    std::vector<Row> rows{};
    rows.reserve(lines.size());
    for (const auto& line : lines) {
        Row row{};
        const auto& parts{ utils::split_string(line, ": ") };
        const int64_t target{ std::stoll(parts[0]) };
        row.target = target;

        for (const auto& num_str : utils::split_string(parts[1], " ")) {
            row.nums.push_back(std::stoll(num_str));
        }
        rows.push_back(row);
    }

    int64_t sum_good{ 0 };
    for (const auto& row : rows) {
        if (check_row(row.nums, 1, row.nums[0], row.target)) {
            sum_good += row.target;
        }
    }

    std::println("{}", sum_good);
}