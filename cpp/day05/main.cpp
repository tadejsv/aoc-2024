#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils/utils.h"

template <class T>
auto
sets_intersect(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2) -> bool {
    for (const auto& el1 : set1) {
        if (set2.contains(el1)) {
            return true;
        }
    }
    return false;
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day5.txt") };

    std::unordered_map<int32_t, std::unordered_set<int32_t>> incoming{};
    std::unordered_map<int32_t, std::unordered_set<int32_t>> outgoing{};

    std::size_t line_ind{ 0 };
    while (true) {
        if (lines[line_ind].size() == 0) {
            break;
        }

        const auto parts{ utils::split_string(lines[line_ind], "|") };

        const int32_t num1{ std::stoi(parts[0]) };
        const int32_t num2{ std::stoi(parts[1]) };

        incoming[num2].insert(num1);
        outgoing[num1].insert(num2);

        ++line_ind;
    }

    std::vector<std::vector<int32_t>> test_cases{};

    for (++line_ind; line_ind < lines.size(); ++line_ind) {
        const auto parts{ utils::split_string(lines[line_ind], ",") };
        std::vector<int32_t> tcase{};
        tcase.reserve(parts.size());
        for (const auto& part : parts) {
            tcase.push_back(std::stoi(part));
        }
        test_cases.push_back(tcase);
    }

    int32_t sum{ 0 };
    for (const auto& tcase : test_cases) {
        bool good{ true };
        std::unordered_set<int32_t> current{};
        for (const auto num : tcase) {
            if (sets_intersect(current, outgoing[num])) {
                good = false;
                break;
            }
            current.insert(num);
        }
        if (good) {
            sum += tcase[tcase.size() / 2];
        }
    }

    std::println("Sum: {0}", sum);
}