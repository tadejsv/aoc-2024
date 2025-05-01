#include <cstddef>
#include <cstdint>
#include <deque>
#include <print>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "utils/utils.h"

using Coords = std::pair<int32_t, int32_t>;

auto
hash_coord(const Coords& coords, const int32_t width) -> int32_t {
    return coords.first * width + coords.second;
};

auto
unhash_coord(const int32_t hashed_coords, const int32_t width) -> Coords {
    return Coords{ hashed_coords / width, hashed_coords % width };
};

int
main() {  // NOLINT
    const auto& lines{ utils::read_lines_from_file("day15.txt") };

    const int32_t width{ static_cast<int32_t>(2 * lines[0].size()) };
    std::unordered_set<int32_t> boxes{};
    std::unordered_set<int32_t> walls{};
    Coords robot{ -1, -1 };

    size_t curr_line{ 0 };
    while (true) {
        if (lines[curr_line].size() == 0) {
            curr_line++;
            break;
        }

        const auto& line{ lines[curr_line] };
        for (size_t i{ 0 }; i < line.size(); i++) {
            switch (line[i]) {
                case '#':
                    walls.insert(hash_coord(Coords{ curr_line, 2 * i }, width));
                    walls.insert(hash_coord(Coords{ curr_line, 2 * i + 1 }, width));
                    break;
                case 'O':
                    boxes.insert(hash_coord(Coords{ curr_line, 2 * i }, width));
                    break;
                case '@':
                    robot = Coords{ curr_line, 2 * i };
                    break;
                default:
                    break;
            }
        }

        curr_line++;
    }

    std::vector<Coords> moves{};
    while (curr_line < lines.size()) {
        for (const auto& move : lines[curr_line]) {
            switch (move) {
                case '^':
                    moves.emplace_back(-1, 0);
                    break;
                case 'v':
                    moves.emplace_back(1, 0);
                    break;
                case '>':
                    moves.emplace_back(0, 1);
                    break;
                case '<':
                    moves.emplace_back(0, -1);
                    break;
                default:
                    break;
            }
        }
        ++curr_line;
    }


    for (const auto& move : moves) {
        bool can_move{ true };
        std::vector<Coords> collected_boxes{};
        std::deque<Coords> check{ { robot.first + move.first, robot.second + move.second } };
        while (check.size() > 0) {
            const auto pos{ check.front() };
            const Coords pos_left{ pos.first, pos.second - 1 };
            const auto pos_hash{ hash_coord(pos, width) };
            const auto pos_left_hash{ hash_coord(pos_left, width) };
            check.pop_front();
            if (walls.contains(pos_hash)) {
                can_move = false;
                break;
            }
            if (boxes.contains(pos_hash) || boxes.contains(pos_left_hash)) {
                auto box_got{ pos };
                if (boxes.contains(pos_left_hash)) {
                    box_got = pos_left;
                }

                collected_boxes.push_back(box_got);
                if (move.first != 0) {
                    check.emplace_back(box_got.first + move.first, box_got.second);
                    check.emplace_back(box_got.first + move.first, box_got.second + 1);
                } else {
                    const int32_t step = (move.second == 1) ? 2 : -1;
                    check.emplace_back(box_got.first, box_got.second + step);
                }
            }
        }


        if (can_move) {
            for (const auto& box : collected_boxes) {
                boxes.erase(hash_coord(box, width));
            }
            for (const auto& box : collected_boxes) {
                boxes.emplace(hash_coord({ box.first + move.first, box.second + move.second }, width));
            }
            robot = { robot.first + move.first, robot.second + move.second };
        }
    }
    int32_t final_sum{ 0 };
    const int32_t heigh_mul{ 100 };
    for (const auto& box : boxes) {
        const auto box_pos{ unhash_coord(box, width) };
        final_sum += heigh_mul * box_pos.first + box_pos.second;
    }
    std::println("{}", final_sum);
}
