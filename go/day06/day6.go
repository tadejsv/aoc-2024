package day6

import (
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

type IntSet = map[int]bool

type Direction int

const (
	Right Direction = iota
	Left
	Up
	Down
)

var turn []Direction = []Direction{Down, Up, Right, Left}

func getBoard(fname string) ([][]int, [2]int, Direction) {
	lines, err := utils.ReadLines(fname)
	if err != nil {
		panic("Could not read lines")
	}

	var pos [2]int
	var dir Direction
	board := make([][]int, len(lines))
	for i, line := range lines {
		boardLine := make([]int, len(line))
		for j, char := range line {
			switch char {
			case '#':
				boardLine[j] = 1
			case '>':
				pos = [2]int{i, j}
				dir = Right
			case '<':
				pos = [2]int{i, j}
				dir = Left
			case 'v':
				pos = [2]int{i, j}
				dir = Down
			case '^':
				pos = [2]int{i, j}
				dir = Up
			}
		}
		board[i] = boardLine
	}

	return board, pos, dir
}

func hashPos(pos [2]int, nCols int) int {
	return pos[0]*nCols + pos[1]
}

func hashPosDir(pos [2]int, dir Direction, nCols int) int {
	return pos[0]*nCols*4 + pos[1]*4 + int(dir)
}

func nextPos(pos [2]int, dir Direction) [2]int {
	switch dir {
	case Up:
		pos = [2]int{pos[0] - 1, pos[1]}
	case Down:
		pos = [2]int{pos[0] + 1, pos[1]}
	case Left:
		pos = [2]int{pos[0], pos[1] - 1}
	case Right:
		pos = [2]int{pos[0], pos[1] + 1}
	}
	return pos
}

func isOOB(pos [2]int, board [][]int) bool {
	if pos[0] < 0 || pos[0] >= len(board) {
		return true
	}
	if pos[1] < 0 || pos[1] >= len(board[0]) {
		return true
	}
	return false
}

func checkLoop(pos [2]int, dir Direction, board [][]int) int {
	visited := make(IntSet)
	for {
		posHash := hashPosDir(pos, dir, len(board[0]))
		if _, ok := visited[posHash]; ok {
			return 1
		}
		visited[posHash] = true

		candidate := nextPos(pos, dir)
		if isOOB(candidate, board) {
			return 0
		}
		if board[candidate[0]][candidate[1]] == 1 {
			dir = turn[dir]
			continue
		}

		pos = candidate
	}
}

func Day6a() {
	board, pos, dir := getBoard("../inputs/day6.txt")

	visited := make(IntSet)
	for {
		visited[hashPos(pos, len(board[0]))] = true
		candidate := nextPos(pos, dir)
		if isOOB(candidate, board) {
			break
		}
		if board[candidate[0]][candidate[1]] == 1 {
			dir = turn[dir]
			continue
		}

		pos = candidate
	}

	fmt.Printf("%v\n", len(visited))
}

func Day6b() {
	board, pos, dir := getBoard("../inputs/day6.txt")

	numLoops := 0
	blocksPlaced := make(IntSet)
	for {
		candidate := nextPos(pos, dir)
		if isOOB(candidate, board) {
			break
		}
		if board[candidate[0]][candidate[1]] == 1 {
			dir = turn[dir]
			continue
		}

		blockHash := hashPos(candidate, len(board[0]))
		if _, ok := blocksPlaced[blockHash]; !ok {
			board[candidate[0]][candidate[1]] = 1
			numLoops += checkLoop(pos, dir, board)
			board[candidate[0]][candidate[1]] = 0
			blocksPlaced[blockHash] = true
		}

		pos = candidate
	}

	fmt.Printf("%v\n", numLoops)
}
