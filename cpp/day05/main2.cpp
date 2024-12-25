#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <print>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils/utils.h"

using Graph = std::unordered_map<int32_t, std::unordered_set<int32_t>>;

template <class C>
auto
sets_intersect(const C& set1, const std::unordered_set<typename C::value_type>& set2) -> bool {
    using T = typename C::value_type;
    return std::ranges::any_of(set1, [&](T element) { return set2.contains(element); });
}

template <class C>
auto
set_intersection(const C& set1, const std::unordered_set<typename C::value_type>& set2)
    -> std::unordered_set<typename C::value_type> {
    using T = typename C::value_type;
    std::unordered_set<T> intersection{};
    for (const auto& el1 : set1) {
        if (set2.contains(el1)) {
            intersection.insert(el1);
        }
    }
    return intersection;
}

auto
topo_order(std::span<const int32_t> els, Graph& outgoing, Graph& incoming) -> std::vector<int32_t> {
    Graph sub_outgoing{};
    Graph sub_incoming{};
    std::vector<int32_t> no_incoming{};
    std::vector<int32_t> ordered{};

    for (auto elm : els) {
        sub_incoming[elm] = set_intersection(els, incoming[elm]);
        sub_outgoing[elm] = set_intersection(els, outgoing[elm]);

        if (sub_incoming[elm].empty()) {
            no_incoming.push_back(elm);
        }
    }

    while (!no_incoming.empty()) {
        const auto node{ no_incoming.back() };
        no_incoming.pop_back();
        ordered.push_back(node);

        for (const auto neighbor : sub_outgoing[node]) {
            sub_incoming[neighbor].erase(node);
            if (sub_incoming[neighbor].empty()) {
                no_incoming.push_back(neighbor);
            }
        }
    }

    return ordered;
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day5.txt") };

    Graph incoming{};
    Graph outgoing{};

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
        std::unordered_set<int32_t> current{};
        for (const auto num : tcase) {
            if (sets_intersect(current, outgoing[num])) {
                const auto ordered_tcase{ topo_order(tcase, outgoing, incoming) };
                sum += ordered_tcase[ordered_tcase.size() / 2];
                break;
            }
            current.insert(num);
        }
    }

    std::println("Sum: {0}", sum);
}