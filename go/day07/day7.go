package day7

import (
	"fmt"
	"math"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

type Row struct {
	target int64
	nums   []int64
}

func CheckTarget(nums []int64, index int, currentSum int64, target int64) bool {
	if index == len(nums) {
		return currentSum == target
	}

	if currentSum > target {
		return false
	}

	if CheckTarget(nums, index+1, currentSum+nums[index], target) {
		return true
	}

	return CheckTarget(nums, index+1, max(currentSum, 0)*nums[index], target)
}

func CheckTargetB(nums []int64, index int, currentSum int64, target int64) bool {
	if index == len(nums) {
		return currentSum == target
	}

	if currentSum > target {
		return false
	}

	numDigits := int(math.Floor(math.Log10(float64(nums[index])))) + 1
	if CheckTargetB(nums, index+1, currentSum*int64(math.Pow10(numDigits))+nums[index], target) {
		return true
	}

	if CheckTargetB(nums, index+1, currentSum+nums[index], target) {
		return true
	}

	return CheckTargetB(nums, index+1, max(currentSum, 0)*nums[index], target)
}

func Day7a() {
	lines, err := utils.ReadLines("../inputs/day7.txt")
	if err != nil {
		panic("Could not read lines")
	}

	rows := make([]Row, len(lines))
	for i, line := range lines {
		parts := strings.Split(line, ": ")
		target, _ := strconv.Atoi(parts[0])
		numsStr := strings.Split(parts[1], " ")
		nums := make([]int64, len(numsStr))
		for j, numStr := range numsStr {
			num, _ := strconv.Atoi(numStr)
			nums[j] = int64(num)
		}
		row := Row{target: int64(target), nums: nums}
		rows[i] = row
	}

	sumGood := int64(0)
	for _, row := range rows {
		if CheckTarget(row.nums, 1, row.nums[0], row.target) {
			sumGood += row.target
		}
	}
	fmt.Printf("%v\n", sumGood)
}

func Day7b() {
	lines, err := utils.ReadLines("../inputs/day7.txt")
	if err != nil {
		panic("Could not read lines")
	}

	rows := make([]Row, len(lines))
	for i, line := range lines {
		parts := strings.Split(line, ": ")
		target, _ := strconv.Atoi(parts[0])
		numsStr := strings.Split(parts[1], " ")
		nums := make([]int64, len(numsStr))
		for j, numStr := range numsStr {
			num, _ := strconv.Atoi(numStr)
			nums[j] = int64(num)
		}
		row := Row{target: int64(target), nums: nums}
		rows[i] = row
	}

	sumGood := int64(0)
	for _, row := range rows {
		if CheckTargetB(row.nums, 1, row.nums[0], row.target) {
			sumGood += row.target
		}
	}
	fmt.Printf("%v\n", sumGood)
}
