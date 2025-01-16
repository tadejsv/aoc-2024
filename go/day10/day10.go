package day10

import (
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

func cellIndex(row, col, nCols int) int {
	return row*nCols + col
}

func Day10a() {
	lines, err := utils.ReadLines("../inputs/day10.txt")
	if err != nil {
		panic("Could not read lines")
	}

	trailheads := make([][2]int, 0)
	heights := make([][]int, len(lines))
	for i, line := range lines {
		lineHeights := make([]int, len(line))
		for j, chr := range line {
			lineHeights[j] = int(chr - '0')
			if lineHeights[j] == 0 {
				trailheads = append(trailheads, [2]int{i, j})
			}
		}
		heights[i] = lineHeights
	}

	nRows, nCols := len(heights), len(heights[0])
	neighborOffsets := [4][2]int{{0, 1}, {1, 0}, {-1, 0}, {0, -1}}
	sumScores := 0
	for _, trailhead := range trailheads {
		visited := make(map[int]bool)
		queue := make([][2]int, 0)
		queue = append(queue, trailhead)

		for len(queue) > 0 {
			current := queue[0]
			currentHeight := heights[current[0]][current[1]]
			queue = queue[1:]
			if heights[current[0]][current[1]] == 9 {
				sumScores += 1
				continue
			}

			for _, offset := range neighborOffsets {
				neighbor := [2]int{current[0] + offset[0], current[1] + offset[1]}
				neighborIndex := cellIndex(neighbor[0], neighbor[1], nCols)
				if neighbor[0] < 0 || neighbor[1] < 0 || neighbor[0] >= nRows || neighbor[1] >= nCols {
					continue
				}
				if heights[neighbor[0]][neighbor[1]] != currentHeight+1 {
					continue
				}
				if _, ok := visited[neighborIndex]; ok {
					continue
				}
				visited[neighborIndex] = true
				queue = append(queue, neighbor)
			}
		}
	}

	fmt.Printf("%v\n", sumScores)
}

func Day10b() {
	lines, err := utils.ReadLines("../inputs/day10.txt")
	if err != nil {
		panic("Could not read lines")
	}

	trailheads := make([][2]int, 0)
	heights := make([][]int, len(lines))
	for i, line := range lines {
		lineHeights := make([]int, len(line))
		for j, chr := range line {
			lineHeights[j] = int(chr - '0')
			if lineHeights[j] == 0 {
				trailheads = append(trailheads, [2]int{i, j})
			}
		}
		heights[i] = lineHeights
	}

	nRows, nCols := len(heights), len(heights[0])
	neighborOffsets := [4][2]int{{0, 1}, {1, 0}, {-1, 0}, {0, -1}}
	sumScores := 0
	for _, trailhead := range trailheads {
		queue := make([][2]int, 0)
		queue = append(queue, trailhead)

		for len(queue) > 0 {
			current := queue[0]
			currentHeight := heights[current[0]][current[1]]
			queue = queue[1:]
			if heights[current[0]][current[1]] == 9 {
				sumScores += 1
				continue
			}

			for _, offset := range neighborOffsets {
				neighbor := [2]int{current[0] + offset[0], current[1] + offset[1]}
				if neighbor[0] < 0 || neighbor[1] < 0 || neighbor[0] >= nRows || neighbor[1] >= nCols {
					continue
				}
				if heights[neighbor[0]][neighbor[1]] != currentHeight+1 {
					continue
				}
				queue = append(queue, neighbor)
			}
		}
	}

	fmt.Printf("%v\n", sumScores)
}
