# C++ STL Containers in Depth

## Table of Contents

1. [Overview of STL Container Families](#1-overview-of-stl-container-families)
2. [Common Container Operations & Rules](#2-common-container-operations--rules)
    - [Value Semantics & Automatic RAII Cleanup](#value-semantics--automatic-raii-cleanup)
3. [Sequence Containers (Order-Preserving)](#3-sequence-containers-order-preserving)
    - [3.1 `std::vector` (Dynamic Contiguous Array)](#31-stdvector-dynamic-contiguous-array)
        - [Why `size()` Differs from `capacity()` (Amortized $O(1)$ Growth)](#why-size-differs-from-capacity-amortized-o1-growth)
        - [Performance Profile & Key Operations](#performance-profile--key-operations)
    - [3.2 `std::deque` (Double-Ended Queue)](#32-stddeque-double-ended-queue)
        - [The Motivation](#the-motivation)
        - [Performance Profile & Key Operations](#performance-profile--key-operations-1)
    - [3.3 `std::list` & `std::forward_list` (Linked Lists)](#33-stdlist--stdforward_list-linked-lists)
        - [The Motivation](#the-motivation-1)
        - [`std::list` (Doubly-Linked List)](#stdlist-doubly-linked-list)
        - [`std::forward_list` (Singly-Linked List)](#stdforward_list-singly-linked-list)
4. [Associative Containers (Key-Based Lookup)](#4-associative-containers-key-based-lookup)
    - [4.1 `std::set` & `std::multiset`](#41-stdset--stdmultiset)
    - [4.2 `std::map` & `std::multimap`](#42-stdmap--stdmultimap)
5. [Container Adapters (Restricted Interfaces)](#5-container-adapters-restricted-interfaces)
    - [5.1 `std::stack` (LIFO)](#51-stdstack-lifo)
    - [5.2 `std::queue` (FIFO)](#52-stdqueue-fifo)
    - [5.3 `std::priority_queue` (Heap)](#53-stdpriority_queue-heap)
6. [How to Choose the Right Container](#6-how-to-choose-the-right-container)
    - [Time-Complexity Quick Reference](#time-complexity-quick-reference)
    - [Practical Decision Checklist](#practical-decision-checklist)

---

## 1. Overview of STL Container Families

An **STL container** is a generic class template designed to store, organize, and manage the lifetime of a collection of objects.

In software engineering, no single data structure excels at every operation: a layout that makes random indexing instantaneous may make inserting into the middle slow, and vice versa. To let you pick the exact memory layout your algorithm needs, the C++ Standard Template Library organizes containers into **three distinct families**:

| Family | Core Organizing Principle | Standard Containers | Supports Iterators? |
| :--- | :--- | :--- | :--- |
| **Sequence Containers** | Preserves the exact linear order in which elements are inserted. | `std::array`, `std::vector`, `std::deque`, `std::list`, `std::forward_list` | **Yes** |
| **Associative Containers** | Automatically organizes elements by a **key** for fast lookup (using balanced binary trees or hash tables). | **Ordered (Tree):** `std::set`, `std::multiset`, `std::map`, `std::multimap`<br>**Unordered (Hash):** `std::unordered_set`, `std::unordered_map`, etc. | **Yes** |
| **Container Adapters** | Wraps an underlying sequence container to enforce a restricted, specialized access discipline. | `std::stack`, `std::queue`, `std::priority_queue` | **No** |

---

## 2. Common Container Operations & Rules

Because the STL is built around uniform interfaces, most containers share a consistent vocabulary of member functions:

- **Capacity Queries:**
  - `.size()` returns the number of elements currently stored.
  - `.empty()` returns `true` if the container holds zero elements. *(Best practice: Always prefer `.empty()` over `.size() == 0`, as `.empty()` is guaranteed $O(1)$ across every standard container.)*
- **Modifiers:**
  - `.insert(...)` and `.erase(...)` add or remove elements.
  - `.clear()` destroys all elements, leaving `.size() == 0`.
  - `.swap(other)` exchanges the internal buffers of two containers of the exact same type in $O(1)$ time (by swapping internal pointers rather than copying elements).
- **Boundary Iterators:** `.begin()` points to the first element; `.end()` points **one position past** the last element (never dereference `.end()`).
- **Lexicographical Comparisons:** Operators `==`, `!=`, `<`, `<=`, `>`, and `>=` compare two containers element-by-element from beginning to end.

### Value Semantics & Automatic RAII Cleanup

STL containers operate using **value semantics**: when you add an object to a container, **the container stores its own copy** (or moves the object into its storage via move semantics). Consequently:
1. Any custom class stored in a container must be copy-constructible or move-constructible (move-only types like `std::unique_ptr` work with move/emplace operations, though operations requiring copies will not compile).
2. The container owns its elements' lifetimes — when the container goes out of scope or is destroyed, it automatically invokes the destructor of every stored element and releases all heap memory (**RAII**), preventing memory leaks.

---

## 3. Sequence Containers (Order-Preserving)

Sequence containers maintain elements in a linear sequence. Their fundamental differences stem from **how they arrange memory**, which directly governs their time complexity and iterator stability.

### 3.1 `std::vector` (Dynamic Contiguous Array)

**Header:** `#include <vector>`

`std::vector` is the default workhorse container in C++. Internally, it manages a single **contiguous block of heap memory** (identical in layout to a raw C array), giving CPUs optimal cache locality while automatically resizing as elements are added.

```text
Contiguous Memory:  [ 10 | 20 | 30 | __ | __ ]
                     ^              ^         ^
                  begin()         end()    capacity()
                     |<-- size() -->|
```

#### Why `size()` Differs from `capacity()` (Amortized $O(1)$ Growth)

Allocating new heap memory on every single `.push_back()` would be prohibitively slow. Instead, `std::vector` separates logical length from allocated storage:

- **`.size()`**: The number of active elements currently inside the vector.
- **`.capacity()`**: The maximum number of elements the currently allocated memory block can hold before needing a reallocation.

When `.size() == .capacity()` and you append another element:

1. `std::vector` allocates a new, larger contiguous buffer (typically growing geometrically by $1.5\times$ or $2\times$).
2. It moves (or copies) all existing elements into the new buffer.
3. It destroys the old elements and frees the old memory block.

Because geometric growth ensures expensive reallocations happen exponentially less often as the vector grows, appending at the back achieves **amortized $O(1)$** time complexity.

> **Crucial Side Effect — Iterator Invalidation:**
> Whenever a `std::vector` reallocates its buffer, all elements move to new memory addresses. **Every existing pointer, reference, and iterator pointing into that vector becomes dangling (invalid).** If you know roughly how many elements you will insert upfront, call `vec.reserve(N)` to pre-allocate capacity and eliminate reallocations.

#### Performance Profile & Key Operations

- **Random Access ($O(1)$):** `vec[i]` performs direct pointer arithmetic (`base + i`) with zero overhead (out-of-bounds access causes undefined behavior). `vec.at(i)` checks bounds and throws `std::out_of_range` if `i >= vec.size()`.
- **Back Operations ($O(1)$ amortized):** `vec.push_back(val)` appends an element; `vec.emplace_back(args...)` constructs the element directly inside the vector's memory buffer, avoiding temporary objects; `vec.pop_back()` removes the last element.
- **Front/Middle Insert & Erase ($O(n)$):** Slow, because preserving memory contiguity requires shifting every subsequent element left or right.

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec {10, 20, 30};
    vec.reserve(10);       // Pre-allocates capacity for 10 ints (size remains 3)
    vec.push_back(40);     // Fast O(1) append without reallocation
    vec.emplace_back(50);  // Constructs 50 directly in-place

    std::cout << "Index 1: " << vec.at(1) << "\n"; // Output: 20
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << "\n";
    return 0;
}
```

---

### 3.2 `std::deque` (Double-Ended Queue)

**Header:** `#include <deque>` *(pronounced "deck")*

#### The Motivation

`std::vector` appends at the back in $O(1)$ time, but inserting at the front (`vec.insert(vec.begin(), val)`) takes $O(n)$ time because every element must shift right. `std::deque` solves this by abandoning a single contiguous array in favor of a **sequence of fixed-size memory chunks** tracked by an internal pointer table (map).

```text
Map Array:   [ ptr0 ] --------> [ __ | __ | 10 | 20 ]  (Chunk 0: room to grow left!)
             [ ptr1 ] --------> [ 30 | 40 | 50 | __ ]  (Chunk 1: room to grow right!)
```

When you push to either end and the edge chunk is full, `std::deque` simply allocates a new fixed-size chunk and links it into the map — never copying existing elements.

#### Performance Profile & Key Operations

- **Both Ends ($O(1)$ amortized):** Supports `push_back()`, `pop_back()`, `emplace_back()` and `push_front()`, `pop_front()`, `emplace_front()` (occasional pointer-table growth involves copying pointers).
- **Random Access ($O(1)$):** Supports `[]` and `.at()`, though slightly slower than `std::vector` due to two-step pointer indirection (map lookup + chunk offset).
- **Middle Insert & Erase ($O(n)$):** Still requires shifting elements within chunks, and invalidates existing iterators.

```cpp
#include <deque>
#include <iostream>

int main() {
    std::deque<int> dq {20, 30};
    dq.push_front(10);  // amortized O(1) front insertion -> {10, 20, 30}
    dq.push_back(40);   // amortized O(1) back insertion  -> {10, 20, 30, 40}

    std::cout << "Front: " << dq.front() << ", Back: " << dq.back() << "\n";
    return 0;
}
```

---

### 3.3 `std::list` & `std::forward_list` (Linked Lists)

**Headers:** `#include <list>`, `#include <forward_list>`

#### The Motivation

If an application frequently inserts or removes items in the **middle** of a large collection while holding iterators to those positions, shifting elements in `std::vector` or `std::deque` becomes a bottleneck. Linked lists solve this by allocating each element in its own independent heap node connected via pointers.

#### `std::list` (Doubly-Linked List)

Each node stores a value alongside two pointers: `prev` and `next`.

- **Insert/Erase Anywhere ($O(1)$):** Given an iterator to a position, inserting or erasing only rewires adjacent pointers — no elements ever move in memory.
- **Strong Iterator Stability:** Inserting or erasing elements **never invalidates** iterators or references to other elements (only iterators pointing directly to an erased node become invalid).
- **No Random Access ($O(n)$ traversal):** Because nodes are scattered across the heap, `[]` and `.at()` do not exist. Reaching the $k$-th element requires walking $k$ pointers sequentially.
- **Memory & Cache Trade-off:** Every node incurs two pointers of memory overhead (16 bytes on 64-bit systems), and non-contiguous memory causes frequent CPU cache misses — meaning iterating over a `std::list` is noticeably slower than iterating over a `std::vector`.
- **Member Sort:** Always call `my_list.sort()` instead of `std::sort()`, because `std::sort()` requires random-access iterators.

#### `std::forward_list` (Singly-Linked List)

A minimal linked list designed for strict memory constraints, where each node stores only a `next` pointer.

- Moves forward only (`push_front`, `pop_front`).
- Provides **no `.back()`**, **no `.push_back()`**, and **no `.size()`** method (omitting the size counter avoids extra bookkeeping overhead on list node operations and `splice_after`).
- Inserts and erases elements *after* a given iterator via `.insert_after()` and `.erase_after()`.

---

## 4. Associative Containers (Key-Based Lookup)

While sequence containers locate elements by *position*, associative containers organize and retrieve elements by **key**. Internally, `std::set` and `std::map` are almost always implemented as **Red-Black Trees** (self-balancing binary search trees), guaranteeing **$O(\log n)$** search, insertion, and deletion.

> **Key Ordering Requirement:** Because balanced search trees keep keys sorted automatically, they compare keys using `operator<` by default. If you use a custom `struct` or `class` as a key, you must either overload `operator<` for that type or supply a custom comparator.
> *(Note: C++11 also provides `std::unordered_set` and `std::unordered_map`, which use hash tables to achieve **$O(1)$ average** lookup without maintaining sorted order.)*

### 4.1 `std::set` & `std::multiset`

**Header:** `#include <set>`

A `std::set` stores a collection of **unique, sorted keys** (the element itself is the key).

- **Automatic Deduplication:** Inserting an existing key is a no-op. `.insert(val)` returns a `std::pair<iterator, bool>`, where `.second` is `true` if insertion succeeded and `false` if the key already existed.
- **Logarithmic Lookup ($O(\log n)$):** Always prefer the member function `my_set.find(key)` over the generic `std::find()`. The member function walks the binary tree in $O(\log n)$ steps, whereas generic `std::find()` performs a linear $O(n)$ scan.
- **Read-Only Keys:** Elements in a `std::set` are immutable (`const`). Modifying a key in-place could violate the binary search tree's sorted invariant; instead, erase the old value and insert the updated one.

**`std::multiset`** behaves identically to `std::set`, except it **permits duplicate keys**. Calling `.count(key)` returns the exact number of matching elements.

```cpp
#include <iostream>
#include <set>

int main() {
    std::set<int> ids {30, 10, 20, 10};  // Duplicate 10 is ignored; stored as {10, 20, 30}

    auto [it, inserted] = ids.insert(25);  // C++17 structured binding
    if (inserted) {
        std::cout << "Inserted: " << *it << "\n";
    }

    if (ids.find(20) != ids.end()) {
        std::cout << "ID 20 exists in the set.\n";
    }
    return 0;
}
```

---

### 4.2 `std::map` & `std::multimap`

**Header:** `#include <map>`

A `std::map` implements a sorted associative dictionary storing **key-value pairs** of type `std::pair<const Key, Value>`. Keys are unique and sorted; mapped values can be duplicated and modified freely.

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> grades { {"Alice", 92}, {"Bob", 85} };

    // Insert a new pair or overwrite an existing value using []
    grades["Charlie"] = 88;
    grades["Bob"] = 90;

    // Safe read-only lookup using .find()
    auto it = grades.find("Alice");
    if (it != grades.end()) {
        // it->first is the const Key, it->second is the Value
        std::cout << it->first << "'s grade: " << it->second << "\n";
    }

    // Iterating in sorted key order (C++17 structured bindings)
    for (const auto& [student, score] : grades) {
        std::cout << student << ": " << score << "\n";
    }
    return 0;
}
```

> **Classic Beginner Pitfall — `map[key]` Mutates the Map:**
> Evaluating `int x = grades["Unknown"];` when `"Unknown"` is not in the map **automatically inserts `"Unknown"` with a default-constructed value (`0`)** and returns a reference to it! Consequently, `operator[]` cannot be used on a `const std::map`. Whenever you want to query a key without modifying the map, use `.find(key)`, `.contains(key)` (C++20+), or `.at(key)` (which throws `std::out_of_range` if the key does not exist).

**`std::multimap`** allows multiple values to share the same key. Because a single key can correspond to several entries, `std::multimap` **does not provide `operator[]` or `.at()`**; you insert with `.insert({key, val})` and locate ranges with `.equal_range(key)`.

---

## 5. Container Adapters (Restricted Interfaces)

Container adapters are not standalone data structures with new memory layouts. Instead, they **wrap an existing sequence container** (such as `std::deque` or `std::vector`) and expose only the operations permitted by a specific abstract data type.

> **Key Limitation:** Container adapters **do not expose iterators**. You cannot iterate over them with a range-based `for` loop or pass them to STL algorithms.

### 5.1 `std::stack` (LIFO)

**Header:** `#include <stack>`

Enforces **Last-In, First-Out (LIFO)** access — like a vertical stack of plates. Wraps `std::deque` by default.

- `.push(val)` / `.emplace(args...)`: Places a new element onto the top of the stack ($O(1)$).
- `.top()`: Returns a reference to the top element without removing it ($O(1)$).
- `.pop()`: Removes the top element ($O(1)$). *(Note: `.pop()` returns `void` rather than the popped value to guarantee strong exception safety — call `.top()` first if you need the value.)*

### 5.2 `std::queue` (FIFO)

**Header:** `#include <queue>`

Enforces **First-In, First-Out (FIFO)** access — like a ticket queue. Wraps `std::deque` by default.

- `.push(val)` / `.emplace(args...)`: Adds an element to the **back** of the queue ($O(1)$).
- `.front()` / `.back()`: Inspects the oldest or newest element without removing it ($O(1)$).
- `.pop()`: Removes the element at the **front** of the queue ($O(1)$, returns `void`).

### 5.3 `std::priority_queue` (Heap)

**Header:** `#include <queue>`

Wraps `std::vector` by default and maintains elements in a **binary max-heap**. Regardless of insertion order, the **largest element is always accessible at `.top()`**:

- `.push(val)` / `.emplace(args...)`: Inserts an element and restores the heap invariant in **$O(\log n)$** time.
- `.top()`: Inspects the maximum element in **$O(1)$** time.
- `.pop()`: Removes the maximum element and re-heapifies in **$O(\log n)$** time.

```cpp
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

int main() {
    // 1. Default max-heap: largest element is always at .top()
    std::priority_queue<int> max_pq;
    max_pq.push(15);
    max_pq.push(40);
    max_pq.push(25);
    std::cout << "Max top: " << max_pq.top() << "\n";  // Output: 40

    // 2. Min-heap: supply underlying container (std::vector<int>) and std::greater<int>
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(15);
    min_pq.push(40);
    min_pq.push(25);
    std::cout << "Min top: " << min_pq.top() << "\n";  // Output: 15
    return 0;
}
```

---

## 6. How to Choose the Right Container

### Time-Complexity Quick Reference

| Container | Random Access | Insert/Erase (Front) | Insert/Erase (Back) | Insert/Erase (Middle / By Key) | Key Lookup |
| --- | --- | --- | --- | --- | --- |
| **`std::vector`** | $O(1)$ | $O(n)$ | $O(1)$ amortized | $O(n)$ | $O(n)$ unsorted / $O(\log n)$ sorted |
| **`std::deque`** | $O(1)$ | $O(1)$ amortized | $O(1)$ amortized | $O(n)$ | $O(n)$ |
| **`std::list`** | — | $O(1)$ | $O(1)$ | $O(1)$ *(with iterator)* | $O(n)$ |
| **`std::set` / `std::map`** | — | — | — | $O(\log n)$ | $O(\log n)$ |
| **`std::unordered_set` / `_map`** | — | — | — | $O(1)$ avg / $O(n)$ worst | $O(1)$ avg / $O(n)$ worst |
| **`std::stack` / `std::queue`** | — | `push`/`pop` $O(1)$ *(queue)* | `push`/`pop` $O(1)$ *(stack/queue)* | — | — |
| **`std::priority_queue`** | — | — | `push`/`pop` $O(\log n)$ | — | `top()` $O(1)$ |

### Practical Decision Checklist

1. **Default Choice:** Start with **`std::vector`**. Contiguous memory makes it overwhelmingly fast on modern CPU caches for almost all general tasks.
2. **Fixed Size Known at Compile Time:** Use **`std::array<T, N>`** for zero heap overhead.
3. **Frequent Insertions/Removals at Both Front and Back:** Use **`std::deque`**.
4. **Frequent Insertions/Removals in the Middle (with existing iterators) or Pointer Stability Required:** Use **`std::list`**.
5. **Fast Lookup of Unique Keys:** Use **`std::set`** if you need elements kept in sorted order, or **`std::unordered_set`** if you only care about $O(1)$ average lookup speed.
6. **Key-Value Dictionary Lookup:** Use **`std::map`** for sorted keys, or **`std::unordered_map`** for maximum average lookup speed.
7. **Strict LIFO, FIFO, or Priority Discipline:** Use **`std::stack`**, **`std::queue`**, or **`std::priority_queue`**.
