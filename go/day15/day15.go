package day15

import (
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

type Position struct {
	x int
	y int
}

func Day15a() {
	lines, err := utils.ReadLines("../inputs/day15.txt")
	if err != nil {
		panic("Could not read lines")
	}

	walls := map[Position]struct{}{}
	boxes := map[Position]struct{}{}

	// Read the map
	robot := Position{}
	height := 0
	for i, line := range lines {
		for j, char := range line {
			switch char {
			case '@':
				robot.x = j
				robot.y = i
			case 'O':
				boxes[Position{x: j, y: i}] = struct{}{}
			case '#':
				walls[Position{x: j, y: i}] = struct{}{}
			}
		}

		if len(line) == 0 {
			height = i
			break
		}
	}

	// Parse the moves
	moves := [][2]int{}
	for j := height + 1; j < len(lines); j++ {
		for _, char := range lines[j] {
			switch char {
			case '<':
				moves = append(moves, [2]int{0, -1})
			case '>':
				moves = append(moves, [2]int{0, 1})
			case 'v':
				moves = append(moves, [2]int{1, 0})
			case '^':
				moves = append(moves, [2]int{-1, 0})
			}
		}
	}

	// Simulate the path of the robot
	for _, move := range moves {
		impacted_boxes := []Position{}
		can_move := true

		new_pos := robot
		for {
			new_pos.x += move[1]
			new_pos.y += move[0]

			if _, ok := walls[new_pos]; ok {
				can_move = false
				break
			}
			if _, ok := boxes[new_pos]; ok {
				impacted_boxes = append(impacted_boxes, new_pos)
			} else {
				break
			}
		}

		if can_move {
			robot.x += move[1]
			robot.y += move[0]

			for _, box := range impacted_boxes {
				delete(boxes, box)
			}
			for _, box := range impacted_boxes {
				new_box := Position{x: box.x + move[1], y: box.y + move[0]}
				boxes[new_box] = struct{}{}
			}
		}
	}

	// Get the result
	result := 0
	for box := range boxes {
		result += 100*box.y + box.x
	}
	fmt.Printf("%v\n", result)
}

func Day15b() {
	lines, err := utils.ReadLines("../inputs/day15.txt")
	if err != nil {
		panic("Could not read lines")
	}

	walls := map[Position]struct{}{}
	boxes := map[Position]struct{}{}

	// Read the map
	robot := Position{}
	height := 0
	for i, line := range lines {
		for j, char := range line {
			switch char {
			case '@':
				robot.x = 2 * j
				robot.y = i
			case 'O':
				boxes[Position{x: 2 * j, y: i}] = struct{}{}
			case '#':
				walls[Position{x: 2 * j, y: i}] = struct{}{}
				walls[Position{x: 2*j + 1, y: i}] = struct{}{}
			}
		}

		if len(line) == 0 {
			height = i
			break
		}
	}

	// Parse the moves
	moves := [][2]int{}
	for j := height + 1; j < len(lines); j++ {
		for _, char := range lines[j] {
			switch char {
			case '<':
				moves = append(moves, [2]int{0, -1})
			case '>':
				moves = append(moves, [2]int{0, 1})
			case 'v':
				moves = append(moves, [2]int{1, 0})
			case '^':
				moves = append(moves, [2]int{-1, 0})
			}
		}
	}

	// Simulate the path of the robot
	for _, move := range moves {
		impacted_boxes := []Position{}
		can_move := true

		queue := []Position{{x: robot.x + move[1], y: robot.y + move[0]}}

		for len(queue) > 0 {
			new_pos := queue[0]
			new_pos_left := Position{x: new_pos.x - 1, y: new_pos.y}
			queue = queue[1:]
			if _, ok := walls[new_pos]; ok {
				can_move = false
				break
			}

			box_pos := Position{x: -1}
			if _, ok := boxes[new_pos]; ok {
				box_pos = new_pos
			} else if _, ok := boxes[new_pos_left]; ok {
				box_pos = new_pos_left
			}
			if box_pos.x > -1 {
				impacted_boxes = append(impacted_boxes, box_pos)
				if move[1] == 1 {
					queue = append(queue, Position{x: box_pos.x + 2, y: box_pos.y})
				} else if move[1] == -1 {
					queue = append(queue, Position{x: box_pos.x - 1, y: box_pos.y})
				} else {
					queue = append(queue, Position{x: box_pos.x, y: box_pos.y + move[0]})
					queue = append(queue, Position{x: box_pos.x + 1, y: box_pos.y + move[0]})
				}
			}
		}

		if can_move {
			robot.x += move[1]
			robot.y += move[0]

			for _, box := range impacted_boxes {
				delete(boxes, box)
			}
			for _, box := range impacted_boxes {
				new_box := Position{x: box.x + move[1], y: box.y + move[0]}
				boxes[new_box] = struct{}{}
			}
		}
	}

	// Get the result
	result := 0
	for box := range boxes {
		result += 100*box.y + box.x
	}
	fmt.Printf("%v\n", result)
}
