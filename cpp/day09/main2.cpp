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

    std::vector<Space> files{};
    std::vector<Space> spaces{};
    spaces.reserve(lines[0].size() / 2);
    files.reserve(lines[0].size() / 2);

    int64_t start{ 0 };
    for (size_t i{ 0 }; i < lines[0].size(); ++i) {
        auto len = static_cast<int64_t>(lines[0][i] - '0');
        if (i % 2 == 0) {
            files.emplace_back(start, len, static_cast<int64_t>(i) / 2);
        } else {
            spaces.emplace_back(start, len, -1);
        }
        start += len;
    }


    for (int32_t i{ static_cast<int32_t>(files.size() - 1) }; i >= 0; --i) {
        Space& file{ files[static_cast<size_t>(i)] };
        for (size_t j{ 0 }; j < spaces.size() && spaces[j].start < file.start; j++) {
            if (spaces[j].len >= file.len) {
                file.start = spaces[j].start;
                spaces[j].start += file.len;
                spaces[j].len -= file.len;
                break;
            }
        }
    }

    int64_t sum{ 0 };
    for (const auto& space : files) {
        for (int64_t i{ 0 }; i < space.len; ++i) {
            sum += space.index * (space.start + i);
        }
    }
    std::println("{}", sum);
}