package day20

import (
	"container/heap"
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

func abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

type position struct {
	x int
	y int
}

var all_dirs = []position{
	{x: 0, y: 1},
	{x: 0, y: -1},
	{x: 1, y: 0},
	{x: -1, y: 0},
}

type queueItem struct {
	pos   position
	score int
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

func dfs(grid [][]bool, startPos position) [][]int {
	h := len(grid)
	if h == 0 {
		return nil
	}
	w := len(grid[0])

	// Initialize all distances to -1
	dist := make([][]int, h)
	for y := range dist {
		dist[y] = make([]int, w)
		for x := range dist[y] {
			dist[y][x] = -1
		}
	}

	start := queueItem{startPos, 0}
	pq := priorityQueue{&start}
	heap.Init(&pq)

	for len(pq) > 0 {
		item := heap.Pop(&pq).(*queueItem)
		x, y := item.pos.x, item.pos.y

		if dist[y][x] != -1 {
			continue
		}

		dist[y][x] = item.score

		for _, dir := range all_dirs {
			nx, ny := x+dir.x, y+dir.y
			if nx < 0 || ny < 0 || ny >= h || nx >= w {
				continue
			}
			if !grid[ny][nx] {
				continue
			}
			newScore := item.score + 1
			if d := dist[ny][nx]; d == -1 || newScore < d {
				heap.Push(&pq, &queueItem{
					pos:   position{nx, ny},
					score: newScore,
				})
			}
		}
	}

	return dist
}

func getNumCheats(grid [][]bool, start position, end position, cheatDuration, minImprovement int) int {
	h := len(grid)
	w := len(grid[0])

	fromStart := dfs(grid, start)
	fromEnd := dfs(grid, end)

	baseline := fromStart[end.y][end.x]

	// Get all candidates that can potentially deliver the improvement with the cheat
	candidates := []position{}
	for y := range h {
		for x := range w {
			if score := fromStart[y][x]; score != -1 && score+minImprovement <= baseline {
				candidates = append(candidates, position{x, y})
			}
		}
	}

	// For all candidates, check all positions inside the disc around it (manhattan distance)
	goodCheats := 0
	for _, candidate := range candidates {
		fromStartDist := fromStart[candidate.y][candidate.x]
		for dy := -cheatDuration; dy <= cheatDuration; dy++ {
			ny := candidate.y + dy
			remaining := cheatDuration - abs(dy)
			for dx := -remaining; dx <= remaining; dx++ {
				nx := candidate.x + dx

				if nx < 0 || ny < 0 || ny >= h || nx >= w {
					continue
				}

				distFromEnd := fromEnd[ny][nx]
				if distFromEnd == -1 {
					continue
				}

				distToEndCheat := abs(dx) + abs(dy) + fromStartDist
				if distToEndCheat+distFromEnd+minImprovement <= baseline {
					goodCheats++
				}
			}
		}
	}
	return goodCheats
}

func Day20a() {
	lines, err := utils.ReadLines("../inputs/day20.txt")
	if err != nil {
		panic("Could not read lines")
	}

	grid := make([][]bool, len(lines))
	for i := range grid {
		grid[i] = make([]bool, len(lines[0]))
	}

	start := position{}
	end := position{}
	for i, line := range lines {
		for j, char := range line {
			switch char {
			case '.':
				grid[i][j] = true
			case 'S':
				grid[i][j] = true
				start = position{j, i}
			case 'E':
				grid[i][j] = true
				end = position{j, i}
			}
		}
	}

	// Get path length from start and end for all valid tiles
	goodCheats := getNumCheats(grid, start, end, 2, 100)

	fmt.Printf("%v\n", goodCheats)
}

func Day20b() {
	lines, err := utils.ReadLines("../inputs/day20.txt")
	if err != nil {
		panic("Could not read lines")
	}

	grid := make([][]bool, len(lines))
	for i := range grid {
		grid[i] = make([]bool, len(lines[0]))
	}

	start := position{}
	end := position{}
	for i, line := range lines {
		for j, char := range line {
			switch char {
			case '.':
				grid[i][j] = true
			case 'S':
				grid[i][j] = true
				start = position{j, i}
			case 'E':
				grid[i][j] = true
				end = position{j, i}
			}
		}
	}

	// Get path length from start and end for all valid tiles
	goodCheats := getNumCheats(grid, start, end, 20, 100)

	fmt.Printf("%v\n", goodCheats)
}
