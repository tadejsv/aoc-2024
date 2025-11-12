package day18

import (
	"fmt"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

func canConstruct(pattern string, towelMap map[byte][]string, cache map[string]bool) bool {
	if len(pattern) == 0 {
		return true
	}

	if cacheResult, ok := cache[pattern]; ok {
		return cacheResult
	}

	result := false
	for _, towel := range towelMap[pattern[0]] {
		if len(towel) <= len(pattern) && towel == pattern[:len(towel)] {
			if canConstruct(pattern[len(towel):], towelMap, cache) {
				result = true
				break
			}
		}
	}

	cache[pattern] = result
	return result
}

func countConstruct(pattern string, towelMap map[byte][]string, cache map[string]int) int {
	if len(pattern) == 0 {
		return 1
	}

	if cacheResult, ok := cache[pattern]; ok {
		return cacheResult
	}

	result := 0
	for _, towel := range towelMap[pattern[0]] {
		if len(towel) <= len(pattern) && towel == pattern[:len(towel)] {
			result += countConstruct(pattern[len(towel):], towelMap, cache)
		}
	}

	cache[pattern] = result
	return result
}

func Day19a() {
	lines, err := utils.ReadLines("../inputs/day19.txt")
	if err != nil {
		panic("Could not read lines")
	}

	towels := strings.Split(lines[0], ", ")
	towelsMap := make(map[byte][]string)
	for _, towel := range towels {
		towelsMap[towel[0]] = append(towelsMap[towel[0]], towel)
	}

	patterns := lines[2:]
	goodPatterns := 0
	cache := map[string]bool{}

	for _, pattern := range patterns {
		if canConstruct(pattern, towelsMap, cache) {
			goodPatterns += 1
		}
	}

	fmt.Printf("%v\n", goodPatterns)
}

func Day19b() {
	lines, err := utils.ReadLines("../inputs/day19.txt")
	if err != nil {
		panic("Could not read lines")
	}

	towels := strings.Split(lines[0], ", ")
	towelsMap := make(map[byte][]string)
	for _, towel := range towels {
		towelsMap[towel[0]] = append(towelsMap[towel[0]], towel)
	}

	patterns := lines[2:]
	countPatterns := 0
	cache := map[string]int{}

	for _, pattern := range patterns {
		countPatterns += countConstruct(pattern, towelsMap, cache)
	}

	fmt.Printf("%v\n", countPatterns)
}
