package day11

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

const maxSteps = 100

func index(num, steps int64) int64 {
	return num*maxSteps + steps
}

func numDigits(num int64) int64 {
	digits := int64(0)
	for num > 0 {
		num = num / 10
		digits += 1
	}
	return digits
}

func pow10(pow int64) int64 {
	result := int64(1)
	for pow > 0 {
		result *= 10
		pow -= 1
	}
	return result
}

func expand(num, steps int64, cache map[int64]int64) int64 {
	if steps == 0 {
		return 1
	}

	if cacheNum, ok := cache[index(num, steps)]; ok {
		return cacheNum
	}

	digits := numDigits(num)
	result := int64(0)
	if num == 0 {
		result = expand(1, steps-1, cache)
	} else if digits%2 == 0 {
		num1 := num / pow10(digits/2)
		num2 := num % pow10(digits/2)
		result = expand(num1, steps-1, cache) + expand(num2, steps-1, cache)
	} else {
		result = expand(num*2024, steps-1, cache)
	}

	cache[index(num, steps)] = result
	return result
}

func Day11a() {
	lines, err := utils.ReadLines("../inputs/day11.txt")
	if err != nil {
		panic("Could not read lines")
	}

	numsStr := strings.Split(lines[0], " ")
	nums := make([]int64, len(numsStr))
	for i, numStr := range numsStr {
		num, _ := strconv.Atoi(numStr)
		nums[i] = int64(num)
	}

	cache := make(map[int64]int64)
	result := int64(0)
	for _, num := range nums {
		result += expand(num, 25, cache)
	}
	fmt.Printf("%v", result)
}

func Day11b() {
	lines, err := utils.ReadLines("../inputs/day11.txt")
	if err != nil {
		panic("Could not read lines")
	}

	numsStr := strings.Split(lines[0], " ")
	nums := make([]int64, len(numsStr))
	for i, numStr := range numsStr {
		num, _ := strconv.Atoi(numStr)
		nums[i] = int64(num)
	}

	cache := make(map[int64]int64)
	result := int64(0)
	for _, num := range nums {
		result += expand(num, 75, cache)
	}
	fmt.Printf("%v", result)
}
