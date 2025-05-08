#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <print>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using Coords = std::pair<int32_t, int32_t>;

constexpr std::array<Coords, 4> dirs{ Coords{ -1, 0 }, Coords{ 0, 1 }, Coords{ 1, 0 }, Coords{ 0, -1 } };
constexpr int32_t num_dirs{ dirs.size() };

auto
hash_coord(const Coords& coords, const int32_t width) -> int32_t {
    return coords.first * width + coords.second;
};

struct QueueItem {
    Coords pos;
    int64_t score;
    size_t dir;
};

struct LowerScore {
    auto operator()(const struct QueueItem& a, const QueueItem& b) const -> bool {
        return b.score < a.score;
    }
};

constexpr int64_t TURN_PENALTY{ 1000 };

int
main() {  // NOLINT

    const auto& lines{ utils::read_lines_from_file("day16.txt") };

    const int32_t width{ static_cast<int32_t>(lines[0].size()) };
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

    std::unordered_map<int32_t, int64_t> min_score_square{};
    std::priority_queue<QueueItem, std::vector<QueueItem>, LowerScore> queue{};
    queue.emplace(start, 0, 1);

    int64_t min_score{ 0 };
    while (!queue.empty()) {
        auto el{ queue.top() };
        queue.pop();

        const auto pos_hash{ hash_coord(el.pos, width) };
        const auto ms_hash{ pos_hash * num_dirs + static_cast<int32_t>(el.dir) };
        if (walls.contains(pos_hash) || min_score_square.contains(ms_hash)) {
            continue;
        }

        if (el.pos.first == end.first && el.pos.second == end.second) {
            min_score = el.score;
            break;
        }

        min_score_square[ms_hash] = el.score;

        for (auto i{ el.dir + num_dirs - 1 }; i < el.dir + num_dirs + 2; i++) {
            auto new_dir_ind{ i % num_dirs };
            Coords new_pos{ el.pos.first + dirs.at(new_dir_ind).first,
                            el.pos.second + dirs.at(new_dir_ind).second };
            auto score{ el.score + 1 };
            if (new_dir_ind != el.dir) {
                score += TURN_PENALTY;
            }
            queue.emplace(new_pos, score, new_dir_ind);
        }
    }

    std::unordered_set<int32_t> best_path{};
    std::deque<QueueItem> rev_queue{};
    for (size_t i{ 0 }; i < 4; ++i) {
        rev_queue.emplace_front(end, min_score, i);
    }

    while (!rev_queue.empty()) {
        auto el{ rev_queue.front() };
        rev_queue.pop_front();

        best_path.insert(hash_coord(el.pos, width));
        if (el.pos.first == start.first && el.pos.second == start.second) {
            continue;
        }

        auto reverse_dir{ dirs.at((el.dir + 2) % num_dirs) };
        Coords new_pos{ el.pos.first + reverse_dir.first, el.pos.second + reverse_dir.second };
        const auto pos_hash{ hash_coord(new_pos, width) };
        for (auto j{ el.dir + num_dirs - 1 }; j < el.dir + num_dirs + 2; j++) {
            auto from_dir_ind{ j % 4 };
            const auto ms_hash{ pos_hash * num_dirs + static_cast<int32_t>(from_dir_ind) };
            if (!min_score_square.contains(ms_hash)) {
                continue;
            }

            const auto ms{ min_score_square[ms_hash] };
            auto new_score{ el.score - 1 };
            if (el.dir != from_dir_ind) {
                new_score -= TURN_PENALTY;
            }
            if (new_score != ms) {
                continue;
            }

            rev_queue.emplace_front(new_pos, new_score, from_dir_ind);
        }
    }
    std::println("best path size: {}", best_path.size());
}