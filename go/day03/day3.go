package day3

import (
	"fmt"
	"regexp"
	"strconv"

	"github.com/tadejsv/aoc-2024/utils"
)

func Day3a() {
	lines, err := utils.ReadLines("../inputs/day3.txt")
	if err != nil {
		panic("Could not read lines")
	}

	str := ""
	for _, line := range lines {
		str += line
	}

	pattern := `mul\((\d{1,3}),(\d{1,3})\)`
	re := regexp.MustCompile(pattern)
	matches := re.FindAllStringSubmatch(str, -1)

	sum := 0
	for _, m := range matches {
		num1, _ := strconv.Atoi(m[1])
		num2, _ := strconv.Atoi(m[2])
		sum += num1 * num2
	}
	fmt.Printf("%v\n", sum)
}

func Day3b() {
	lines, err := utils.ReadLines("../inputs/day3.txt")
	if err != nil {
		panic("Could not read lines")
	}

	str := ""
	for _, line := range lines {
		str += line
	}

	pattern := `(mul\((\d{1,3}),(\d{1,3})\))|(do\(\))|(don't\(\))`
	re := regexp.MustCompile(pattern)
	matches := re.FindAllStringSubmatch(str, -1)

	sum := 0
	add := true
	for _, m := range matches {
		if m[4] != "" {
			add = true
			continue
		}
		if m[5] != "" {
			add = false
			continue
		}
		if add {
			num1, _ := strconv.Atoi(m[2])
			num2, _ := strconv.Atoi(m[3])
			sum += num1 * num2
		}
	}
	fmt.Printf("%v\n", sum)
}
