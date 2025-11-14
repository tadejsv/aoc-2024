#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <print>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "utils/utils.h"

struct PairHash {
    template <class T, class U>
    auto operator()(const std::pair<T, U>& p) const noexcept -> size_t {
        size_t h1 = std::hash<T>{}(p.first);
        size_t h2 = std::hash<U>{}(p.second);
        return h1 ^ (h2 << 1);  // simple combination
    }
};

using Coords = std::pair<int32_t, int32_t>;

enum class Directions : uint8_t {
    Up,
    Down,
    Left,
    Right,
    Press,
};

auto
make_dir_keyboard_moves()
    -> std::unordered_map<std::pair<Directions, Directions>, std::vector<std::vector<Directions>>, PairHash> {
    using D = Directions;

    std::unordered_map<std::pair<D, D>, std::vector<std::vector<D>>, PairHash> dir_keyboard_moves;

    const auto U = D::Up;
    const auto Dn = D::Down;
    const auto L = D::Left;
    const auto R = D::Right;
    const auto A = D::Press;

    // From Directions::Up
    dir_keyboard_moves[{ U, U }] = { {} };
    dir_keyboard_moves[{ U, Dn }] = { { Dn } };
    dir_keyboard_moves[{ U, L }] = { { Dn, L } };
    dir_keyboard_moves[{ U, R }] = { { Dn, R }, { R, Dn } };
    dir_keyboard_moves[{ U, A }] = { { R } };

    // From Directions::Press
    dir_keyboard_moves[{ A, U }] = { { L } };
    dir_keyboard_moves[{ A, A }] = { {} };
    dir_keyboard_moves[{ A, L }] = { { Dn, L, L }, { L, Dn, L } };
    dir_keyboard_moves[{ A, Dn }] = { { Dn, L }, { L, Dn } };
    dir_keyboard_moves[{ A, R }] = { { Dn } };

    // From Directions::Left
    dir_keyboard_moves[{ L, U }] = { { R, U } };
    dir_keyboard_moves[{ L, A }] = { { R, U, R }, { R, R, U } };
    dir_keyboard_moves[{ L, L }] = { {} };
    dir_keyboard_moves[{ L, Dn }] = { { R } };
    dir_keyboard_moves[{ L, R }] = { { R, R } };

    // From Directions::Down
    dir_keyboard_moves[{ Dn, U }] = { { U } };
    dir_keyboard_moves[{ Dn, A }] = { { U, R }, { R, U } };
    dir_keyboard_moves[{ Dn, L }] = { { L } };
    dir_keyboard_moves[{ Dn, Dn }] = { {} };
    dir_keyboard_moves[{ Dn, R }] = { { R } };

    // From Directions::Right
    dir_keyboard_moves[{ R, U }] = { { U, L }, { L, U } };
    dir_keyboard_moves[{ R, A }] = { { U } };
    dir_keyboard_moves[{ R, L }] = { { L, L } };
    dir_keyboard_moves[{ R, Dn }] = { { L } };
    dir_keyboard_moves[{ R, R }] = { {} };

    return dir_keyboard_moves;
}

const std::vector<std::pair<Directions, Coords>> dirs{ { Directions::Up, { -1, 0 } },
                                                       { Directions::Down, { 1, 0 } },
                                                       { Directions::Left, { 0, -1 } },
                                                       { Directions::Right, { 0, 1 } } };

auto
make_num_keyboard_moves()
    -> std::unordered_map<std::pair<int32_t, int32_t>, std::vector<std::vector<Directions>>, PairHash> {
    // num_positions
    std::unordered_map<int32_t, Coords> num_positions{};
    num_positions[0] = { 3, 1 };
    num_positions[1] = { 2, 0 };
    num_positions[2] = { 2, 1 };
    num_positions[3] = { 2, 2 };
    num_positions[4] = { 1, 0 };
    num_positions[5] = { 1, 1 };
    num_positions[6] = { 1, 2 };
    num_positions[7] = { 0, 0 };
    num_positions[8] = { 0, 1 };
    num_positions[9] = { 0, 2 };
    num_positions[10] = { 3, 2 };  // usually 'A' / Press

    // positions_num (reverse)
    std::unordered_map<Coords, int32_t, PairHash> positions_num{};
    for (const auto& item : num_positions) {
        positions_num[item.second] = item.first;
    }

    // num_keyboard_moves
    std::unordered_map<std::pair<int32_t, int32_t>, std::vector<std::vector<Directions>>, PairHash>
        num_keyboard_moves;

    struct QueueItem {
        int32_t dist;
        Coords pos;
        std::vector<Directions> moves;
    };

    for (int32_t num_from{ 0 }; num_from <= 10; ++num_from) {
        std::unordered_map<int32_t, int32_t> min_dist{};
        std::deque<QueueItem> queue{
            QueueItem{ .dist = 0, .pos = num_positions[num_from], .moves = {} }
        };

        while (!queue.empty()) {
            const auto item = queue.front();
            queue.pop_front();

            const int32_t num_to{ positions_num[item.pos] };
            if (min_dist.contains(num_to) && min_dist[num_to] < item.dist) {
                continue;
            }

            min_dist[num_to] = item.dist;
            num_keyboard_moves[{ num_from, num_to }].push_back(item.moves);

            for (const auto& dir_item : dirs) {
                const Coords new_pos{ item.pos.first + dir_item.second.first,
                                      item.pos.second + dir_item.second.second };

                if (new_pos.first < 0 || new_pos.second < 0 || new_pos.first > 3
                    || new_pos.second > 2 || (new_pos.first == 3 && new_pos.second == 0)) {
                    continue;
                }

                std::vector<Directions> new_moves{ item.moves };
                new_moves.push_back(dir_item.first);
                queue.emplace_back(
                    QueueItem{ .dist = item.dist + 1, .pos = new_pos, .moves = std::move(new_moves) }
                );
            }
        }
    }

    return num_keyboard_moves;
}

