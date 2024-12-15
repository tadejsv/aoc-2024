#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

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
        const bool decreasing{ line_nums[0] > line_nums[1] };

        bool ok{ true };
        for (std::size_t i{ 1 }; i < line_nums.size(); i++) {
            const auto diff{ line_nums[i] - line_nums[i - 1] };
            if (abs(diff) < 1 || abs(diff) > 3) {
                ok = false;
                break;
            }
            if ((decreasing && diff > 0) || (!decreasing && diff < 0)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            good++;
        }
    }

    std::println("Num good: {0}", good);
}