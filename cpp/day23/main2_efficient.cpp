#include <cstddef>
#include <cstdint>
#include <Eigen/Dense>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils/utils.h"

using AdjMatrix = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>;
using Node = int32_t;
using Clique = std::vector<Node>;

void
get_clique(const AdjMatrix& adj, const Clique& initial, const Clique& candidates, Clique& best) {
    // If no candidates left, initial is a maximal clique
    if (candidates.empty()) {
        if (initial.size() > best.size()) {
            best = initial;
        }
        return;
    }

    // Very simple branch-and-bound (optional but helps a lot)
    if (initial.size() + candidates.size() <= best.size()) {
        return;
    }

    // We mimic your "unseen_candidates" by only looking at
    // candidates after the current index.
    for (std::size_t i = 0; i < candidates.size(); ++i) {
        Node v = candidates[i];

        // New clique = initial ∪ {v}
        Clique new_clique = initial;
        new_clique.push_back(v);

        // New candidates = neighbors(v) ∩ {candidates after i}
        Clique new_candidates;
        new_candidates.reserve(candidates.size() - i - 1);

        for (std::size_t j = i + 1; j < candidates.size(); ++j) {
            Node u = candidates[j];
            if (adj(v, u) != 0) {
                new_candidates.push_back(u);
            }
        }

        get_clique(adj, new_clique, new_candidates, best);
    }
}

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day23.txt") };

    std::unordered_map<std::string, int32_t> nodes_id{};
    auto get_id = [&](const std::string& name) {
        if (nodes_id.contains(name)) {
            return nodes_id[name];
        }

        const auto new_id{ static_cast<int32_t>(nodes_id.size()) };
        nodes_id[name] = new_id;
        return new_id;
    };


    std::vector<std::pair<int32_t, int32_t>> edges{};
    edges.reserve(lines.size());
    for (const auto& line : lines) {
        const auto parts = utils::split_string(line, "-");
        const std::string& node1{ parts[0] };
        const std::string& node2{ parts[1] };

        edges.emplace_back(get_id(node1), get_id(node2));
    }
    const int32_t num_nodes{ static_cast<int32_t>(nodes_id.size()) };

    std::vector<std::string> nodes_vec{};
    nodes_vec.resize(nodes_id.size());
    for (const auto& [name, id] : nodes_id) {
        nodes_vec[id] = name;
    }

    AdjMatrix adj{ AdjMatrix::Zero(num_nodes, num_nodes) };
    for (const auto [node1, node2] : edges) {
        adj(node1, node2) = 1;
        adj(node2, node1) = 1;
    }

    Clique initial;
    Clique candidates;
    candidates.reserve(num_nodes);
    for (Node i = 0; i < num_nodes; ++i) {
        candidates.push_back(i);
    }

    Clique best;
    get_clique(adj, initial, candidates, best);

    std::vector<std::string> best_nodes{};
    for (Node n : best) {
        best_nodes.push_back(nodes_vec[n]);
    }
    std::ranges::sort(best_nodes);

    std::string joined;
    for (const auto& node : best_nodes) {
        joined += ",";
        joined += node;
    }
    std::println("{}, {}", joined.substr(1, joined.size()), best_nodes.size());
}
