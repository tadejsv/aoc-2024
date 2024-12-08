def check_line(line: list[int]) -> int:
    decreasing = line[0] > line[1]
    for i in range(len(line) - 1):
        if decreasing and line[i] < line[i + 1]:
            return i
        if not decreasing and line[i] > line[i + 1]:
            return i

        abs_diff = abs(line[i] - line[i + 1])
        if abs_diff < 1 or abs_diff > 3:
            return i

    return -1


def day2a() -> None:
    num_lines = []

    with open("../inputs/day2.txt", "r") as file:
        for line in file:
            parts = line.strip().split()
            num_lines.append([int(x) for x in parts])

    result = 0
    for line in num_lines:
        if check_line(line) == -1:
            result += 1

    print(result)

def day2b() -> None:
    num_lines = []

    with open("../inputs/day2.txt", "r") as file:
        for line in file:
            parts = line.strip().split()
            num_lines.append([int(x) for x in parts])

    result = 0
    for line in num_lines:
        mistake = check_line(line)
        if mistake == -1:
            result += 1
            continue

        for rem_ind in [mistake-1,mistake, mistake+1]:
            if rem_ind < 0:
                continue

            if check_line(line[:rem_ind] + line[rem_ind+1:]) == -1:
                result += 1
                break

    print(result)

day2b()
