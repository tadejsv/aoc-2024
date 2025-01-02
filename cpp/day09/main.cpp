#include <cstddef>
#include <cstdint>
#include <print>
#include <vector>

#include "utils/utils.h"

struct Space {
    int64_t start;
    int64_t len;
    int64_t index;
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day9.txt") };

    std::vector<Space> spaces{};
    spaces.reserve(lines[0].size());
    int64_t start{ 0 };
    for (size_t i{ 0 }; i < lines[0].size(); ++i) {
        int64_t index = -1;
        auto len = static_cast<int64_t>(lines[0][i] - '0');
        if (i % 2 == 0) {
            index = static_cast<int64_t>(i) / 2;
        }
        spaces.emplace_back(start, len, index);
        start += len;
    }

    size_t curr_empty_space_idx{ 1 };
    for (size_t i{ lines[0].size() - 1 }; i > curr_empty_space_idx; i -= 2) {
        Space& space{ spaces[i] };
        while (i > curr_empty_space_idx && space.len > 0) {
            Space& empty{ spaces[curr_empty_space_idx] };
            if (empty.len > space.len) {
                space.start = empty.start;
                empty.start += space.len;
                empty.len -= space.len;
                break;
            }

            empty.index = space.index;
            space.len -= empty.len;
            curr_empty_space_idx += 2;
        }
    }

    int64_t sum{ 0 };
    for (const auto& space : spaces) {
        if (space.index > 0 && space.len > 0) {
            for (int64_t i{ 0 }; i < space.len; ++i) {
                sum += space.index * (space.start + i);
            }
        }
    }
    std::println("{}", sum);
}