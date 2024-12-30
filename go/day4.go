package main

import "fmt"

func checkXmas(x, y, dx, dy int, lines []string) bool {
	maxX, maxY := x+3*dx, y+3*dy
	if maxX < 0 || maxX >= len(lines[0]) {
		return false
	}
	if maxY < 0 || maxY >= len(lines) {
		return false
	}

	for i, char := range "XMAS" {
		if lines[y+i*dy][x+i*dx] != byte(char) {
			return false
		}
	}

	return true
}

func checkMas(x, y, dx1, dy1, dx2, dy2 int, lines []string) bool {
	if x-1 < 0 || x+1 >= len(lines[0]) {
		return false
	}
	if y-1 < 0 || y+1 >= len(lines) {
		return false
	}

	if lines[y][x] != 'A' {
		return false
	}
	if lines[y+dy1][x+dx1] != 'M' || lines[y+dy2][x+dx2] != 'M' {
		return false
	}
	if lines[y-dy1][x-dx1] != 'S' || lines[y-dy2][x-dx2] != 'S' {
		return false
	}

	return true
}

func Day4a() {
	lines, err := ReadLines("../inputs/day4.txt")
	if err != nil {
		panic("Could not read lines")
	}

	dirs := [][2]int{
		{0, 1},
		{0, -1},
		{1, 1},
		{1, 0},
		{1, -1},
		{-1, 1},
		{-1, 0},
		{-1, -1},
	}

	num_xmas := 0
	for i, line := range lines {
		for j := range line {
			for _, dir := range dirs {
				if checkXmas(j, i, dir[0], dir[1], lines) {
					num_xmas += 1
				}
			}
		}
	}

	fmt.Printf("Num XMAS: %v\n", num_xmas)
}

func Day4b() {
	lines, err := ReadLines("../inputs/day4.txt")
	if err != nil {
		panic("Could not read lines")
	}

	dirs := [][4]int{
		{1, -1, -1, -1},
		{-1, -1, -1, 1},
		{-1, 1, 1, 1},
		{1, 1, 1, -1},
	}

	num_xmas := 0
	for i, line := range lines {
		for j := range line {
			for _, dir := range dirs {
				if checkMas(j, i, dir[0], dir[1], dir[2], dir[3], lines) {
					num_xmas += 1
				}
			}
		}
	}

	fmt.Printf("Num X-MAS: %v\n", num_xmas)
}
