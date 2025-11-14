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

enum class Directions : uint8_t { Up, Down, Left, Right, Press };

using MoveSeq = std::vector<Directions>;
template <class K>
using MoveMap = std::unordered_map<K, std::vector<MoveSeq>, PairHash>;

constexpr std::array<std::pair<Directions, Coords>, 4> dirs{ {
    { Directions::Up, { -1, 0 } },
    { Directions::Down, { 1, 0 } },
    { Directions::Left, { 0, -1 } },
    { Directions::Right, { 0, 1 } },
} };

auto
make_dir_keyboard_moves() -> MoveMap<std::pair<Directions, Directions>> {
    using D = Directions;
    const auto U = D::Up;
    const auto Dn = D::Down;
    const auto L = D::Left;
    const auto R = D::Right;
    const auto A = D::Press;

    return {
        // From Up
        { { U, U }, { {} } },
        { { U, Dn }, { { Dn } } },
        { { U, L }, { { Dn, L } } },
        { { U, R }, { { Dn, R }, { R, Dn } } },
        { { U, A }, { { R } } },

        // From Press
        { { A, U }, { { L } } },
        { { A, A }, { {} } },
        { { A, L }, { { Dn, L, L }, { L, Dn, L } } },
        { { A, Dn }, { { Dn, L }, { L, Dn } } },
        { { A, R }, { { Dn } } },

        // From Left
        { { L, U }, { { R, U } } },
        { { L, A }, { { R, U, R }, { R, R, U } } },
        { { L, L }, { {} } },
        { { L, Dn }, { { R } } },
        { { L, R }, { { R, R } } },

        // From Down
        { { Dn, U }, { { U } } },
        { { Dn, A }, { { U, R }, { R, U } } },
        { { Dn, L }, { { L } } },
        { { Dn, Dn }, { {} } },
        { { Dn, R }, { { R } } },

        // From Right
        { { R, U }, { { U, L }, { L, U } } },
        { { R, A }, { { U } } },
        { { R, L }, { { L, L } } },
        { { R, Dn }, { { L } } },
        { { R, R }, { {} } },
    };
}

auto
make_num_keyboard_moves() -> MoveMap<std::pair<int32_t, int32_t>> {
    // num_positions
    std::unordered_map<int32_t, Coords> num_positions{
        { 0, { 3, 1 } }, { 1, { 2, 0 } }, { 2, { 2, 1 } }, { 3, { 2, 2 } },
        { 4, { 1, 0 } }, { 5, { 1, 1 } }, { 6, { 1, 2 } }, { 7, { 0, 0 } },
        { 8, { 0, 1 } }, { 9, { 0, 2 } }, { 10, { 3, 2 } }  // 'A' / Press
    };

    // positions_num (reverse)
    std::unordered_map<Coords, int32_t, PairHash> positions_num;
    for (const auto& [n, pos] : num_positions) {
        positions_num[pos] = n;
    }

    MoveMap<std::pair<int32_t, int32_t>> num_keyboard_moves;

    struct Node {
        Coords pos;
        MoveSeq moves;
    };

    for (int32_t from = 0; from <= 10; ++from) {
        std::unordered_map<int32_t, int32_t> best;
        std::deque<Node> q{ Node{ num_positions[from], {} } };

        while (!q.empty()) {
            auto [pos, moves] = std::move(q.front());
            q.pop_front();

            const int32_t to = positions_num[pos];
            const int32_t dist = static_cast<int32_t>(moves.size());

            if (auto it = best.find(to); it != best.end() && it->second < dist) {
                continue;
            }
            best[to] = dist;
            num_keyboard_moves[{ from, to }].push_back(moves);

            for (const auto& [dir, delta] : dirs) {
                Coords np{ pos.first + delta.first, pos.second + delta.second };
                if (np.first < 0 || np.second < 0 || np.first > 3 || np.second > 2
                    || (np.first == 3 && np.second == 0)) {
                    continue;
                }
                auto nm = moves;
                nm.push_back(dir);
                q.push_back(Node{ .pos=np, .moves=std::move(nm) });
            }
        }
    }

    return num_keyboard_moves;
}

auto
hash_query_dir(Directions from, Directions to, int32_t level) -> int32_t {
    return level * 25 + static_cast<int32_t>(to) * 5 + static_cast<int32_t>(from);
}

auto
hash_query_num(int32_t from, int32_t to) -> int32_t {
    return to * 11 + from;
};

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
    constexpr int32_t NUM_LEVELS{ 26 };
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

    int64_t result{ 0 };
    for (const auto& code : codes) {
        int64_t cost{ 0 };
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
        result += cost * static_cast<int64_t>((code[0] * 100 + code[1] * 10 + code[2]));
    }

    std::println("{}", result);
}