# Sequence ADT Implementation in C++

## Project Overview

A C++ implementation of fundamental Abstract Data Types (ADTs) including sequences, stacks, queues, and deques with both mutable and immutable variants.

## Key Features

### Core Data Structures
- **Dynamic Array** - Resizable array implementation
- **Linked List** - Node-based linked list
- **Sequences**:
  - Mutable/Immutable Array Sequence
  - Mutable/Immutable List Sequence

### Container Types
- **Stack** - LIFO structure
- **Queue** - FIFO structure  
- **Deque** - Double-ended queue

### Complete Functional Operations
| Operation | Description | Complexity |
|-----------|-------------|------------|
| `map` | Applies transformation to all elements | O(n) |
| `where` | Filters elements using predicate | O(n) |
| `reduce` | Aggregates elements with accumulator | O(n) |
| `concat` | Combines two structures | O(n+m) |
| `getSubsequence` | Extracts range of elements | O(k) |
| `zip` | Pairs elements with another sequence | O(min(n,m)) |
| `split` | Divides using predicate | O(n) |
| `contains` | Checks element existence | O(n) |
| `containsSubsequence` | Checks subsequence | O(n*m) |
| `sort` | Sorts elements (with optional comparator) | O(n log n) |

### Building and Testing
- **make all** - Build everything
- **make run** - Build and run cli
- **make test** - Build and run tests
- **make clean** - Clean build