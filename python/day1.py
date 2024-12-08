from collections import Counter


def day1a() -> None:
    col1, col2 = [], []

    with open("../inputs/day1.txt", "r") as file:
        for line in file:
            parts = line.strip().split()
            col1.append(int(parts[0]))
            col2.append(int(parts[-1]))

    col1.sort()
    col2.sort()

    sum = 0
    for i in range(len(col1)):
        sum += abs(col1[i] - col2[i])

    print(sum)


def day1b() -> None:
    col1 = []
    col2_counts = Counter()

    with open("../inputs/day1.txt", "r") as file:
        for line in file:
            parts = line.strip().split()
            col1.append(int(parts[0]))
            col2_counts.update([int(parts[-1])])

    sum = 0
    for num in col1:
        sum += num * col2_counts[num]

    print(sum)


day1b()
