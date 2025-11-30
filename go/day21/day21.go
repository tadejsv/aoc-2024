package day21

import (
	"container/heap"
	"fmt"
	"math"
	"strconv"

	"github.com/tadejsv/aoc-2024/utils"
)

type position struct {
	y int
	x int
}

var all_dirs = map[int]position{
	Down:  {x: 0, y: 1},
	Up:    {x: 0, y: -1},
	Right: {x: 1, y: 0},
	Left:  {x: -1, y: 0},
}

type queueItem struct {
	pos   position
	score int
	path  Path
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

const (
	Press int = iota
	Up
	Down
	Left
	Right
)

type Path []int

var dirPadMoves = map[[2]int][]Path{
	// From Up
	{Up, Up}:    {{}},
	{Up, Down}:  {{Down}},
	{Up, Left}:  {{Down, Left}},
	{Up, Right}: {{Down, Right}, {Right, Down}},
	{Up, Press}: {{Right}},

	// From Press
	{Press, Up}:    {{Left}},
	{Press, Press}: {{}},
	{Press, Left}: {
		{Down, Left, Left},
		{Left, Down, Left},
	},
	{Press, Down}: {
		{Down, Left},
		{Left, Down},
	},
	{Press, Right}: {{Down}},

	// From Left
	{Left, Up}: {{Right, Up}},
	{Left, Press}: {
		{Right, Up, Right},
		{Right, Right, Up},
	},
	{Left, Left}:  {{}},
	{Left, Down}:  {{Right}},
	{Left, Right}: {{Right, Right}},

	// From Down
	{Down, Up}: {{Up}},
	{Down, Press}: {
		{Up, Right},
		{Right, Up},
	},
	{Down, Left}:  {{Left}},
	{Down, Down}:  {{}},
	{Down, Right}: {{Right}},

	// From Right
	{Right, Up}: {
		{Up, Left},
		{Left, Up},
	},
	{Right, Press}: {{Up}},
	{Right, Left}:  {{Left, Left}},
	{Right, Down}:  {{Left}},
	{Right, Right}: {{}},
}

// -1 is the A button
var numPadMoves = map[[2]int][]Path{}

var numLocations = map[int]position{
	-1: {3, 2},
	0:  {3, 1},
	1:  {2, 0},
	2:  {2, 1},
	3:  {2, 2},
	4:  {1, 0},
	5:  {1, 1},
	6:  {1, 2},
	7:  {0, 0},
	8:  {0, 1},
	9:  {0, 2},
}

func addNumMoves() {
	locationsNum := map[position]int{}
	for num, loc := range numLocations {
		locationsNum[loc] = num
	}

	for num, loc := range numLocations {
		cache := map[position]int{}
		pq := priorityQueue{{loc, 0, []int{}}}

		for len(pq) > 0 {
			item := heap.Pop(&pq).(*queueItem)
			x, y := item.pos.x, item.pos.y
			if score, ok := cache[item.pos]; ok && score < item.score {
				continue
			}

			cache[item.pos] = item.score

			newNum := locationsNum[item.pos]
			numPadMoves[[2]int{num, newNum}] = append(numPadMoves[[2]int{num, newNum}], item.path)

			for button, dir := range all_dirs {
				nx, ny := x+dir.x, y+dir.y
				if nx < 0 || ny < 0 || ny >= 4 || nx >= 3 {
					continue
				}
				if ny == 3 && nx == 0 {
					continue
				}
				newScore := item.score + 1
				newPath := make([]int, len(item.path)+1)
				copy(newPath, item.path)
				newPath[len(newPath)-1] = button
				heap.Push(&pq, &queueItem{
					pos:   position{ny, nx},
					score: newScore,
					path:  newPath,
				})
			}

		}
	}
}

type HashKey struct {
	from  int
	to    int
	level int
}

func getNumMoves(
	from, to, level int,
	moves map[[2]int][]Path,
	cache map[HashKey]int64,
) int64 {
	if level == 0 {
		return 1
	}

	ck := HashKey{from, to, level}
	if v, ok := cache[ck]; ok {
		return v
	}

	best := int64(math.MaxInt64)
	for _, path := range moves[[2]int{from, to}] {
		var cost int64
		prev := int(Press)
		for _, b := range path {
			cost += getNumMoves(prev, b, level-1, dirPadMoves, cache)
			prev = b
		}
		cost += getNumMoves(prev, int(Press), level-1, dirPadMoves, cache)
		best = min(cost, best)
	}

	cache[ck] = best
	return best
}

func getNum(c byte) int {
	if c == 'A' {
		return -1
	}
	return int(c - '0')
}

func Day21a() {
	lines, err := utils.ReadLines("../inputs/day21.txt")
	if err != nil {
		panic("Could not read lines")
	}

	addNumMoves()

	cache := map[HashKey]int64{}
	result := int64(0)
	for _, line := range lines {
		total := int64(0)
		for i := range len(line) {
			from := -1

			if i > 0 {
				from = getNum(line[i-1])
			}
			to := getNum(line[i])
			total += getNumMoves(from, to, 3, numPadMoves, cache)
		}

		n, _ := strconv.Atoi(line[:3])
		result += total * int64(n)
	}

	fmt.Printf("%v\n", result)
}

func Day21b() {
	lines, err := utils.ReadLines("../inputs/day21.txt")
	if err != nil {
		panic("Could not read lines")
	}

	addNumMoves()

	cache := map[HashKey]int64{}
	result := int64(0)
	for _, line := range lines {
		total := int64(0)
		for i := range len(line) {
			from := -1

			if i > 0 {
				from = getNum(line[i-1])
			}
			to := getNum(line[i])
			total += getNumMoves(from, to, 26, numPadMoves, cache)
		}

		n, _ := strconv.Atoi(line[:3])
		result += total * int64(n)
	}

	fmt.Printf("%v\n", result)
}
