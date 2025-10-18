package day14

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

const width = 101
const height = 103
const periods = 1000

type Robot struct {
	position [2]int
	velocity [2]int
}

func Day14a() {
	lines, err := utils.ReadLines("../inputs/day14.txt")
	if err != nil {
		panic("Could not read lines")
	}

	robots := make([]Robot, len(lines))
	for i, line := range lines {
		lineParts := strings.Split(line, " ")
		posParts := strings.Split(strings.Split(lineParts[0], "=")[1], ",")
		velParts := strings.Split(strings.Split(lineParts[1], "=")[1], ",")

		px, _ := strconv.Atoi(posParts[0])
		py, _ := strconv.Atoi(posParts[1])
		vx, _ := strconv.Atoi(velParts[0])
		vy, _ := strconv.Atoi(velParts[1])

		robots[i].position = [2]int{px, py}
		robots[i].velocity = [2]int{vx, vy}
	}

	var quadrants [4]int
	for i := range robots {
		px := robots[i].position[0]
		py := robots[i].position[1]
		vx := robots[i].velocity[0]
		vy := robots[i].velocity[1]

		newPx := (px + vx*periods) % width
		newPy := (py + vy*periods) % height

		newPx = (newPx + width) % width
		newPy = (newPy + height) % height

		if newPx == width/2 || newPy == height/2 {
			continue
		}

		if newPx > width/2 {
			if newPy > height/2 {
				quadrants[3] += 1
			} else {
				quadrants[1] += 1
			}
		} else {
			if newPy > height/2 {
				quadrants[2] += 1
			} else {
				quadrants[0] += 1
			}
		}
	}

	sf := 1
	for _, q := range quadrants {
		sf *= q
	}
	fmt.Printf("%v\n", sf)
}

func Day14b() {
	lines, err := utils.ReadLines("../inputs/day14.txt")
	if err != nil {
		panic("Could not read lines")
	}
	fmt.Printf("%v\n", len(lines))
	robots := make([]Robot, len(lines))
	for i, line := range lines {
		lineParts := strings.Split(line, " ")
		posParts := strings.Split(strings.Split(lineParts[0], "=")[1], ",")
		velParts := strings.Split(strings.Split(lineParts[1], "=")[1], ",")

		px, _ := strconv.Atoi(posParts[0])
		py, _ := strconv.Atoi(posParts[1])
		vx, _ := strconv.Atoi(velParts[0])
		vy, _ := strconv.Atoi(velParts[1])

		robots[i].position = [2]int{px, py}
		robots[i].velocity = [2]int{vx, vy}
	}

	// We check how many are in the middle line each period
	for t := range 10000 {
		table := make([][]int, height)
		for i := range table {
			table[i] = make([]int, width)
		}
		for i := range robots {
			px := robots[i].position[0]
			py := robots[i].position[1]
			vx := robots[i].velocity[0]
			vy := robots[i].velocity[1]

			newPx := ((px+vx)%width + width) % width
			newPy := ((py+vy)%height + height) % height
			robots[i].position = [2]int{newPx, newPy}
			table[newPy][newPx] = 1
		}
		maxLine := 0
		for x := range width {
			line := 0
			for y := range height {
				line += table[y][x]
			}
			maxLine = max(line, maxLine)
		}

		if maxLine > 35 {
			fmt.Printf("%v\n", t+1)
			for _, row := range table {
				for _, item := range row {
					if item == 0 {
						fmt.Printf(" ")
					} else {
						fmt.Printf("*")
					}
				}
				fmt.Printf("\n")
			}
		}
	}
}
