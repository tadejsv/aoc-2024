package day9

import (
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

type Space struct {
	start, len, index int64
}

func Day9a() {
	lines, err := utils.ReadLines("../inputs/day9.txt")
	if err != nil {
		panic("Could not read lines")
	}

	spaces := make([]Space, len(lines[0]))
	start := int64(0)
	for i, chr := range lines[0] {
		index := -1
		if i%2 == 0 {
			index = i / 2
		}
		spaces[i] = Space{start, int64(chr - '0'), int64(index)}
		start += int64(chr - '0')
	}

	firstEmptyInd := 1
	for i := len(spaces) - 1; i > firstEmptyInd; i -= 2 {
		for i > firstEmptyInd && spaces[i].len > 0 {
			if spaces[firstEmptyInd].len > spaces[i].len {
				spaces[i].start = spaces[firstEmptyInd].start
				spaces[firstEmptyInd].len -= spaces[i].len
				spaces[firstEmptyInd].start += spaces[i].len
				break
			}
			spaces[firstEmptyInd].index = spaces[i].index
			spaces[i].len -= spaces[firstEmptyInd].len
			firstEmptyInd += 2
		}
	}

	sum := int64(0)
	for _, space := range spaces {
		for i := range space.len {
			sum += (space.start + i) * max(0, space.index)
		}
	}

	fmt.Printf("%v\n", sum)
}

func Day9b() {
	lines, err := utils.ReadLines("../inputs/day9.txt")
	if err != nil {
		panic("Could not read lines")
	}

	fileSpaces := make([]Space, 0, len(lines[0])/2+1)
	emptySpaces := make([]Space, 0, len(lines[0])/2)

	start := int64(0)
	for i, chr := range lines[0] {
		if i%2 == 0 {
			fileSpaces = append(fileSpaces, Space{start, int64(chr - '0'), int64(i / 2)})
		} else {
			emptySpaces = append(emptySpaces, Space{start, int64(chr - '0'), -1})
		}
		start += int64(chr - '0')
	}

	for i := len(fileSpaces) - 1; i >= 0; i-- {
		file := &fileSpaces[i]
		for j := 0; j < len(emptySpaces) && file.start > emptySpaces[j].start; j++ {
			if emptySpaces[j].len >= file.len {
				empty := &emptySpaces[j]
				file.start = empty.start
				empty.len -= file.len
				empty.start += file.len
				break
			}
		}
	}

	sum := int64(0)
	for _, file := range fileSpaces {
		for i := range file.len {
			sum += (file.start + i) * file.index
		}
	}

	fmt.Printf("%v\n", sum)
}
