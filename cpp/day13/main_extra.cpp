#include <array>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "utils/utils.h"

struct Machine {
    std::array<int64_t, 2> a_vals{};
    std::array<int64_t, 2> b_vals{};
    std::array<int64_t, 2> p_vals{};
};

// Perform expanded euclidean algorithm to obtain gcd(a,b) and x and y from bezout's identity
auto
expanded_euclid(int64_t a, int64_t b) -> std::tuple<int64_t, int64_t, int64_t> {
    if (b == 0) {
        return { a, 1, 0 };
    }

    auto [gcd, x, y] = expanded_euclid(b, a % b);
    int64_t new_x{ y };
    int64_t new_y{ x - (a / b) * y };

    return { gcd, new_x, new_y };
}

// Get gcd(a,b) by using euclidean algorithm
auto
gcd_euclid(int64_t a, int64_t b) -> int64_t {
    if (b == 0) {
        return a;
    }
    return gcd_euclid(b, a % b);
}

auto
solve(const Machine& machine) -> int64_t {
    constexpr int64_t cost_a{ 3 };
    constexpr int64_t cost_b{ 1 };

    int64_t det{ (machine.b_vals[1] * machine.a_vals[0]) - (machine.b_vals[0] * machine.a_vals[1]) };
    int64_t b_solution{ 0 };

    // Case with co-linear equations
    if (det == 0) {
        // Rule out the inconsistent case
        if (machine.a_vals[1] / machine.a_vals[0] != machine.p_vals[1] / machine.p_vals[0]) {
            return 0;
        }

        // Rule out the case with no possible integer solutions
        if (machine.p_vals[0] % gcd_euclid(machine.a_vals[0], machine.b_vals[0]) != 0) {
            return 0;
        }

        // Situation with many possible solutions, need to pick one with min A or min B, depending
        // on the ration between their cost and "productivity"

        // Start by computing B mod a1' = p1 * (b1)^(-1) mod a1', where a1' = a1 / gcd(a1,b1)
        const auto a1_prime{ machine.a_vals[0] / gcd_euclid(machine.a_vals[0], machine.b_vals[0]) };
        const auto [_gcd, b1_inv, _a1pinv] = expanded_euclid(machine.b_vals[0], a1_prime);
        auto b_mod{ (machine.p_vals[0] * b1_inv) % a1_prime };
        if (b_mod < 0) {
            b_mod += a1_prime;
        }

        // Choose the maximum of the most cost-effective button
        if (cost_a / cost_b > machine.a_vals[0] / machine.b_vals[0]) {
            int64_t b_max{ machine.p_vals[0] / machine.b_vals[0] };
            b_solution = a1_prime * (b_max / a1_prime) + b_mod;
            if (b_solution > b_max) {
                b_solution -= a1_prime;
            }
        } else {
            b_solution = b_mod;
        }
    } else {
        // Case with a unique solution (not necessarily integer)
        int64_t c_part{ (machine.p_vals[1] * machine.a_vals[0])
                        - (machine.p_vals[0] * machine.a_vals[1]) };
        b_solution = c_part / det;
    }

    int64_t a_solution{ (machine.p_vals[0] - b_solution * machine.b_vals[0]) / machine.a_vals[0] };
    if (a_solution < 0 || b_solution < 0) {
        return 0;
    }

    // Test that we indeed have an integer solution
    int64_t p_solution_1{ (machine.a_vals[0] * a_solution) + (machine.b_vals[0] * b_solution) };
    int64_t p_solution_2{ (machine.a_vals[1] * a_solution) + (machine.b_vals[1] * b_solution) };
    if (p_solution_1 == machine.p_vals[0] && p_solution_2 == machine.p_vals[1]) {
        return (cost_a * a_solution) + (cost_b * b_solution);
    }
    return 0;
};

int
main() {  // NOLINT
    std::vector<Machine> machines{};
    const auto& lines{ utils::read_lines_from_file("day13_extra.txt") };
    for (size_t i{ 0 }; i < lines.size(); i += 4) {
        const auto& a_parts{ utils::split_string(utils::split_string(lines[i], ": ")[1], ", ") };
        const auto& b_parts{ utils::split_string(utils::split_string(lines[i + 1], ": ")[1], ", ") };
        const auto& p_parts{ utils::split_string(utils::split_string(lines[i + 2], ": ")[1], ", ") };

        Machine machine{};
        for (size_t j{ 0 }; j < 2; ++j) {
            machine.a_vals.at(j) = std::stol(utils::split_string(a_parts[j], "+")[1]);
            machine.b_vals.at(j) = std::stol(utils::split_string(b_parts[j], "+")[1]);
            machine.p_vals.at(j) = std::stol(utils::split_string(p_parts[j], "=")[1]);
        }
        machines.push_back(machine);
    }

    int64_t sum_tokens{ 0 };
    for (const auto& machine : machines) {
        sum_tokens += solve(machine);
    }

    std::println("{}", sum_tokens);
}
