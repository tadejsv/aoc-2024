package day17

import (
	"fmt"
	"slices"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

func get_combo(val int, regA int, regB int, regC int) int {
	switch val {
	case 0, 1, 2, 3:
		return val
	case 4:
		return regA
	case 5:
		return regB
	case 6:
		return regC
	default:
		panic("unexpected value")
	}
}

type Computer struct {
	pos  int
	regA int
	regB int
	regC int
}

func (c *Computer) combo(literal int) int {
	switch literal {
	case 0, 1, 2, 3:
		return literal
	case 4:
		return c.regA
	case 5:
		return c.regB
	case 6:
		return c.regC
	default:
		panic("unexpected value")
	}
}

func (c *Computer) Run(operand int, literal int) (int, bool) {
	combo := c.combo(literal)
	switch operand {
	case 0:
		c.regA = c.regA / (1 << combo)
	case 1:
		c.regB = c.regB ^ literal
	case 2:
		c.regB = combo % 8
	case 3:
		if c.regA != 0 {
			c.pos = literal
			return 0, false
		}
	case 4:
		c.regB = c.regB ^ c.regC
	case 5:
		c.pos += 2
		return combo % 8, true
	case 6:
		c.regB = c.regA / (1 << combo)
	case 7:
		c.regC = c.regA / (1 << combo)
	}

	c.pos += 2
	return 0, false
}

type QueueItem struct {
	currentNumber int
	currentPos    int
}

func Day17a() {
	lines, err := utils.ReadLines("../inputs/day17.txt")
	if err != nil {
		panic("Could not read lines")
	}

	regA, _ := strconv.Atoi(strings.Split(lines[0], ": ")[1])
	regB, _ := strconv.Atoi(strings.Split(lines[1], ": ")[1])
	regC, _ := strconv.Atoi(strings.Split(lines[2], ": ")[1])

	nums_str := strings.Split(strings.Split(lines[4], ": ")[1], ",")
	nums := make([]int, len(nums_str))
	for i, num_str := range nums_str {
		num, _ := strconv.Atoi(num_str)
		nums[i] = num
	}

	computer := Computer{0, regA, regB, regC}

	for computer.pos < len(nums)-1 {
		literal := nums[computer.pos+1]
		output, ok := computer.Run(nums[computer.pos], literal)
		if ok {
			fmt.Printf("%v,", output)
		}
	}
}

func Day17b() {
	lines, err := utils.ReadLines("../inputs/day17.txt")
	if err != nil {
		panic("Could not read lines")
	}

	nums_str := strings.Split(strings.Split(lines[4], ": ")[1], ",")
	nums := make([]int, len(nums_str))
	for i, num_str := range nums_str {
		num, _ := strconv.Atoi(num_str)
		nums[i] = num
	}

	queue := []QueueItem{{0, 0}}
	goodNums := []int{}

	for len(queue) > 0 {
		item := queue[0]
		queue = queue[1:]

		if item.currentPos == len(nums) {
			goodNums = append(goodNums, item.currentNumber)
			continue
		}

		for num := range 8 {
			newNum := item.currentNumber<<3 + num
			computer := Computer{0, newNum, 0, 0}

			for {
				literal := nums[computer.pos+1]
				output, ok := computer.Run(nums[computer.pos], literal)
				if ok {
					if output == nums[len(nums)-1-item.currentPos] {
						newItem := QueueItem{newNum, item.currentPos + 1}
						queue = append(queue, newItem)
					}

					break
				}
			}
		}
	}

	fmt.Printf("%v\n", slices.Min(goodNums))
}
