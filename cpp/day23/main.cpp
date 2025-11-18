#include <algorithm>
#include <cstddef>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "utils/utils.h"

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day23.txt") };

    std::unordered_map<std::string, std::unordered_set<std::string>> neighbors{};
    for (const auto& line : lines) {
        const auto parts = utils::split_string(line, "-");
        neighbors[parts[0]].insert(parts[1]);
        neighbors[parts[1]].insert(parts[0]);
    }

    std::vector<std::string> t_nodes{};
    for (const auto& item : neighbors) {
        if (item.first[0] == 't') {
            t_nodes.push_back(item.first);
        }
    }

    std::unordered_set<std::string> triplets{};
    for (const auto& t_node : t_nodes) {
        const auto& t_node_neighbors{ neighbors[t_node] };
        for (const auto& neighbor : t_node_neighbors) {
            const auto& neighbor_neighbors{ neighbors[neighbor] };

            for (const auto& n_neighbor : neighbor_neighbors) {
                if (t_node_neighbors.contains(n_neighbor) && n_neighbor != t_node) {
                    std::array<std::string, 3> triplet{ t_node, neighbor, n_neighbor };
                    std::ranges::sort(triplet);
                    triplets.insert( triplet[0] + "," + triplet[1] + "," + triplet[2]);
                }
            }
        }
    }

    std::println("{}", triplets.size());
}