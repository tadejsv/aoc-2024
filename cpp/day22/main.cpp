#include <cstddef>
#include <cstdint>
#include <print>
#include <string>

#include "utils/utils.h"


constexpr int32_t MOD{ 16777216 };

auto
gen_new_num(int64_t num) -> int64_t {
    int64_t new_num{ num * 64 };
    num = (num ^ new_num) % MOD;

    new_num = num / 32;
    num = (num ^ new_num) % MOD;

    new_num = num * 2048;
    num = (num ^ new_num) % MOD;
    return num;
};

int
main() {  // NOLINT
    constexpr int ROUNDS = 2000;
    const auto& lines{ utils::read_lines_from_file("day22.txt") };

    int64_t sum{ 0 };
    for (const auto& line : lines) {
        int64_t num{ std::stoll(line) };

        for (int i{ 0 }; i < ROUNDS; ++i) {
            num = gen_new_num(num);
        }
        sum += num;
    }

    std::println("sum: {}", sum);
}