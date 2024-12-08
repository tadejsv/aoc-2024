#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/utils.h"

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day1.txt") };

    std::vector<std::int32_t> col1{};
    std::unordered_map<std::int32_t, std::int32_t> col2_counts{};

    for (const auto& line : lines) {
        const auto& parts = utils::split_string(line, "   ");
        col1.push_back(std::stoi(parts[0]));
        col2_counts[std::stoi(parts[1])] += 1;
    }

    std::int32_t sum{ 0 };
    for (const auto& key : col1) {
        sum += key * col2_counts[key];
    }
    std::println("Sum: {0}", sum);
}