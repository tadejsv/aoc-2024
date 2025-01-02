from dataclasses import dataclass


@dataclass
class Space:
    index: int
    start: int
    length: int


def day9a() -> None:
    row = ""
    with open("../inputs/day9.txt", "r") as file:
        for line in file:
            row = line.strip()

    spaces: list[Space] = []
    start = 0
    for i, chr in enumerate(row):
        index = i // 2 if i % 2 == 0 else -1
        spaces.append(Space(index, start, int(chr)))
        start += int(chr)

    es_ind = 1
    for i in range(len(spaces) - 1, -1, -2):
        space = spaces[i]
        while i > es_ind and space.length > 0:
            empty = spaces[es_ind]
            if empty.length > space.length:
                space.start = empty.start
                empty.start += space.length
                empty.length -= space.length
                break

            empty.index = space.index
            space.length -= empty.length
            es_ind += 2

    checksum = 0
    for space in spaces:
        if space.index > 0:
            for i in range(space.length):
                checksum += space.index * (space.start + i)

    print(checksum)


def day9b() -> None:
    row = ""
    with open("../inputs/day9.txt", "r") as file:
        for line in file:
            row = line.strip()

    files: list[Space] = []
    empties: list[Space] = []
    start = 0
    for i, chr in enumerate(row):
        if i % 2 == 0:
            files.append(Space(i // 2, start, int(chr)))
        else:
            empties.append(Space(-1, start, int(chr)))
        start += int(chr)

    for i in range(len(files) - 1, -1, -1):
        file = files[i]
        for j in range(len(empties)):
            empty = empties[j]
            if empty.start > file.start:
                break
            if empty.length < file.length:
                continue

            file.start = empty.start
            empty.start += file.length
            empty.length -= file.length

    checksum = 0
    for file in files:
        for i in range(file.length):
            checksum += file.index * (file.start + i)

    print(checksum)


day9b()
