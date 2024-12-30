import graphlib
from collections import defaultdict


def read_input(
    fname: str,
) -> tuple[dict[int, set[int]], list[list[int]]]:
    outgoing = defaultdict(set)
    updates = []
    with open(fname, "r") as file:
        updates_part = False
        for line in file:
            if line == "\n":
                # Start reading the "updates" part after the empty line
                updates_part = True
                continue

            if updates_part:
                parts = line.strip().split(",")
                updates.append([int(x) for x in parts])
            else:
                parts = line.strip().split("|")
                outgoing[int(parts[0])].add(int(parts[1]))

    return outgoing, updates


def day5a() -> None:
    outgoing, updates = read_input("../inputs/day5.txt")

    sum_good = 0
    for update in updates:
        seen = set()
        good = True
        for num in update:
            if seen.intersection(outgoing[num]):
                good = False
                break
            seen.add(num)

        if good:
            sum_good += update[len(update) // 2]

    print(sum_good)


def test_bad(update: list[int], outgoing: dict[int, set[int]]) -> int:
    update_set = set(update)

    outgoing_update = dict()
    for num in update:
        outgoing_update[num] = outgoing[num].intersection(update_set)

    toposort = graphlib.TopologicalSorter(outgoing_update)
    try:
        order = list(toposort.static_order())
        return order[len(order) // 2]
    except graphlib.CycleError:
        return 0


def day5b() -> None:
    outgoing, updates = read_input("../inputs/day5.txt")

    sum_bad = 0
    for update in updates:
        seen = set()
        for num in update:
            if seen.intersection(outgoing[num]):
                sum_bad += test_bad(update, outgoing)
                break
            seen.add(num)

    print(sum_bad)


day5b()
