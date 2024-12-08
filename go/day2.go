package main

import (
	"fmt"
	"math"
	"strconv"
	"strings"
)

func checkLine(numLine []int) int {
	decrasing := numLine[0] > numLine[1]
	for i := range len(numLine) - 1 {
		if decrasing && numLine[i] < numLine[i+1] {
			return i
		}
		if !decrasing && numLine[i] > numLine[i+1] {
			return i
		}

		absDiff := math.Abs(float64(numLine[i] - numLine[i+1]))
		if absDiff > 3 || absDiff < 1 {
			return i
		}
	}
	return -1
}

func Day2a() {
	// read lines
	lines, err := ReadLines("../inputs/day2.txt")
	if err != nil {
		panic("Could not read lines")
	}

	// convert all numbers to integers
	numLines := make([][]int, 0)
	for _, line := range lines {
		nums := []int{}
		for _, numStr := range strings.Split(line, " ") {
			num, err := strconv.Atoi(numStr)
			if err != nil {
				panic("could not conver number")
			}
			nums = append(nums, num)
		}
		numLines = append(numLines, nums)
	}

	// Check
	count := 0
	for _, numLine := range numLines {
		mistake := checkLine(numLine)
		if mistake == -1 {
			count += 1
		}
	}

	fmt.Printf("%v\n", count)
}

func Day2b() {
	// read lines
	lines, err := ReadLines("../inputs/day2.txt")
	if err != nil {
		panic("Could not read lines")
	}

	// convert all numbers to integers
	numLines := make([][]int, 0)
	for _, line := range lines {
		nums := []int{}
		for _, numStr := range strings.Split(line, " ") {
			num, err := strconv.Atoi(numStr)
			if err != nil {
				panic("could not conver number")
			}
			nums = append(nums, num)
		}
		numLines = append(numLines, nums)
	}

	// Check
	count := 0
	for _, numLine := range numLines {
		mistake := checkLine(numLine)
		if mistake == -1 {
			count += 1
			continue
		}

		for _, removeInd := range []int{mistake - 1, mistake, mistake + 1} {
			if removeInd < 0 {
				continue
			}
			newLine := make([]int, len(numLine)-1)
			copy(newLine[:removeInd], numLine[:removeInd])
			copy(newLine[removeInd:], numLine[removeInd+1:])

			if checkLine(newLine) == -1 {
				count += 1
				break
			}
		}
	}

	fmt.Printf("%v\n", count)
}
