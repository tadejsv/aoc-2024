package day12

import (
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

var neighborOffsets = [4][2]int{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}
var surroundOffsets = [8][2]int{{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
var corners = [4][2]int{{2, 2}, {2, 0}, {0, 2}, {0, 0}}

func isOOB(row, col, nRows, nCols int) bool {
	return row < 0 || col < 0 || row >= nRows || col >= nCols
}

func exploreRegion(row, col int, grid []string, visited [][]bool) (int, int) {
	queue := [][2]int{{row, col}}
	nRows, nCols := len(grid), len(grid[0])
	regionType := grid[row][col]
	visited[row][col] = true

	area, perimeter := 0, 0
	for len(queue) > 0 {
		area += 1
		current := queue[0]
		queue = queue[1:]

		for _, nOffset := range neighborOffsets {
			row, col := current[0]+nOffset[0], current[1]+nOffset[1]
			if isOOB(row, col, nRows, nCols) {
				perimeter += 1
				continue
			}
			if grid[row][col] != regionType {
				perimeter += 1
				continue
			}
			if visited[row][col] {
				continue
			}
			queue = append(queue, [2]int{row, col})
			visited[row][col] = true
		}
	}
	return area, perimeter
}

func exploreRegionB(row, col int, grid []string, visited [][]bool) (int, int) {
	queue := [][2]int{{row, col}}
	nRows, nCols := len(grid), len(grid[0])
	regionType := grid[row][col]
	visited[row][col] = true

	area, sides := 0, 0
	for len(queue) > 0 {
		area += 1
		current := queue[0]
		queue = queue[1:]

		surrounding := [3][3]bool{}
		for _, offset := range surroundOffsets {
			var isSame bool
			if isOOB(current[0]+offset[0], current[1]+offset[1], nRows, nCols) {
				isSame = false
			} else {
				isSame = grid[current[0]+offset[0]][current[1]+offset[1]] == regionType
			}
			surrounding[offset[0]+1][offset[1]+1] = isSame
		}

		for _, corner := range corners {
			// check inner corner
			if !surrounding[corner[0]][1] && !surrounding[1][corner[1]] {
				sides += 1
			}

			// check outer corner
			if surrounding[corner[0]][1] && surrounding[1][corner[1]] && !surrounding[corner[0]][corner[1]] {
				sides += 1
			}
		}
		for _, nOffset := range neighborOffsets {
			row, col := current[0]+nOffset[0], current[1]+nOffset[1]
			if isOOB(row, col, nRows, nCols) || grid[row][col] != regionType || visited[row][col] {
				continue
			}
			queue = append(queue, [2]int{row, col})
			visited[row][col] = true
		}
	}
	return area, sides
}

func Day12a() {
	grid, err := utils.ReadLines("../inputs/day12.txt")
	if err != nil {
		panic("Could not read lines")
	}

	visited := make([][]bool, len(grid))
	for row := range visited {
		visited[row] = make([]bool, len(grid[0]))
	}

	total := 0
	for i := range grid {
		for j := range grid[i] {
			if !visited[i][j] {
				area, perimeter := exploreRegion(i, j, grid, visited)
				total += area * perimeter
			}
		}
	}

	fmt.Printf("%v", total)
}

func Day12b() {
	grid, err := utils.ReadLines("../inputs/day12.txt")
	if err != nil {
		panic("Could not read lines")
	}

	visited := make([][]bool, len(grid))
	for row := range visited {
		visited[row] = make([]bool, len(grid[0]))
	}

	total := 0
	for i := range grid {
		for j := range grid[i] {
			if !visited[i][j] {
				area, sides := exploreRegionB(i, j, grid, visited)
				total += area * sides
			}
		}
	}

	fmt.Printf("%v", total)
}
