package day1

import (
	"fmt"
	"math"
	"sort"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

func Day1a() {
	lines, err := utils.ReadLines("../inputs/day1.txt")
	if err != nil {
		panic("Could not read lines")
	}

	col1, col2 := make([]int, 0), make([]int, 0)
	for _, line := range lines {
		parts := strings.Split(line, " ")
		n1, err := strconv.Atoi(parts[0])
		if err != nil {
			panic("Could not convert number 1")
		}
		n2, err := strconv.Atoi(parts[len(parts)-1])
		if err != nil {
			panic("Could not convert number 2")
		}

		col1 = append(col1, n1)
		col2 = append(col2, n2)
	}

	sort.Ints(col1)
	sort.Ints(col2)

	sum := 0
	for i := range col1 {
		sum += int(math.Abs(float64(col1[i] - col2[i])))
	}
	fmt.Println(sum)
}

func Day1b() {
	lines, err := utils.ReadLines("../inputs/day1.txt")
	if err != nil {
		panic("Could not read lines")
	}

	col1 := make([]int, 0)
	col2Map := make(map[int]int)
	for _, line := range lines {
		parts := strings.Split(line, " ")
		n1, err := strconv.Atoi(parts[0])
		if err != nil {
			panic("Could not convert number 1")
		}
		n2, err := strconv.Atoi(parts[len(parts)-1])
		if err != nil {
			panic("Could not convert number 2")
		}

		col1 = append(col1, n1)
		col2Map[n2] += 1
	}

	sum := 0
	for _, num := range col1 {
		sum += col2Map[num] * num
	}
	fmt.Println(sum)
}
