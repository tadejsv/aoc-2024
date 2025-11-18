#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include "utils/utils.h"

enum class Operation : uint8_t { AND, OR, XOR };

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day24.txt") };

    std::unordered_map<std::string, uint8_t> inputs{};
    size_t i{ 0 };
    for (; i < lines.size(); ++i) {
        if (lines[i].size() == 0) {
            break;
        }

        const auto parts{ utils::split_string(lines[i], ": ") };
        inputs[parts[0]] = static_cast<uint8_t>(std::stoi(parts[1]));
    }

    std::unordered_map<std::string, std::pair<std::string, std::string>> in_conns{};
    std::unordered_map<std::string, std::vector<std::string>> out_conns{};
    std::unordered_map<std::string, Operation> operations{};

    ++i;
    for (; i < lines.size(); ++i) {
        const auto parts{ utils::split_string(lines[i], " ") };

        const std::string& input1{ parts[0] };
        const std::string& input2{ parts[2] };
        const std::string& output{ parts[4] };
        const std::string& operation{ parts[1] };

        in_conns[output] = { input1, input2 };
        out_conns[input1].push_back(output);
        out_conns[input2].push_back(output);

        if (operation == "AND") {
            operations[output] = Operation::AND;
        } else if (operation == "OR") {
            operations[output] = Operation::OR;
        } else {
            operations[output] = Operation::XOR;
        }
    }

    // Sort the graph topologically
    std::unordered_map<std::string, uint8_t> in_edge{};
    std::unordered_map<std::string, std::vector<std::string>> out_edge{ out_conns };
    for (const auto& item : in_conns) {
        in_edge[item.first] = 2;
    }

    std::deque<std::string> queue{};
    std::vector<std::string> ordered_nodes{};
    ordered_nodes.reserve(inputs.size());
    for (const auto& item : inputs) {
        ordered_nodes.push_back(item.first);
        queue.push_back(item.first);
    }

    while (!queue.empty()) {
        const std::string node{ queue.front() };
        queue.pop_front();

        for (const auto& conn : out_edge[node]) {
            --in_edge[conn];
            if (in_edge[conn] == 0) {
                queue.push_back(conn);
                ordered_nodes.push_back(conn);
            }
        }
    }

    // Run the graph
    std::unordered_map<std::string, uint8_t> values{ inputs };

    for (const auto& node : ordered_nodes) {
        if (!operations.contains(node)) {
            continue;
        }

        const auto inp1{ values[in_conns[node].first] };
        const auto inp2{ values[in_conns[node].second] };
        const auto operation{ operations[node] };

        uint8_t output{ 0 };
        switch (operation) {
            case Operation::AND:
                output = inp1 * inp2;
                break;
            case Operation::OR:
                output = std::max(inp1, inp2);
                break;
            case Operation::XOR:
                if (inp1 != inp2) {
                    output = 1;
                }
                break;
        }
        values[node] = output;
    }

    // Compute final result
    std::vector<std::string> z_nodes{};
    for (const auto& item : values) {
        if (item.first[0] == 'z') {
            z_nodes.push_back(item.first);
        }
    }
    std::ranges::sort(z_nodes);

    int64_t result{ 0 };
    for (size_t i{ 0 }; i < z_nodes.size(); ++i) {
        result += static_cast<int64_t>(values[z_nodes[i]]) << i;
    }

    std::println("{}", result);
}