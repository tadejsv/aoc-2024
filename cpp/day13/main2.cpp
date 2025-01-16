#include <array>
#include <cstddef>
#include <cstdint>
#include <print>
#include <string>
#include <string_view>
#include <vector>

#include "utils/utils.h"

constexpr int64_t prize_add_const{ 10000000000000 };

struct Machine {
    std::array<int64_t, 2> a_vals{};
    std::array<int64_t, 2> b_vals{};
    std::array<int64_t, 2> p_vals{};
};

auto
solve(const Machine& machine) -> int64_t {
    // Check for co-linear equations - assume they all have no solutions (true for given inputs)
    int64_t det{ (machine.b_vals[1] * machine.a_vals[0]) - (machine.b_vals[0] * machine.a_vals[1]) };
    if (det == 0) {
        if (machine.a_vals[1] / machine.a_vals[0] != machine.p_vals[1] / machine.p_vals[0]) {
            return 0;
        }
        return 0;
    }

    int64_t c_part{ (machine.p_vals[1] * machine.a_vals[0])
                    - (machine.p_vals[0] * machine.a_vals[1]) };
    int64_t b_solution{ c_part / det };
    int64_t a_solution{ (machine.p_vals[0] - b_solution * machine.b_vals[0]) / machine.a_vals[0] };
    if (a_solution < 0 || b_solution < 0) {
        return 0;
    }

    // Test that we indeed have an integer solution
    int64_t p_solution_1{ (machine.a_vals[0] * a_solution) + (machine.b_vals[0] * b_solution) };
    int64_t p_solution_2{ (machine.a_vals[1] * a_solution) + (machine.b_vals[1] * b_solution) };

    if (p_solution_1 == machine.p_vals[0] && p_solution_2 == machine.p_vals[1]) {
        return (3 * a_solution) + b_solution;
    }
    return 0;
};

int
main() {  // NOLINT
    std::vector<Machine> machines{};
    const auto& lines{ utils::read_lines_from_file("day13.txt") };
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
        machine.p_vals[0] += prize_add_const;
        machine.p_vals[1] += prize_add_const;

        machines.push_back(machine);
    }

    int64_t sum_tokens{ 0 };
    for (const auto& machine : machines) {
        sum_tokens += solve(machine);
    }

    std::println("{}", sum_tokens);
}
