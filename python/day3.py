import re


def day3a() -> None:
    string = ""
    with open("../inputs/day3.txt", "r") as file:
        for line in file:
            string += line

    pattern = r"mul\((\d{1,3}),(\d{1,3})\)"
    results = re.findall(pattern, string)

    sum_product = 0
    for res in results:
        sum_product += int(res[0]) * int(res[1])
        # print(type(res))
    print(sum_product)


def day3b() -> None:
    string = ""
    with open("../inputs/day3.txt", "r") as file:
        for line in file:
            string += line

    pattern = r"(mul\((\d{1,3}),(\d{1,3})\))|(do\(\))|(don't\(\))"
    results = re.findall(pattern, string)

    add = True
    sum_product = 0
    for res in results:
        if res[3] != "":
            add = True
            continue
        if res[4] != "":
            add = False
            continue

        if add:
            sum_product += int(res[1]) * int(res[2])

    print(sum_product)


day3b()
