# CPP09 - STL (Standard Template Library)

This module introduces the Standard Template Library (STL) in C++. Each exercise requires using different containers to solve various algorithmic problems.

## Table of Contents

- [Overview](#overview)
- [Exercise 00: Bitcoin Exchange](#exercise-00-bitcoin-exchange)
- [Exercise 01: Reverse Polish Notation](#exercise-01-reverse-polish-notation)
- [Exercise 02: PmergeMe](#exercise-02-pmergeme)
- [Container Usage Summary](#container-usage-summary)
- [Compilation](#compilation)
- [References](#references)

---

## Overview

| Exercise | Program | Container(s) | Algorithm/Concept |
|----------|---------|--------------|-------------------|
| ex00 | `btc` | `std::map` | Date lookup, CSV parsing |
| ex01 | `RPN` | `std::stack` | Reverse Polish Notation |
| ex02 | `PmergeMe` | `std::vector`, `std::deque` | Ford-Johnson sort |

### Module Rules
- Each container can only be used **once** across all exercises
- Must use at least **one** container per exercise (ex02 requires **two**)
- Compile with `-Wall -Wextra -Werror`
- Code must be compatible with C++98 standard

---

## Exercise 00: Bitcoin Exchange

### Description
A program that calculates the value of a certain amount of Bitcoin on a specific date using historical exchange rate data.

### Container Used
- **`std::map<std::string, float>`** - Stores date-rate pairs for O(log n) lookup

### How It Works

```
Input file format: "date | value"
Database format: "date,exchange_rate"

Example:
  Input: 2011-01-03 | 3
  Database: 2011-01-03,0.3
  Output: 2011-01-03 => 3 = 0.9
```

### Key Features
- **`upper_bound()`** - Binary search to find the closest date if exact match not found
- Date validation (YYYY-MM-DD format)
- Value validation (0-1000 range)

### Usage
```bash
cd ex00
make
./btc input.txt
```

### Example Output
```
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
```

### Error Handling
| Error | Message |
|-------|---------|
| Invalid date format | `Error: bad input => [line]` |
| Negative value | `Error: not a positive number.` |
| Value > 1000 | `Error: too large a number.` |
| Date too early | `Error: date too early` |

---

## Exercise 01: Reverse Polish Notation

### Description
A calculator that evaluates mathematical expressions in Reverse Polish Notation (postfix notation).

### Container Used
- **`std::stack<int>`** - LIFO structure perfect for RPN evaluation

### How RPN Works

```
Infix:    (3 + 4) * 2
Postfix:  3 4 + 2 *

Evaluation:
  Push 3      → Stack: [3]
  Push 4      → Stack: [3, 4]
  Apply +     → Stack: [7]
  Push 2      → Stack: [7, 2]
  Apply *     → Stack: [14]
  Result: 14
```

### Algorithm
```
for each token in expression:
    if token is a number:
        push to stack
    if token is an operator (+, -, *, /):
        pop two operands (a, b)
        compute: b operator a
        push result
return top of stack
```

### Usage
```bash
cd ex01
make
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
```

### Example Output
```
$ ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
$ ./RPN "7 7 * 7 -"
42
$ ./RPN "1 2 * 2 / 2 * 2 4 - +"
0
$ ./RPN "(1 + 1)"
Error
```

### Constraints
- Numbers must be single digits (0-9)
- Supported operators: `+`, `-`, `*`, `/`
- No brackets or decimal numbers

---

## Exercise 02: PmergeMe

### Description
Implementation of the Ford-Johnson merge-insert sort algorithm using two different containers to compare performance.

### Containers Used
- **`std::vector<int>`** - Contiguous memory, fast random access
- **`std::deque<int>`** - Double-ended queue, better middle insertions

### Ford-Johnson Algorithm (Merge-Insert Sort)

The algorithm minimizes comparisons by using a clever insertion strategy:

#### Step 1: Pairing
```
Input: [3, 5, 9, 7, 4, 4, 4]
Pairs: (3,5), (9,7), (4,4)
Winners (larger):  [5, 9, 4]
Losers (smaller):  [3, 7, 4]
Straggler: 4 (odd element)
```

#### Step 2: Recursive Sort
```
Recursively sort winners: [5, 9, 4] → [4, 5, 9]
```

#### Step 3: Build Main Chain
```
Reorder losers to match sorted winners
sortedLosers = [4, 3, 7]
mainChain = [first_loser] + [winners] = [4, 4, 5, 9]
```

#### Step 4: Jacobsthal Insertion
```
Insert remaining losers using Jacobsthal sequence order
Jacobsthal order: 1, 3, 2, 5, 4, 11, 10, 9, 8, 7, 6, ...

This order minimizes worst-case comparisons!
```

#### Step 5: Insert Straggler
```
Binary search insert straggler
Final: [3, 4, 4, 4, 5, 7, 9]
```

### Jacobsthal Sequence
```
J(0) = 0
J(1) = 1
J(n) = J(n-1) + 2*J(n-2)

Sequence: 0, 1, 1, 3, 5, 11, 21, 43, 85, 171, ...
```

### Why Jacobsthal?
- Ensures binary search operates on optimal-sized ranges (2^k - 1)
- Minimizes worst-case comparison count
- Achieves near-optimal: `≈ n⌈log₂n⌉ - 2^⌈log₂n⌉ + 1` comparisons

### Usage
```bash
cd ex02
make
./PmergeMe 3 5 9 7 4

# Test with 3000 random elements
./PmergeMe $(shuf -i 1-100000 -n 3000 | tr "\n" " ")

# OSX alternative
./PmergeMe $(jot -r 3000 1 100000 | tr '\n' ' ')
```

### Example Output
```
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 0.00031 us
Time to process a range of 5 elements with std::deque  : 0.00014 us
```

### Performance Comparison
| Container | 3000 elements |
|-----------|---------------|
| `std::vector` | ~35,000 us |
| `std::deque` | ~300,000 us |

Vector is faster due to contiguous memory and better cache locality.

---

## Container Usage Summary

| Exercise | Container | Why This Container? |
|----------|-----------|---------------------|
| ex00 | `std::map` | O(log n) lookup with `upper_bound()`, automatic sorting by key |
| ex01 | `std::stack` | LIFO structure is perfect for RPN evaluation |
| ex02 | `std::vector` | Fast random access, efficient for sorting |
| ex02 | `std::deque` | Required second container for comparison |

---

## Compilation

Each exercise has its own Makefile:

```bash
# Compile all
cd ex00 && make && cd ..
cd ex01 && make && cd ..
cd ex02 && make && cd ..

# Clean all
cd ex00 && make fclean && cd ..
cd ex01 && make fclean && cd ..
cd ex02 && make fclean && cd ..
```

### Makefile Rules
- `make` / `make all` - Compile
- `make clean` - Remove object files
- `make fclean` - Remove all generated files
- `make re` - Recompile

---

## Project Structure

```
CPP09/
├── README.md
├── ex00/
│   ├── Makefile
│   ├── main.cpp
│   ├── BitcoinExchange.cpp
│   ├── BitcoinExchange.hpp
│   └── data.csv
├── ex01/
│   ├── Makefile
│   ├── main.cpp
│   ├── RPN.cpp
│   └── RPN.hpp
└── ex02/
    ├── Makefile
    ├── main.cpp
    ├── PmergeMe.cpp
    └── PmergeMe.hpp
```

---

## References

### Books
- **Knuth, Donald E.** "The Art of Computer Programming, Volume 3: Sorting and Searching" - Section 5.3.1 "Merge Insertion", Pages 183-186

### Papers
- **Ford, L.R. Jr. and Johnson, S.M.** "A Tournament Problem", The American Mathematical Monthly, Vol. 66, No. 5 (May 1959), pp. 387-389

### Online Resources
- [Wikipedia: Merge-insertion sort](https://en.wikipedia.org/wiki/Merge-insertion_sort)
- [Wikipedia: Reverse Polish Notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation)
- [OEIS A001045: Jacobsthal sequence](https://oeis.org/A001045)
- [cppreference: std::map](https://en.cppreference.com/w/cpp/container/map)
- [cppreference: std::stack](https://en.cppreference.com/w/cpp/container/stack)
- [cppreference: std::vector](https://en.cppreference.com/w/cpp/container/vector)
- [cppreference: std::deque](https://en.cppreference.com/w/cpp/container/deque)

---

## Author

**hpehliva** - 42 Heilbronn

---

## License

This project is part of the 42 school curriculum.

