#include <algorithm>
#include <cstddef>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using NodesSet = std::unordered_set<std::string>;
using Neighbors = std::unordered_map<std::string, NodesSet>;

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

auto
get_clique(const NodesSet& initial, const Neighbors& neighbors, const NodesSet& candidates)
    -> NodesSet {
    NodesSet max_clique{ initial };
    NodesSet unseen_candidates{ candidates };
    for (const auto& candidate : candidates) {
        const auto& cn = neighbors.at(candidate);
        const auto new_candidates = set_intersection(cn, unseen_candidates);
        unseen_candidates.erase(candidate);

        auto new_initial{ initial };
        new_initial.insert(candidate);
        if (!new_candidates.empty()) {
            auto clique = get_clique(new_initial, neighbors, new_candidates);
            if (clique.size() > max_clique.size()) {
                max_clique = std::move(clique);
            }
        } else {
            if (new_initial.size() > max_clique.size()) {
                max_clique = std::move(new_initial);
            }
        }
    }
    return max_clique;
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day23.txt") };

    Neighbors neighbors{};
    for (const auto& line : lines) {
        const auto parts = utils::split_string(line, "-");
        neighbors[parts[0]].insert(parts[1]);
        neighbors[parts[1]].insert(parts[0]);
    }

    NodesSet nodes{};
    for (const auto& item : neighbors) {
        nodes.insert(item.first);
    }
    const auto max_clique = get_clique({}, neighbors, nodes);
    std::vector<std::string> max_clique_vec(max_clique.begin(), max_clique.end());
    std::ranges::sort(max_clique_vec);

    std::string joined;
    for (std::size_t i = 0; i < max_clique_vec.size(); ++i) {
        if (i > 0) {
            joined += ",";
        }
        joined += max_clique_vec[i];
    }
    std::println("{}", joined);
}
