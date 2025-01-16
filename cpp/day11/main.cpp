#include <cstddef>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/utils.h"

auto
num_digits(int64_t number) -> int32_t {
    int32_t digits = 0;
    if (number < 0) {
        digits = 1;
    }
    while (number != 0) {
        number /= 10;
        digits++;
    }
    return digits;
}

auto
pow_10(int64_t pow) -> int64_t {
    int64_t result = 1;
    while (pow-- > 0) {
        result *= 10;
    }

    return result;
}

auto
map_index(int64_t num, int64_t steps) -> int64_t {
    return (100 * num) + steps;
}

auto
expand(int64_t num, int64_t steps, std::unordered_map<int64_t, int64_t>& memory) -> int64_t {
    if (steps == 0) {
        return 1;
    }

    auto index{ map_index(num, steps) };
    if (memory.contains(index)) {
        return memory.at(index);
    }

    int64_t expanded{ 0 };
    if (num == 0) {
        expanded = expand(1, steps - 1, memory);
    } else {
        int32_t n_digits{ num_digits(num) };
        if (n_digits % 2 == 0) {
            expanded += expand(num / pow_10(n_digits / 2), steps - 1, memory);
            expanded += expand(num % pow_10(n_digits / 2), steps - 1, memory);
        } else {
            expanded = expand(num * 2024, steps - 1, memory);
        }
    }
    memory[index] = expanded;
    return expanded;
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day11.txt") };

    std::vector<int64_t> nums{};
    const auto& parts{ utils::split_string(lines[0], " ") };
    for (const auto& part : parts) {
        nums.push_back(std::stol(part));
    }

    int64_t total{ 0 };
    std::unordered_map<int64_t, int64_t> memory{};
    for (auto num : nums) {
        total += expand(num, 25, memory);
    };
    std::println("{}", total);
}