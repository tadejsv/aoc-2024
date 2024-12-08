#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day1.txt") };

    std::vector<std::int32_t> col1{};
    std::vector<std::int32_t> col2{};

    for (const auto& line : lines) {
        const auto& parts = utils::split_string(line, "   ");
        col1.push_back(std::stoi(parts[0]));
        col2.push_back(std::stoi(parts[1]));
    }

    std::ranges::sort(col1);
    std::ranges::sort(col2);

    std::int32_t sum{ 0 };
    for (std::size_t i{ 0 }; i < col1.size(); ++i) {
        sum += abs(col1[i] - col2[i]);
    }
    std::println("Sum: {0}", sum);
}