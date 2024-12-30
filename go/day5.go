package main

import (
	"fmt"
	"strconv"
	"strings"
)

type IntSet = map[int]bool

func setIntersection(set1, set2 IntSet) IntSet {
	intersection := make(IntSet)
	for num1 := range set1 {
		if _, ok := set2[num1]; ok {
			intersection[num1] = true
		}
	}

	return intersection
}

func parseInput(fname string) (incoming, outgoing map[int]IntSet, updates [][]int) {
	lines, err := ReadLines(fname)
	if err != nil {
		panic("Could not read lines")
	}

	incoming = make(map[int]IntSet)
	outgoing = make(map[int]IntSet)

	parseUpdates := false
	for _, line := range lines {
		if len(line) == 0 {
			parseUpdates = true
			continue
		}

		if parseUpdates {
			parts := strings.Split(line, ",")
			update := make([]int, len(parts))
			for i, part := range parts {
				num, _ := strconv.Atoi(part)
				update[i] = num
			}
			updates = append(updates, update)
		} else {
			parts := strings.Split(line, "|")
			num1, _ := strconv.Atoi(parts[0])
			num2, _ := strconv.Atoi(parts[1])

			if _, ok := outgoing[num1]; !ok {
				outgoing[num1] = make(IntSet)
			}
			if _, ok := incoming[num2]; !ok {
				incoming[num2] = make(IntSet)
			}
			outgoing[num1][num2] = true
			incoming[num2][num1] = true
		}
	}

	return
}

func topologicallyOrder(list []int, incoming, outgoing map[int]IntSet) []int {
	listSet := make(IntSet)
	for _, num := range list {
		listSet[num] = true
	}
	restrictedOutgoing := make(map[int]IntSet)
	restrictedIncoming := make(map[int]IntSet)
	for num, numOutgoing := range outgoing {
		restrictedOutgoing[num] = setIntersection(numOutgoing, listSet)
	}
	for num, numIncoming := range incoming {
		restrictedIncoming[num] = setIntersection(numIncoming, listSet)
	}

	empty := make([]int, 0)
	for _, num := range list {
		if len(restrictedIncoming[num]) == 0 {
			empty = append(empty, num)
		}
	}

	ordered := make([]int, 0, len(list))
	for len(empty) > 0 {
		num := empty[0]
		ordered = append(ordered, empty[0])
		empty = empty[1:]

		for neighbor := range restrictedOutgoing[num] {
			delete(restrictedIncoming[neighbor], num)
			if len(restrictedIncoming[neighbor]) == 0 {
				empty = append(empty, neighbor)
			}
		}
	}

	return ordered
}

func Day5a() {
	_, outgoing, updates := parseInput("../inputs/day5.txt")

	sum_good := 0
	for _, update := range updates {
		seen := make(IntSet)
		good := true
		for _, num := range update {
			intersection := setIntersection(seen, outgoing[num])
			if len(intersection) != 0 {
				good = false
				break
			}
			seen[num] = true
		}
		if good {
			sum_good += update[len(update)/2]
		}
	}

	fmt.Printf("%v\n", sum_good)
}

func Day5b() {
	incoming, outgoing, updates := parseInput("../inputs/day5.txt")

	sum_bad := 0
	for _, update := range updates {
		seen := make(IntSet)
		for _, num := range update {
			intersection := setIntersection(seen, outgoing[num])
			if len(intersection) != 0 {
				ordered := topologicallyOrder(update, incoming, outgoing)
				sum_bad += ordered[len(ordered)/2]
				break
			}
			seen[num] = true
		}
	}

	fmt.Printf("%v\n", sum_bad)
}
