import math


def check_target(nums: list[int], index: int, current_sum: int, target: int) -> bool:
    if index == len(nums):
        return current_sum == target

    if current_sum > target:
        return False

    if check_target(nums, index + 1, max(1, current_sum) * nums[index], target):
        return True

    return check_target(nums, index + 1, current_sum + nums[index], target)


def check_target_b(nums: list[int], index: int, current_sum: int, target: int) -> bool:
    if index == len(nums):
        return current_sum == target

    if current_sum > target:
        return False

    if index > 0 and check_target_b(
        nums,
        index + 1,
        current_sum * 10 ** (int(math.log10(abs(nums[index]))) + 1) + nums[index],
        target,
    ):
        return True

    if check_target_b(nums, index + 1, max(1, current_sum) * nums[index], target):
        return True

    return check_target_b(nums, index + 1, current_sum + nums[index], target)


def day7a() -> None:
    with open("../inputs/day7.txt", "r") as file:
        rows = []
        for line in file.readlines():
            parts = line.split(": ")
            target = int(parts[0])
            nums_str = parts[1].split(" ")
            nums = [int(ns) for ns in nums_str]
            rows.append((target, nums))

    sum_good = 0
    for row in rows:
        target, nums = row
        if check_target(nums, 0, 0, target):
            sum_good += target

    print(sum_good)


def day7b() -> None:
    with open("../inputs/day7.txt", "r") as file:
        rows = []
        for line in file.readlines():
            parts = line.split(": ")
            target = int(parts[0])
            nums_str = parts[1].split(" ")
            nums = [int(ns) for ns in nums_str]
            rows.append((target, nums))

    sum_good = 0
    for row in rows:
        target, nums = row
        if check_target_b(nums, 0, 0, target):
            sum_good += target

    print(sum_good)


day7b()
