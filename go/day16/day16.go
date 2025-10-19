package day16

import (
	"container/heap"
	"fmt"

	"github.com/tadejsv/aoc-2024/utils"
)

const turn_penalty = 1000

type position struct {
	x int
	y int
}

var all_dirs = []position{{x: 0, y: 1}, {x: 0, y: -1}, {x: 1, y: 0}, {x: -1, y: 0}}

type queueItem struct {
	pos   position
	dir   position
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

func Day16a() {
	lines, err := utils.ReadLines("../inputs/day16.txt")
	if err != nil {
		panic("Could not read lines")
	}

	start := position{}
	end := position{}

	walls := make([][]bool, len(lines))
	for i := range walls {
		walls[i] = make([]bool, len(lines[0]))
		for j, char := range lines[i] {
			switch char {
			case '#':
				walls[i][j] = true
			case 'S':
				start = position{x: j, y: i}
			case 'E':
				end = position{x: j, y: i}
			}
		}
	}

	cache := map[cacheKey]struct{}{}
	top_score := 0
	queue := priorityQueue{{pos: start, dir: position{x: 1, y: 0}, score: 0}}
	heap.Init(&queue)

	for {
		item := heap.Pop(&queue).(*queueItem)

		ck := cacheKey{pos: item.pos, dir: item.dir}
		if _, ok := cache[ck]; ok {
			continue
		}
		cache[ck] = struct{}{}

		if item.pos.x == end.x && item.pos.y == end.y {
			top_score = item.score
			break
		}

		// Try going forward
		new_pos := position{x: item.pos.x + item.dir.x, y: item.pos.y + item.dir.y}
		if !walls[new_pos.y][new_pos.x] {
			new_item := &queueItem{pos: new_pos, dir: item.dir, score: item.score + 1}
			heap.Push(&queue, new_item)
		}

		// Try turning
		new_dir_1 := position{x: item.dir.y, y: item.dir.x}
		new_dir_2 := position{x: -item.dir.y, y: -item.dir.x}

		new_item := &queueItem{pos: item.pos, dir: new_dir_1, score: item.score + turn_penalty}
		heap.Push(&queue, new_item)

		new_item = &queueItem{pos: item.pos, dir: new_dir_2, score: item.score + turn_penalty}
		heap.Push(&queue, new_item)
	}

	fmt.Printf("%v\n", top_score)
}

func Day16b() {
	lines, err := utils.ReadLines("../inputs/day16.txt")
	if err != nil {
		panic("Could not read lines")
	}

	start := position{}
	end := position{}

	walls := make([][]bool, len(lines))
	for i := range walls {
		walls[i] = make([]bool, len(lines[0]))
		for j, char := range lines[i] {
			switch char {
			case '#':
				walls[i][j] = true
			case 'S':
				start = position{x: j, y: i}
			case 'E':
				end = position{x: j, y: i}
			}
		}
	}

	top_score := 0
	cache := map[cacheKey]int{}
	queue := priorityQueue{{pos: start, dir: position{x: 1, y: 0}, score: 0}}
	heap.Init(&queue)

	for {
		item := heap.Pop(&queue).(*queueItem)

		ck := cacheKey{pos: item.pos, dir: item.dir}
		if _, ok := cache[ck]; ok {
			continue
		}
		cache[ck] = item.score

		if item.pos.x == end.x && item.pos.y == end.y {
			top_score = item.score
			break
		}

		// Try going forward
		new_pos := position{x: item.pos.x + item.dir.x, y: item.pos.y + item.dir.y}
		if !walls[new_pos.y][new_pos.x] {
			new_item := &queueItem{pos: new_pos, dir: item.dir, score: item.score + 1}
			heap.Push(&queue, new_item)
		}

		// Try turning
		new_dirs := []position{{x: item.dir.y, y: item.dir.x}, {x: -item.dir.y, y: -item.dir.x}}
		for _, new_dir := range new_dirs {
			new_item := &queueItem{pos: item.pos, dir: new_dir, score: item.score + turn_penalty}
			heap.Push(&queue, new_item)
		}
	}

	// Backtrack from end to start
	best_squares := map[position]struct{}{}
	best_queue := []queueItem{}
	for _, dir := range all_dirs {
		best_queue = append(best_queue, queueItem{pos: end, dir: dir, score: top_score})
	}

	for len(best_queue) > 0 {
		item := best_queue[0]
		best_queue = best_queue[1:]
		best_squares[item.pos] = struct{}{}

		new_pos := position{x: item.pos.x - item.dir.x, y: item.pos.y - item.dir.y}
		new_score := item.score - 1
		if cv, ok := cache[cacheKey{pos: new_pos, dir: item.dir}]; ok && cv == new_score {
			best_queue = append(best_queue, queueItem{pos: new_pos, dir: item.dir, score: new_score})
		}

		new_dirs := []position{{x: item.dir.y, y: item.dir.x}, {x: -item.dir.y, y: -item.dir.x}}
		new_score = item.score - turn_penalty
		for _, new_dir := range new_dirs {
			if cv, ok := cache[cacheKey{pos: item.pos, dir: new_dir}]; ok && cv == new_score {
				best_queue = append(best_queue, queueItem{pos: item.pos, dir: new_dir, score: new_score})
			}
		}
	}

	fmt.Printf("%v\n", len(best_squares))
}
