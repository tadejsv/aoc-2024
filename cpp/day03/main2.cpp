#include <cstddef>
#include <cstdint>
#include <print>
#include <regex>
#include <string>

#include "utils/utils.h"

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day3.txt") };

    std::string inp{};
    for (const auto& line : lines) {
        inp += line;
    }

    std::regex pattern(R"((mul\((\d{1,3}),(\d{1,3})\))|(do\(\))|(don't\(\)))");
    auto matches_begin = std::sregex_iterator(inp.begin(), inp.end(), pattern);
    auto matches_end = std::sregex_iterator();

    bool add{ true };
    std::int32_t sum{ 0 };
    for (std::sregex_iterator i = matches_begin; i != matches_end; ++i) {
        const std::smatch& match = *i;
        if (match[4].matched) {
            add = true;
            continue;
        }
        if (match[5].matched) {
            add = false;
            continue;
        }
        if (add) {
            sum += std::stoi(match[2].str()) * std::stoi(match[3].str());
        }
    }

    std::println("Sum: {0}", sum);
}