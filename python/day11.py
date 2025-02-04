def num_digits(num: int) -> int:
    digits = 0
    while num > 0:
        num = num // 10
        digits += 1
    return digits


def pow_10(pow: int) -> int:
    result = 1
    while pow > 0:
        pow -= 1
        result *= 10
    return result


def expand(num: int, steps: int, cache: dict[tuple[int, int], int]) -> int:
    if steps == 0:
        return 1

    if (num, steps) in cache:
        return cache[(num, steps)]

    ret = 0
    if num == 0:
        return expand(1, steps - 1, cache)
    else:
        digits = num_digits(num)
        if digits % 2 == 0:
            pow10_half = pow_10(digits // 2)
            half1 = num // pow10_half
            half2 = num % pow10_half
            ret = expand(half1, steps - 1, cache) + expand(half2, steps - 1, cache)
        else:
            ret = expand(num * 2024, steps - 1, cache)

    cache[(num, steps)] = ret
    return ret


def day11a() -> None:
    with open("../inputs/day11.txt", "r") as file:
        for line in file:
            nums = tuple(int(x) for x in line.strip().split(" "))
            break

    result = 0
    cache = {}
    for num in nums:
        result += expand(num, 25, cache)

    print(result)


def day11b() -> None:
    with open("../inputs/day11.txt", "r") as file:
        for line in file:
            nums = tuple(int(x) for x in line.strip().split(" "))
            break

    result = 0
    cache = {}
    for num in nums:
        result += expand(num, 75, cache)

    print(result)


day11b()
