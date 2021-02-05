package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

const NULLVAL int = 0

type Command struct {
	op  int
	val int
}

type Heap []int

func (heap *Heap) Min() int {
	return (*heap)[0]
}

func (heap *Heap) Add(v int) {
	*heap = append(*heap, v)

	index := heap.size() - 1
	for (*heap)[index] < (*heap)[parent(index)] {
		(*heap)[index], (*heap)[parent(index)] = (*heap)[parent(index)], (*heap)[index]
		index = parent(index)
	}
}

func (heap *Heap) Delete(v int) {
	index := (*heap).indexOf(v)
	lastIndex := (*heap).size() - 1

	(*heap)[index] = (*heap)[lastIndex]
	*heap = (*heap)[:lastIndex]
	heap.minHeapify(index)
}

func (heap *Heap) size() int {
	return len(*heap)
}

func (heap *Heap) indexOf(val int) int {
	for i, _ := range *heap {
		if (*heap)[i] == val {
			return i
		}
	}
	return -1
}

func (heap *Heap) minHeapify(index int) {
	var smallest int
	l := left(index)
	r := right(index)
	if l < heap.size() && (*heap)[l] < (*heap)[index] {
		smallest = l
	} else {
		smallest = index
	}
	if r < heap.size() && (*heap)[r] < (*heap)[smallest] {
		smallest = r
	}
	if smallest != index {
		(*heap)[index], (*heap)[smallest] = (*heap)[smallest], (*heap)[index]
		heap.minHeapify(smallest)
	}
}

func parent(i int) int {
	if i == 0 {
		return 0
	} else if i%2 == 0 {
		return i/2 - 1
	}
	return i / 2
}

func left(i int) int {
	return 2*i + 1
}

func right(i int) int {
	return 2*i + 2
}

func parseCommand(cmd []string) Command {
	op, _ := strconv.Atoi(cmd[0])
	if len(cmd) == 1 {
		return Command{op, NULLVAL}
	}
	val, _ := strconv.Atoi(cmd[1])
	return Command{op, val}
}

func runCommand(heap *Heap, cmd Command) {
	switch cmd.op {
	case 1:
		heap.Add(cmd.val)
	case 2:
		heap.Delete(cmd.val)
	case 3:
		fmt.Println(heap.Min())
	}
}

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	scanner.Scan()
	len := scanner.Text()
	val, _ := strconv.Atoi(len)

	var heap Heap

	for i := 0; i < val; i++ {
		scanner.Scan()
		val := scanner.Text()
		cmd := strings.Split(val, " ")
		runCommand(&heap, parseCommand(cmd))
	}
}
