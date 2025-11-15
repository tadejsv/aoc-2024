#include <algorithm>
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
    constexpr int BASE = 20;  // 10 negative + 10 positive
    constexpr int BASE_POW_3 = BASE * BASE * BASE;
    const auto& lines{ utils::read_lines_from_file("day22.txt") };

    std::vector<int32_t> total(static_cast<size_t>(BASE * BASE * BASE * BASE));  // 160000

    // last buyer that used this seq
    std::vector<int32_t> seen(static_cast<size_t>(BASE * BASE * BASE * BASE), -1);

    int buyer_id = 0;
    for (const auto& line : lines) {
        ++buyer_id;
        int64_t num{ std::stoll(line) };

        auto prev_digit = static_cast<int32_t>(num % 10);
        size_t seq = 0;

        for (int i = 0; i < ROUNDS; ++i) {
            num = gen_new_num(num);
            const auto digit = static_cast<int32_t>(num % 10);

            const int32_t diff = digit - prev_digit + 10;  // [0, 19]

            // rolling window of 4 diffs
            seq = (seq % BASE_POW_3) * BASE + static_cast<size_t>(diff);

            if (i >= 3 && seen[seq] != buyer_id) {
                seen[seq] = buyer_id;
                total[seq] += digit;  // add this buyer's first price for this seq
            }
            prev_digit = digit;
        }
    }

    std::int32_t max_sum{ 0 };
    for (const auto val : total) {
        max_sum = std::max(max_sum, val);
    }

    std::println("{}", max_sum);
}