auto
hash_query_dir(const Directions from, const Directions to, const int32_t level) -> int32_t {
    return level * 25 + static_cast<int32_t>(to) * 5 + static_cast<int32_t>(from);
}

auto
hash_query_num(const int32_t from, const int32_t to) -> int32_t {
    return to * 11 + from;
}

auto
num_moves_dir(
    const Directions from,
    const Directions to,
    const int32_t level,
    const std::unordered_map<std::pair<Directions, Directions>, std::vector<std::vector<Directions>>, PairHash>&
        dir_keyboard_moves,
    std::unordered_map<int32_t, int64_t>& memory
) -> int64_t {
    const auto hash{ hash_query_dir(from, to, level) };
    if (memory.contains(hash)) {
        return memory[hash];
    }

    if (level == 0) {
        return 1;
    }
    int64_t min_cost{ std::numeric_limits<int64_t>::max() };

    auto all_moves = dir_keyboard_moves.find({ from, to });
    for (const auto& moves : all_moves->second) {
        int64_t move_sum{ 0 };
        Directions prev_button{ Directions::Press };
        for (const auto& button : moves) {
            move_sum += num_moves_dir(prev_button, button, level - 1, dir_keyboard_moves, memory);
            prev_button = button;
        }
        move_sum += num_moves_dir(prev_button, Directions::Press, level - 1, dir_keyboard_moves, memory);

        min_cost = std::min(min_cost, move_sum);
    }

    memory[hash] = min_cost;
    return min_cost;
};

auto
num_moves_num(
    const int32_t from,
    const int32_t to,
    const int32_t level,
    const std::unordered_map<std::pair<int32_t, int32_t>, std::vector<std::vector<Directions>>, PairHash>&
        num_keyboard_moves,
    const std::unordered_map<std::pair<Directions, Directions>, std::vector<std::vector<Directions>>, PairHash>&
        dir_keyboard_moves,
    std::unordered_map<int32_t, int64_t>& memory_num,
    std::unordered_map<int32_t, int64_t>& memory_dir
) -> int64_t {
    const auto hash{ hash_query_num(from, to) };
    if (memory_num.contains(hash)) {
        return memory_num[hash];
    }

    int64_t min_cost{ std::numeric_limits<int64_t>::max() };

    auto all_moves = num_keyboard_moves.find({ from, to });
    for (const auto& moves : all_moves->second) {
        int64_t move_sum{ 0 };
        Directions prev_button{ Directions::Press };
        for (const auto& button : moves) {
            move_sum += num_moves_dir(prev_button, button, level - 1, dir_keyboard_moves, memory_dir);
            prev_button = button;
        }
        move_sum += num_moves_dir(prev_button, Directions::Press, level - 1, dir_keyboard_moves, memory_dir);

        min_cost = std::min(min_cost, move_sum);
    }

    memory_num[hash] = min_cost;
    return min_cost;
};

int
main() {  // NOLINT
    constexpr int32_t NUM_LEVELS{3};
    const auto& lines{ utils::read_lines_from_file("day21.txt") };

    std::vector<std::array<int32_t, 4>> codes(lines.size());
    for (size_t i{ 0 }; i < lines.size(); ++i) {
        for (size_t j{ 0 }; j < lines[i].size(); ++j) {
            const auto letter = lines[i][j];
            if (letter == 'A') {
                codes[i][j] = 10;
            } else {
                codes[i][j] = letter - '0';
            }
        }
    }


    const auto dir_keyboard_moves = make_dir_keyboard_moves();
    const auto num_keyboard_moves = make_num_keyboard_moves();

    std::unordered_map<int32_t, int64_t> memory_dir{};
    std::unordered_map<int32_t, int64_t> memory_num{};

    int32_t result{ 0 };
    for (const auto& code : codes) {
        int32_t cost{ 0 };
        int32_t prev_num = 10;

        for (const auto& num : code) {
            cost += num_moves_num(
                prev_num,
                num,
                NUM_LEVELS,
                num_keyboard_moves,
                dir_keyboard_moves,
                memory_num,
                memory_dir
            );
            prev_num = num;
        }
        result += cost * (code[0] * 100 + code[1] * 10 + code[2]);
        std::println("{}: {}", code, cost);
    }

    std::println("{}", result);
}