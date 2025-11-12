#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <print>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using Coords = std::pair<int32_t, int32_t>;

constexpr std::array<Coords, 4> dirs{ Coords{ -1, 0 }, Coords{ 0, 1 }, Coords{ 1, 0 }, Coords{ 0, -1 } };

auto
hash_coord(const Coords& coords, const int32_t width) -> int32_t {
    return coords.first * width + coords.second;
};

auto
unhash_coord(const int32_t coords_hash, const int32_t width) -> Coords {
    return { coords_hash / width, coords_hash % width };
};

struct QueueItem {
    Coords pos;
    int32_t dist;
};

auto
dijkstra(
    const int32_t width,
    const int32_t height,
    const Coords start,
    const std::unordered_set<int32_t>& walls
) -> std::unordered_map<int32_t, int32_t> {
    std::unordered_map<int32_t, int32_t> dist{};

    std::deque<QueueItem> queue{ { .pos = start, .dist = 0 } };

    while (!queue.empty()) {
        const auto item{ queue.front() };
        queue.pop_front();

        const auto coords_hash{ hash_coord(item.pos, width) };

        if (dist.contains(coords_hash) || walls.contains(coords_hash)) {
            continue;
        }

        dist[coords_hash] = item.dist;

        for (const auto dir : dirs) {
            const Coords new_coords{ item.pos.first + dir.first, item.pos.second + dir.second };

            if (new_coords.first >= 0 && new_coords.first < height && new_coords.second >= 0
                && new_coords.second < width) {
                queue.emplace_back(new_coords, item.dist + 1);
            }
        }
    }

    return dist;
}

int
main() {  // NOLINT
    const int32_t DECREASE{ 100 };
    const int32_t CHEAT_DURATION{ 20 };

    const auto& lines{ utils::read_lines_from_file("day20.txt") };

    const int32_t width{ static_cast<int32_t>(lines[0].size()) };
    const int32_t height{ static_cast<int32_t>(lines.size()) };
    std::unordered_set<int32_t> walls{};
    Coords start{ -1, -1 };
    Coords end{ -1, -1 };

    for (size_t i{ 0 }; i < lines.size(); ++i) {
        for (size_t j{ 0 }; j < lines[i].size(); ++j) {
            switch (lines[i][j]) {
                case 'S':
                    start = { i, j };
                    break;
                case 'E':
                    end = { i, j };
                    break;
                case '#':
                    walls.insert(hash_coord({ i, j }, width));
                    break;
                default:
                    break;
            }
        }
    }

    auto from_start{ dijkstra(width, height, start, walls) };
    auto from_end{ dijkstra(width, height, end, walls) };

    const int32_t max_cost{ from_start.at(hash_coord(end, width)) };

    std::vector<Coords> candidates{};
    for (const auto item : from_start) {
        if (item.second < max_cost - DECREASE) {
            candidates.push_back(unhash_coord(item.first, width));
        }
    }

    std::int32_t count{ 0 };
    for (const auto& candidate : candidates) {
        const int32_t candidate_hash{ hash_coord(candidate, width) };
        const int32_t start_cost{ from_start[candidate_hash] };

        for (int32_t dr{ -CHEAT_DURATION }; dr <= CHEAT_DURATION; ++dr) {
            const int32_t row{ candidate.first + dr };
            if (row < 0 || row >= height) {
                continue;
            }

            const int32_t leftover{ CHEAT_DURATION - std::abs(dr) };
            for (int32_t dc{ -leftover }; dc <= leftover; ++dc) {
                const int32_t col{ candidate.second + dc };
                if (col < 0 || col >= width) {
                    continue;
                }

                const Coords end_pos{ row, col };
                const int32_t end_pos_hash{ hash_coord(end_pos, width) };

                if (walls.contains(end_pos_hash)) {
                    continue;
                }

                const auto final_cost = start_cost + std::abs(dr) + std::abs(dc)
                                        + from_end[end_pos_hash];
                if (final_cost <= max_cost - DECREASE) {
                    ++count;
                }
            }
        }
    };

    std::println("count cheats: {}", count);
}