package day8

import (
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

func hashCell(y, x, nCols int) int {
	return y*nCols + x
}

func isOOB(y, x, nRows, nCols int) bool {
	return y < 0 || x < 0 || y >= nRows || x >= nCols
}

func addAntiNode(antiNodes map[int]bool, y, x, nRows, nCols int) {
	if !isOOB(y, x, nRows, nCols) {
		antiNodes[hashCell(y, x, nCols)] = true
	}
}

func gcd(x, y int) int {
	if y == 0 {
		return x
	}
	return gcd(y, x%y)
}

func addAntiNodesLine(antiNodes map[int]bool, y, x, dy, dx, nRows, nCols int) {
	for i := 0; isOOB(y+i*dy, x+i*dx, nRows, nCols); i++ {
		antiNodes[hashCell(y+i*dy, x+i*dx, nCols)] = true
	}
	for i := 0; isOOB(y+i*dy, x+i*dx, nRows, nCols); i-- {
		antiNodes[hashCell(y+i*dy, x+i*dx, nCols)] = true
	}
}

func Day8a() {
	lines, err := utils.ReadLines("../inputs/day8.txt")
	if err != nil {
		panic("Could not read lines")
	}

	nRows, nCols := len(lines), len(lines[0])
	antennas := make(map[rune][][2]int)
	for i, line := range lines {
		for j, chr := range line {
			if chr != '.' {
				antennas[chr] = append(antennas[chr], [2]int{i, j})
			}
		}
	}

	antinodes := make(map[int]bool)
	for _, ants := range antennas {
		for i := range ants {
			for j := i + 1; j < len(ants); j++ {
				dy := ants[i][0] - ants[j][0]
				dx := ants[i][1] - ants[j][1]
				addAntiNode(antinodes, ants[i][0]+dy, ants[i][1]+dx, nRows, nCols)
				addAntiNode(antinodes, ants[j][0]-dy, ants[j][1]-dx, nRows, nCols)
			}
		}
	}
	fmt.Printf("%v\n", len(antinodes))
}

func Day8b() {
	lines, err := utils.ReadLines("../inputs/day8.txt")
	if err != nil {
		panic("Could not read lines")
	}

	nRows, nCols := len(lines), len(lines[0])
	antennas := make(map[rune][][2]int)
	for i, line := range lines {
		for j, chr := range line {
			if chr != '.' {
				antennas[chr] = append(antennas[chr], [2]int{i, j})
			}
		}
	}

	antinodes := make(map[int]bool)
	for _, ants := range antennas {
		for i := range ants {
			for j := i + 1; j < len(ants); j++ {
				dy := ants[i][0] - ants[j][0]
				dx := ants[i][1] - ants[j][1]
				gcd := gcd(dy, dx)
				dy, dx = dy/gcd, dx/gcd

				addAntiNodesLine(antinodes, ants[i][0], ants[i][1], dy, dx, nRows, nCols)
			}
		}
	}
	fmt.Printf("%v\n", len(antinodes))
}
