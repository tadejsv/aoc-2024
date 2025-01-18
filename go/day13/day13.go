package day13

import (
	"fmt"
	"strconv"
	"strings"

	"github.com/tadejsv/aoc-2024/utils"
)

type Machine struct {
	aVals [2]int64
	bVals [2]int64
	pVals [2]int64
}

func gcd_euclid(a, b int64) int64 {
	if b == 0 {
		return a
	}
	return gcd_euclid(b, a%b)
}

func extended_euclidean(a, b int64) (x, y, gcd int64) {
	if b == 0 {
		return 1, 0, a
	}
	xOld, yOld, gcd := extended_euclidean(b, a%b)
	x = yOld
	y = xOld - yOld*(a/b)
	return x, y, gcd
}

func solveExtra(m *Machine) int64 {
	const costA = 3
	const costB = 1

	// Check for co-linear equations - assume they all have no solutions (true for given inputs)
	det := m.bVals[1]*m.aVals[0] - m.bVals[0]*m.aVals[1]
	var B int64
	if det == 0 {
		// Test for inconsistency
		if m.aVals[0]/m.aVals[1] != m.pVals[0]/m.pVals[1] {
			return 0
		}

		// Test for lack of integer solutions
		if m.pVals[0]%gcd_euclid(m.aVals[0], m.bVals[0]) != 0 {
			return 0
		}

		aPrime := m.aVals[0] / gcd_euclid(m.aVals[0], m.bVals[0])
		bInv, _, _ := extended_euclidean(m.bVals[0], aPrime)
		bMod := (m.pVals[0] * bInv) % aPrime
		if bMod < 0 {
			bMod += aPrime
		}

		if float64(costA)/float64(costB) > float64(m.aVals[0])/float64(m.bVals[0]) {
			bMax := m.pVals[0] / m.bVals[0]
			B = aPrime*(bMax/aPrime) + bMod
			if B > bMax {
				B -= aPrime
			}
		} else {
			B = bMod
		}
	} else {
		cPart := m.pVals[1]*m.aVals[0] - m.pVals[0]*m.aVals[1]
		B = cPart / det
	}
	A := (m.pVals[0] - B*m.bVals[0]) / m.aVals[0]
	if A < 0 || B < 0 {
		return 0
	}

	// Test that we indeed have an integer solution
	pSolution1 := (m.aVals[0] * A) + (m.bVals[0] * B)
	pSolution2 := (m.aVals[1] * A) + (m.bVals[1] * B)

	if pSolution1 == m.pVals[0] && pSolution2 == m.pVals[1] {
		return (3 * A) + B
	}
	return 0
}

func solve(m *Machine) int64 {
	// Check for co-linear equations - assume they all have no solutions (true for given inputs)
	det := m.bVals[1]*m.aVals[0] - m.bVals[0]*m.aVals[1]
	if det == 0 {
		return 0
	}
	cPart := m.pVals[1]*m.aVals[0] - m.pVals[0]*m.aVals[1]
	B := cPart / det
	A := (m.pVals[0] - B*m.bVals[0]) / m.aVals[0]
	if A < 0 || B < 0 {
		return 0
	}

	// Test that we indeed have an integer solution
	pSolution1 := (m.aVals[0] * A) + (m.bVals[0] * B)
	pSolution2 := (m.aVals[1] * A) + (m.bVals[1] * B)

	if pSolution1 == m.pVals[0] && pSolution2 == m.pVals[1] {
		return (3 * A) + B
	}
	return 0
}

func Day13a() {
	lines, err := utils.ReadLines("../inputs/day13.txt")
	if err != nil {
		panic("Could not read lines")
	}

	machines := []Machine{}
	for i := 0; i < len(lines); i += 4 {
		machine := Machine{}
		aParts := strings.Split(strings.Split(lines[i], ": ")[1], ", ")
		bParts := strings.Split(strings.Split(lines[i+1], ": ")[1], ", ")
		pParts := strings.Split(strings.Split(lines[i+2], ": ")[1], ", ")

		for j := range aParts {
			aVal, _ := strconv.Atoi(aParts[j][2:])
			machine.aVals[j] = int64(aVal)
			bVal, _ := strconv.Atoi(bParts[j][2:])
			machine.bVals[j] = int64(bVal)
			pVal, _ := strconv.Atoi(pParts[j][2:])
			machine.pVals[j] = int64(pVal)
		}
		machines = append(machines, machine)
	}

	totalTokens := int64(0)
	for _, machine := range machines {
		totalTokens += solve(&machine)
	}
	fmt.Printf("%v\n", totalTokens)
}

func Day13b() {
	const addPrize = 10000000000000
	lines, err := utils.ReadLines("../inputs/day13.txt")
	if err != nil {
		panic("Could not read lines")
	}

	machines := []Machine{}
	for i := 0; i < len(lines); i += 4 {
		machine := Machine{}
		aParts := strings.Split(strings.Split(lines[i], ": ")[1], ", ")
		bParts := strings.Split(strings.Split(lines[i+1], ": ")[1], ", ")
		pParts := strings.Split(strings.Split(lines[i+2], ": ")[1], ", ")

		for j := range aParts {
			aVal, _ := strconv.Atoi(aParts[j][2:])
			machine.aVals[j] = int64(aVal)
			bVal, _ := strconv.Atoi(bParts[j][2:])
			machine.bVals[j] = int64(bVal)
			pVal, _ := strconv.Atoi(pParts[j][2:])
			machine.pVals[j] = int64(pVal) + addPrize
		}
		machines = append(machines, machine)
	}

	totalTokens := int64(0)
	for _, machine := range machines {
		totalTokens += solve(&machine)
	}
	fmt.Printf("%v\n", totalTokens)
}

func Day13extra() {
	lines, err := utils.ReadLines("../inputs/day13_extra.txt")
	if err != nil {
		panic("Could not read lines")
	}

	machines := []Machine{}
	for i := 0; i < len(lines); i += 4 {
		machine := Machine{}
		aParts := strings.Split(strings.Split(lines[i], ": ")[1], ", ")
		bParts := strings.Split(strings.Split(lines[i+1], ": ")[1], ", ")
		pParts := strings.Split(strings.Split(lines[i+2], ": ")[1], ", ")

		for j := range aParts {
			aVal, _ := strconv.Atoi(aParts[j][2:])
			machine.aVals[j] = int64(aVal)
			bVal, _ := strconv.Atoi(bParts[j][2:])
			machine.bVals[j] = int64(bVal)
			pVal, _ := strconv.Atoi(pParts[j][2:])
			machine.pVals[j] = int64(pVal)
		}
		machines = append(machines, machine)
	}

	totalTokens := int64(0)
	for _, machine := range machines {
		totalTokens += solveExtra(&machine)
	}
	fmt.Printf("%v\n", totalTokens)
}
