# C++ STL Iterators, Algorithms & Custom Types

## Table of Contents

1. [The Architectural Bridge: Why Iterators Exist](#1-the-architectural-bridge-why-iterators-exist)
    - [The $M \times N$ Engineering Problem](#the-m-times-n-engineering-problem)
2. [Half-Open Ranges `[begin, end)` & Iterator Variants](#2-half-open-ranges-begin-end--iterator-variants)
    - [Why Half-Open Ranges `[begin, end)`?](#why-half-open-ranges-begin-end)
    - [Standard, Const, and Reverse Iterators](#standard-const-and-reverse-iterators)
3. [Iterator Categories & Capability Tiers](#3-iterator-categories--capability-tiers)
    - [Why Categories Matter in Practice](#why-categories-matter-in-practice)
4. [STL Algorithms & Customization via Lambdas](#4-stl-algorithms--customization-via-lambdas)
    - [Injecting Custom Logic with Lambda Expressions](#injecting-custom-logic-with-lambda-expressions)
        - [Anatomy of a Lambda Expression](#anatomy-of-a-lambda-expression)
5. [Essential STL Algorithms in Practice](#5-essential-stl-algorithms-in-practice)
    - [5.1 Searching & Counting (`std::find`, `std::count`, `std::count_if`)](#51-searching--counting-stdfind-stdcount-stdcount_if)
    - [5.2 Quantifier Checks (`std::all_of`, `std::any_of`, `std::none_of`)](#52-quantifier-checks-stdall_of-stdany_of-stdnone_of)
    - [5.3 Transforming & Filtering (`std::transform`, `std::copy_if`, `std::back_inserter`)](#53-transforming--filtering-stdtransform-stdcopy_if-stdback_inserter)
    - [5.4 Numeric Reduction (`std::accumulate`)](#54-numeric-reduction-stdaccumulate)
6. [Using Custom Types with STL Containers & Algorithms](#6-using-custom-types-with-stl-containers--algorithms)
7. [Common Beginner Pitfalls & Best Practices](#7-common-beginner-pitfalls--best-practices)

---

## 1. The Architectural Bridge: Why Iterators Exist

C++ Standard Template Library (STL) containers organize memory in fundamentally different ways:
- `std::vector` stores elements in a **contiguous block of memory**.
- `std::list` scatters elements across heap-allocated nodes linked by pointers.
- `std::set` and `std::map` organize elements inside **balanced binary search trees**.

### The $M \times N$ Engineering Problem

Suppose the STL provides $M$ containers and $N$ algorithms (searching, sorting, counting, reversing). If every algorithm were written directly against each container's internal layout, library authors would have to write and maintain $M \times N$ separate implementations.

**Iterators solve this decoupling problem.** An iterator is an object that acts as a **generalized pointer**, exposing a uniform interface regardless of how the underlying container stores data:
- `*it` accesses the element at the current position.
- `++it` advances to the next element.
- `it1 == it2` / `it1 != it2` checks whether two iterators refer to the same position.

Because STL algorithms operate exclusively on iterators rather than containers directly, complexity drops from $M \times N$ to **$M + N$**: **a single algorithm implementation can process dozens of different container types.**

---

## 2. Half-Open Ranges `[begin, end)` & Iterator Variants

Every iterable STL container exposes two boundary member functions:
- **`.begin()`**: Returns an iterator pointing directly to the **first element**.
- **`.end()`**: Returns a sentinel iterator pointing **one position past the last element**.

```text
Container:  [ 10 ]  [ 20 ]  [ 30 ]  [ 40 ]   [ PAST-THE-END ]
              ^                                     ^
          vec.begin()                           vec.end()  <-- Sentinel (DO NOT DEREFERENCE!)
```

### Why Half-Open Ranges `[begin, end)`?

Mathematically, `[begin, end)` includes `begin` and stops *immediately before* `end`. The STL standardizes on half-open ranges for three practical reasons:

1. **Empty Containers:** When a container has zero elements, `begin() == end()` naturally represents an empty range without special-case flags.
2. **Clean Loop Termination:** Traversal simply loops while `it != container.end()`.
3. **Zero-Overlap Range Splitting:** Any sub-range `[begin, mid)` and `[mid, end)` partitions a sequence cleanly without duplicating or skipping `mid`.

> **Critical Safety Rule:** Because `.end()` points *past* the last valid element, dereferencing `*vec.end()` causes **undefined behavior** (segmentation faults or silent memory corruption).

### Standard, Const, and Reverse Iterators

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> nums {10, 20, 30, 40};

    // 1. Mutable iterator: allows reading and modifying elements
    for (auto it = nums.begin(); it != nums.end(); ++it) {
        *it += 1;  // Modifies elements in-place -> {11, 21, 31, 41}
    }

    // 2. Const iterator: guarantees read-only access (use cbegin() / cend())
    for (auto it = nums.cbegin(); it != nums.cend(); ++it) {
        std::cout << *it << " ";  // Output: 11 21 31 41
    }
    std::cout << "\n";

    // 3. Reverse iterator: traverses from back to front
    // Note: ++rit moves BACKWARD toward the front of the container!
    for (auto rit = nums.rbegin(); rit != nums.rend(); ++rit) {
        std::cout << *rit << " ";  // Output: 41 31 21 11
    }
    std::cout << "\n";

    return 0;
}
```

> **Under the Hood:** Modern C++ range-based `for` loops (`for (const auto& x : nums)`) are purely syntactic sugar — the compiler lowers them into standard `.begin()` / `.end()` iterator loops behind the scenes. Notice also the loop condition `it != nums.end()`: always prefer `!=` over `<`, because linked-list and tree iterators do not support relational `<` comparisons.

---

## 3. Iterator Categories & Capability Tiers

Not all memory layouts support the same operations. Jumping forward by 5 positions (`it + 5`) is an $O(1)$ pointer offset in a contiguous `std::vector`, but impossible in a singly-linked `std::forward_list` without stepping node by node. Consequently, the STL classifies iterators into **five hierarchical capability tiers** (each tier supports all operations of the tiers above it):

| Iterator Category | Core Supported Operations | Representative Containers |
| --- | --- | --- |
| **Input Iterator** | Read-only, single-pass forward (`++`, `*it`, `==`, `!=`) | Input streams (`std::istream_iterator`) |
| **Output Iterator** | Write-only, single-pass forward (`++`, `*it = val`) | Output streams, `std::back_insert_iterator` (via `std::back_inserter`) |
| **Forward Iterator** | Multi-pass, forward-only (`++`, `==`, `!=`) | `std::forward_list`, `std::unordered_set` |
| **Bidirectional Iterator** | Moves both forward (`++`) and backward (`--`) | `std::list`, `std::set`, `std::map` |
| **Random-Access Iterator** | Arbitrary jumps and ordering (`+ n`, `- n`, `<`, `>`, `[]`) | `std::vector`, `std::deque`, `std::array`, raw pointers |

### Why Categories Matter in Practice

Every STL algorithm documents the **minimum iterator category** it requires:

- `std::find` requires only an **Input Iterator**, so it works on every iterable container.
- `std::reverse` requires a **Bidirectional Iterator**, so it works on `std::list` and `std::vector`, but fails to compile on `std::forward_list`.
- `std::sort` (typically implemented via Introsort, a hybrid of Quicksort, Heapsort, and Insertion Sort) requires a **Random-Access Iterator** for $O(1)$ mid-point indexing. Passing `std::list` iterators to `std::sort` triggers a compile-time error — which is why `std::list` provides its own `.sort()` member function that relinks nodes in-place.

> Tip: To advance any iterator safely by `n` steps regardless of whether it supports `+ n`, use `std::advance(it, n)` from `<iterator>`.

---

## 4. STL Algorithms & Customization via Lambdas

**Headers:** `#include <algorithm>` *(and `#include <numeric>` for numeric reductions like `std::accumulate`)*

The STL provides over 100 generic algorithms, broadly divided into:

1. **Non-Modifying Algorithms:** Inspect elements without mutating values or order (e.g., `std::find`, `std::count`, `std::all_of`, `std::for_each`).
2. **Modifying Algorithms:** Mutate values, copy ranges, or reorder elements (e.g., `std::sort`, `std::replace`, `std::copy`, `std::transform`, `std::reverse`).

Every STL algorithm guarantees an explicit **worst-case or average time complexity** (such as $O(n)$ linear or $O(n \log n)$ log-linear), giving you predictable performance characteristics.

### Injecting Custom Logic with Lambda Expressions

Many algorithms allow you to supply custom rules — such as counting numbers that are even or sorting objects by score. While C++ accepts function pointers and function objects (functors), modern C++ overwhelmingly uses **lambda expressions** because they keep logic concise and local to the call site.

#### Anatomy of a Lambda Expression

```cpp
[ capture_list ] ( parameter_list ) { function_body }
```

- **`[ capture_list ]`**: Controls which local variables from the enclosing scope the lambda can access (`[]` captures nothing; `[=]` captures used variables by value; `[&]` captures by reference; `[threshold]` captures a specific variable by value).
- **`( parameter_list )`**: Arguments passed into the lambda by the algorithm (typically an element from the range).
- **`{ function_body }`**: The code executed for each invocation. A callable that returns a `bool` to test a condition is known in STL terminology as a **predicate**.

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> scores {55, 72, 88, 94, 61};
    int passing_grade = 70;

    // Capture 'passing_grade' by value to check if each score passes
    int passed = std::count_if(scores.begin(), scores.end(), [passing_grade](int s) {
        return s >= passing_grade;
    });

    std::cout << "Students passed: " << passed << "\n";  // Output: 3
    return 0;
}
```

---

## 5. Essential STL Algorithms in Practice

### 5.1 Searching & Counting (`std::find`, `std::count`, `std::count_if`)

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> data {1, 2, 3, 4, 5, 6, 3};

    // 1. std::find: Returns an iterator to the first match, or data.end() if absent
    auto it = std::find(data.begin(), data.end(), 4);
    if (it != data.end()) {
        std::cout << "Found value: " << *it << "\n";
    }

    // 2. std::count: Counts exact occurrences of a value
    auto threes = std::count(data.begin(), data.end(), 3);  // 2

    // 3. std::count_if: Counts elements satisfying a predicate
    auto evens = std::count_if(data.begin(), data.end(), [](int x) {
        return x % 2 == 0;
    });  // 3

    std::cout << "Threes: " << threes << ", Evens: " << evens << "\n";
    return 0;
}
```

### 5.2 Quantifier Checks (`std::all_of`, `std::any_of`, `std::none_of`)

These algorithms short-circuit as soon as the final answer is determined:

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> scores {85, 92, 78, 95};

    bool all_passed = std::all_of(scores.begin(), scores.end(), [](int s) {
        return s >= 60;
    });

    bool has_perfect = std::any_of(scores.begin(), scores.end(), [](int s) {
        return s == 100;
    });

    std::cout << std::boolalpha
              << "All passed: " << all_passed << "\n"    // true
              << "Any 100:    " << has_perfect << "\n";  // false
    return 0;
}
```

### 5.3 Transforming & Filtering (`std::transform`, `std::copy_if`, `std::back_inserter`)

- **`std::transform`** reads each element from `[first, last)`, applies a function, and writes the result starting at a destination iterator (which may be the same range for in-place transformation).
- **`std::back_inserter(container)`** (from `<iterator>`) wraps a container in an output iterator whose assignment operator calls `container.push_back(...)`, letting algorithms grow an empty container dynamically.

```cpp
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    // Example A: Transform a string to uppercase in-place
    std::string text {"modern c++ stl"};
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });
    std::cout << text << "\n";  // Output: MODERN C++ STL

    // Example B: Filter elements > 10 into an initially empty vector
    std::vector<int> src {5, 12, 8, 25, 3, 19};
    std::vector<int> dst; // Empty!

    std::copy_if(src.begin(), src.end(), std::back_inserter(dst), [](int x) {
        return x > 10;
    });

    for (int x : dst) {
        std::cout << x << " ";  // Output: 12 25 19
    }
    std::cout << "\n";
    return 0;
}
```

### 5.4 Numeric Reduction (`std::accumulate`)

**Header:** `#include <numeric>`

```cpp
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    std::vector<double> prices {10.5, 20.25, 5.25};

    // CRITICAL: std::accumulate deduces its internal accumulator type AND return
    // type from the 3rd argument! Passing 0 (int) instead of 0.0 (double) would
    // silently truncate fractional parts at every step.
    double total = std::accumulate(prices.begin(), prices.end(), 0.0);

    std::cout << "Total: " << total << "\n";  // Output: 36
    return 0;
}
```

---

## 6. Using Custom Types with STL Containers & Algorithms

When you store user-defined `struct` or `class` objects inside STL containers or pass them to algorithms, the STL relies on standard comparison operators to establish identity and order:

- **Equality (`operator==`):** Used by `std::find`, `std::count`, and `std::replace` to determine whether two objects match.
- **Strict Weak Ordering (`operator<`):** Used by `std::sort`, `std::set`, `std::map`, and `std::priority_queue` to order elements consistently (if $a < b$, then $b < a$ must be false).

```cpp
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

class Student {
public:
    std::string name;
    int id;

    Student(std::string n, int i) : name{std::move(n)}, id{i} {}

    // Enables equality-based algorithms (std::find, std::count)
    bool operator==(const Student& other) const {
        return id == other.id;
    }

    // Enables ordering containers & algorithms (std::set, std::map, std::sort);
    // std::set treats two keys as equivalent when neither is '<' the other.
    bool operator<(const Student& other) const {
        return id < other.id;
    }
};

int main() {
    // 1. std::set automatically keeps Students ordered by id via operator<
    // (and deduplicates by equivalence, not operator==)
    std::set<Student> roster { {"Alice", 102}, {"Bob", 101} };

    // 2. For std::set, prefer member .find() (O(log n), uses equivalence via operator<)
    // over generic std::find (O(n) linear scan, uses operator==)
    Student target{"Bob", 101};
    auto it = roster.find(target);
    if (it != roster.end()) {
        std::cout << "Found: " << it->name << " (ID: " << it->id << ")\n";
    }

    // 3. Need a DIFFERENT sort order (e.g., alphabetical by name) without changing
    // operator<? Pass a custom comparator lambda as the 3rd argument to std::sort!
    std::vector<Student> vec { {"Charlie", 103}, {"Alice", 102}, {"Bob", 101} };
    std::sort(vec.begin(), vec.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });

    for (const auto& s : vec) {
        std::cout << s.name << " ";  // Output: Alice Bob Charlie
    }
    std::cout << "\n";
    return 0;
}
```

---

## 7. Common Beginner Pitfalls & Best Practices

1. **Never Dereference `.end()`:** `.end()` is a past-the-end sentinel, not an element. Always verify `if (it != container.end())` before evaluating `*it`.
2. **Watch Out for Iterator Invalidation:** Structural modifications — such as `vec.push_back()`, `vec.insert()`, or `container.erase()` — can invalidate iterators. For `std::vector`, reallocation invalidates all iterators, pointers, and references; even without reallocation, `insert` and `erase` invalidate iterators at or after the modified position. Always re-acquire iterators after modifying container structure.
3. **Prefer Container Member Functions Over Generic Algorithms:** When an associative or node-based container provides its own member function — such as `my_set.find(key)`, `my_map.find(key)`, or `my_list.sort()` — use it. Generic `std::find` performs a linear $O(n)$ scan, whereas `std::set::find` leverages the tree structure for $O(\log n)$ lookup.
4. **Match the Initial Value Type in `std::accumulate`:** Always match the literal type of the third argument (`0` vs. `0.0` vs. `0LL`) to your desired sum type to prevent silent decimal truncation or integer overflow.
5. **Never Write Past the End of an Empty Container:** Passing `dst.begin()` of an empty `std::vector` into `std::copy` or `std::transform` writes into unallocated memory. Either pre-allocate space with `dst.resize(src.size())` or wrap the destination in `std::back_inserter(dst)`.
