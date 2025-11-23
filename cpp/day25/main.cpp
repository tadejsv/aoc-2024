#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils/utils.h"

template <typename T>
auto
set_intersection(const std::unordered_set<T>& a, const std::unordered_set<T>& b)
    -> std::unordered_set<T> {
    std::unordered_set<T> intersection{};
    if (a.size() < b.size()) {
        for (const auto& el_a : a) {
            if (b.contains(el_a)) {
                intersection.insert(el_a);
            }
        }
    } else {
        for (const auto& el_b : b) {
            if (a.contains(el_b)) {
                intersection.insert(el_b);
            }
        }
    }

    return intersection;
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day25.txt") };

    int32_t current_id{ 0 };
    constexpr const size_t max_height{ 7 };

    std::unordered_map<int32_t, std::vector<size_t>> locks{};
    std::vector<std::array<std::unordered_set<int32_t>, max_height>> keys_idx{};
    keys_idx.resize(lines[0].size());


    size_t start{ 0 };
    for (size_t current_line{ 0 }; current_line < lines.size(); ++current_line) {
        // for this to work, I added an extra empty line at the end
        if (lines[current_line].size() > 0) {
            continue;
        }

        std::vector<size_t> heights(lines[start].size(), 0);
        for (size_t j{ 0 }; j < current_line - start; ++j) {
            const auto& line{ lines[start + j] };
            for (size_t i{ 0 }; i < lines[start].size(); ++i) {
                if (line[i] == '#') {
                    ++heights[i];
                }
            }
        }

        const bool is_key{ lines[start][0] == '.' };
        if (is_key) {
            for (size_t i{ 0 }; i < heights.size(); ++i) {
                keys_idx[i].at(heights[i]).insert(current_id);
            }
        } else {
            locks[current_id] = heights;
        }

        start = current_line + 1;
        ++current_id;
    }

    size_t total{ 0 };
    for (const auto& [lock_id, lock] : locks) {
        std::unordered_set<int32_t> candidates{};
        for (size_t i{ 0 }; i < lock.size(); ++i) {
            std::unordered_set<int32_t> num_candidates{};
            for (size_t height{ 0 }; height <= max_height - lock[i]; ++height) {
                num_candidates.insert_range(keys_idx[i].at(height));
            }
            if (i == 0) {
                candidates = num_candidates;
            } else {
                candidates = set_intersection(candidates, num_candidates);
            }
        }
        total += candidates.size();
    }

    std::println("{}", total);
}