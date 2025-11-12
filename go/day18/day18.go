package day18

import (
	"container/heap"
	"fmt"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

const GRID_SIZE = 71
const SECONDS = 1024

type position struct {
	x int
	y int
}

var all_dirs = []position{{x: 0, y: 1}, {x: 0, y: -1}, {x: 1, y: 0}, {x: -1, y: 0}}

type queueItem struct {
	pos   position
	score int
}

type cacheKey struct {
	pos position
	dir position
}

type priorityQueue []*queueItem

func (pq priorityQueue) Len() int { return len(pq) }

func (pq priorityQueue) Less(i, j int) bool {
	return pq[i].score < pq[j].score
}

func (pq priorityQueue) Swap(i, j int) {
	pq[i], pq[j] = pq[j], pq[i]
}

func (pq *priorityQueue) Push(x any) {
	item := x.(*queueItem)
	*pq = append(*pq, item)
}

func (pq *priorityQueue) Pop() any {
	old := *pq
	n := len(old)
	item := old[n-1]
	old[n-1] = nil // don't stop the GC from reclaiming the item eventually
	*pq = old[0 : n-1]
	return item
}

func getPath(grid *[GRID_SIZE][GRID_SIZE]int) int {
	cache := [GRID_SIZE][GRID_SIZE]bool{}
	start := queueItem{position{0, 0}, 0}
	queue := priorityQueue{&start}
	heap.Init(&queue)

	best := -1
	for len(queue) > 0 {
		item := heap.Pop(&queue).(*queueItem)
		if item.pos.x == GRID_SIZE-1 && item.pos.y == GRID_SIZE-1 {
			best = item.score
			break
		}
		if cache[item.pos.y][item.pos.x] {
			continue
		}
		cache[item.pos.y][item.pos.x] = true

		for _, dir := range all_dirs {
			new_pos := position{item.pos.x + dir.x, item.pos.y + dir.y}
			if new_pos.x < GRID_SIZE && new_pos.y < GRID_SIZE && new_pos.x >= 0 && new_pos.y >= 0 && grid[new_pos.y][new_pos.x] == 0 {
				new_item := &queueItem{new_pos, item.score + 1}
				heap.Push(&queue, new_item)
			}
		}
	}

	return best
}

func Day18a() {
	lines, err := utils.ReadLines("../inputs/day18.txt")
	if err != nil {
		panic("Could not read lines")
	}

	grid := [GRID_SIZE][GRID_SIZE]int{}

	blocks := [][2]int{}
	for _, line := range lines[:SECONDS] {
		parts := strings.Split(line, ",")
		col, _ := strconv.Atoi(parts[0])
		row, _ := strconv.Atoi(parts[1])

		blocks = append(blocks, [2]int{row, col})
	}

	for _, block := range blocks {
		grid[block[0]][block[0]] = 1
	}

	best := getPath(&grid)

	fmt.Printf("%v\n", best)
}

func Day18b() {
	lines, err := utils.ReadLines("../inputs/day18.txt")
	if err != nil {
		panic("Could not read lines")
	}
	blocks := [][2]int{}
	for _, line := range lines[:SECONDS] {
		parts := strings.Split(line, ",")
		col, _ := strconv.Atoi(parts[0])
		row, _ := strconv.Atoi(parts[1])

		blocks = append(blocks, [2]int{row, col})
	}

	N := len(lines)

	l, u := 0, N
	var n int
	for l < u {
		n = (l + u) / 2

		grid := [GRID_SIZE][GRID_SIZE]int{}
		for _, block := range blocks[:n] {
			grid[block[0]][block[0]] = 1
		}
		best := getPath(&grid)

		if best > 0 {
			l = n + 1
		} else {
			u = n
		}
	}

	fmt.Printf("%v\n", lines[n-1])
}
