#include <cstddef>
#include <print>
#include <string>
#include <vector>

#include "utils/utils.h"

constexpr const size_t max_height{ 7 };

auto
compatible(const std::vector<size_t>& lock, const std::vector<size_t>& key) -> bool {
    for (size_t i = 0; i < lock.size(); ++i) {
        if (lock[i] + key[i] > max_height) {
            return false;
        }
    }
    return true;
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day25.txt") };

    std::vector<std::vector<size_t>> locks{};
    std::vector<std::vector<size_t>> keys{};

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

        if (lines[start][0] == '.') {  // is key
            keys.push_back(heights);
        } else {  // is lock
            locks.push_back(heights);
        }

        start = current_line + 1;
    }

    size_t total = 0;
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            if (compatible(lock, key)) {
                ++total;
            }
        }
    }

    std::println("{}", total);
}