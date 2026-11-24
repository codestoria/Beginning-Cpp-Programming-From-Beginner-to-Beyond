# C++ Generic Programming & Templates

## Table of Contents

1. [Introduction to the C++ STL](#1-introduction-to-the-c-stl)
2. [What Is Generic Programming?](#2-what-is-generic-programming)
3. [Attempt 1: Preprocessor Macros (And Why They Fail)](#3-attempt-1-preprocessor-macros-and-why-they-fail)
4. [Attempt 2: Function Templates (The Modern C++ Way)](#4-attempt-2-function-templates-the-modern-c-way)
5. [Class Templates & The Header-Only Rule](#5-class-templates--the-header-only-rule)
6. [Non-Type Template Parameters (`std::array` Motivation)](#6-non-type-template-parameters-stdarray-motivation)
7. [Summary & Key Takeaways](#7-summary--key-takeaways)

---

## 1. Introduction to the C++ STL

The **C++ Standard Template Library (STL)** is a foundational collection of reusable, type-safe, and high-performance components built directly into the C++ standard. Originally architected by Alexander Stepanov in the early 1990s, the STL provides ready-to-use data structures (such as dynamic arrays and hash tables) and algorithms (such as sorting and binary search).

### Why Learn the STL?

Without the STL, every time you needed a dynamically resizing array or a sorting routine, you would have to implement it from scratch—risking memory leaks, subtle edge-case bugs, and inconsistent performance. The STL eliminates this burden by offering production-grade components refined over decades.

More importantly, the STL is built around a **unified architectural model** consisting of three decoupled parts:

* **Containers** store and manage collections of data (e.g., `std::vector`, `std::map`).
* **Iterators** act as generalized pointers that provide a uniform interface to traverse container elements.
* **Algorithms** process data ranges through iterators without needing to know what underlying container holds the data (e.g., `std::sort`, `std::find`).

Because these three components are decoupled, learning how one container, one iterator, and one algorithm interact gives you the mental model required to use almost the entire library.

---

## 2. What Is Generic Programming?

To understand *how* the STL achieves massive code reuse without sacrificing C++'s signature execution speed, you must first understand **generic programming**.

Bjarne Stroustrup (the creator of C++) defines generic programming as:
> *"Writing code that works with a variety of types as arguments, as long as those argument types meet specific syntactic and semantic requirements."*

### The Core Problem: Static Typing vs. Code Duplication

C++ is a **statically typed** language: the compiler must know the exact data type of every variable and function parameter at compile time. While static typing catches type errors early and enables aggressive compiler optimizations, it creates friction when writing general-purpose logic.

Suppose you want a function that returns the larger of two values. For integers, you write:

```cpp
int max_value(int a, int b) {
    return (a > b) ? a : b;
}

```

What if you also need to compare `double` values, `char` values, or custom `Player` objects? Without generic programming, you are forced to write and maintain separate overloaded functions for every single type:

```cpp
double max_value(double a, double b) { return (a > b) ? a : b; }
char max_value(char a, char b)       { return (a > b) ? a : b; }
// ...and dozens more overloads for every other type!

```

This violates the **DRY (Don't Repeat Yourself)** principle. Every overload duplicates identical logic; if you discover a bug or want to refine the algorithm, you must update every copy manually.

**The Engineering Goal:** Write **a single, type-independent code blueprint** and let the compiler automatically generate type-specific machine code on demand **at compile time**—preserving full type safety with **zero runtime overhead**.

---

## 3. Attempt 1: Preprocessor Macros (And Why They Fail)

Before templates were added to C++, programmers inherited **preprocessor macros** from the C language to simulate generic code. Understanding why macros fail clarifies why templates were invented.

### How Macros Work

The C++ preprocessor is a text-processing tool that runs *before* actual compilation begins. When you define a macro using `#define`, the preprocessor performs **blind text substitution**. It has zero knowledge of C++ data types, variable scopes, or operator precedence.

Consider a macro implementation of `MAX`:

```cpp
// Legacy C style — strictly discouraged in modern C++
#define MAX(a, b) ((a) > (b) ? (a) : (b))

```

Notice the defensive parentheses wrapped around every parameter and around the entire expression. Without them, even basic arithmetic fails. Yet even *with* careful parentheses, macros suffer from three fatal flaws:

1. **No Type Safety:** Comparing incompatible types (e.g., `MAX(10, "hello")`) is not caught at the macro call site; the preprocessor blindly pastes the text, resulting in cryptic compiler errors inside the expanded expression.
2. **Operator Precedence Bugs:** Consider a naive macro meant to square a number:
```cpp
#define SQUARE(a) a * a

int result = 100 / SQUARE(2 + 3); 
// Blindly expands to: 100 / 2 + 3 * 2 + 3
// Evaluates as: (100 / 2) + (3 * 2) + 3 = 59 (Expected: 4!)

```


3. **Multiple Evaluation Side Effects:** Calling `MAX(++x, ++y)` expands to `((++x) > (++y) ? (++x) : (++y))`. Whichever variable is larger gets evaluated—and therefore incremented—**twice**, corrupting program state.

### When Are Macros Acceptable Today?

In modern C++, macros should be restricted to header include guards (`#ifndef` / `#define`) and conditional compilation (such as platform-specific code). **Never use macros to define constants (`constexpr` replaces them) or generic functions (templates replace them).**

---

## 4. Attempt 2: Function Templates (The Modern C++ Way)

C++ solves the generic programming problem cleanly through **templates**. A **function template** is a compile-time blueprint that instructs the compiler on how to generate a concrete function when invoked with specific types.

### Syntax & Compilation Model

```cpp
template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

```

* **`template <typename T>`** introduces a **template parameter list**, telling the compiler that `T` is a placeholder for a type to be supplied later. *(Note: You may see `template <class T>` in older codebases. In a template parameter list, `typename` and `class` are interchangeable, though `typename` is often preferred for clarity because `T` can be a primitive type like `int`; `typename` also has a distinct role when naming dependent types inside templates.)*
* **Crucial Mental Model:** A template by itself generates **no executable machine code**. It is purely a blueprint stored in compiler memory. Concrete functions are only generated when you **instantiate** (use) the template with actual types.

### Template Instantiation (Explicit vs. Deduced)

You can invoke a function template in two ways:

```cpp
#include <iostream>

template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    // 1. Explicit instantiation: you explicitly specify T = int
    int res1 = max_value<int>(10, 20);

    // 2. Implicit instantiation (Template Argument Deduction): 
    // The compiler deduces T = double from the arguments
    double res2 = max_value(3.14, 2.71);

    // Note: max_value(10, 3.14) will NOT compile because T cannot be
    // deduced as both int and double simultaneously. Specifying <double>
    // bypasses deduction and converts arguments to double:
    double res3 = max_value<double>(10, 3.14);

    std::cout << res1 << " | " << res2 << " | " << res3 << "\n";
    return 0;
}

```

**Output:**

```text
20 | 3.14 | 10

```

Behind the scenes, the compiler generates two completely independent, type-safe functions—one accepting `int` and one accepting `double`—just as if you had written both overloads by hand.

### Type Requirements (Syntactic & Semantic Contracts)

Because templates are compiled against concrete types during instantiation, **any type substituted for `T` must support every operation performed on `T` inside the template body.**

Look back at `max_value`: its body executes `a > b`.

* Fundamental types (`int`, `double`, `char`) support `>` natively.
* If you pass a user-defined `struct` or `class`, compilation will **fail** unless that type defines `operator>`:

```cpp
#include <iostream>
#include <string>

template <typename T>
T max_value(T a, T b) {
    return (a > b) ? a : b;
}

struct Player {
    std::string name;
    int score;

    // Define operator> so Player satisfies max_value's syntactic requirement
    bool operator>(const Player& other) const {
        return score > other.score;
    }
};

int main() {
    Player p1{"Alice", 150};
    Player p2{"Bob", 200};

    Player winner = max_value(p1, p2); // Compiles cleanly!
    std::cout << "Winner: " << winner.name << "\n";
    return 0;
}

```

**Output:**

```text
Winner: Bob

```

### Multiple Template Parameters

Templates are not restricted to a single type parameter. You can declare multiple independent placeholder types, and pass arguments by `const` reference (`const T&`) to avoid expensive copies when `T` is a large object like `std::string`:

```cpp
#include <iostream>
#include <string>

template <typename T1, typename T2>
void print_pair(const T1& first, const T2& second) {
    std::cout << "[" << first << ", " << second << "]\n";
}

int main() {
    print_pair(42, std::string{"The Answer"}); // T1 = int, T2 = std::string
    return 0;
}

```

---

## 5. Class Templates & The Header-Only Rule

Just as function templates act as blueprints for functions, **class templates** act as blueprints for classes. Every STL container—such as `std::vector<int>` or `std::map<std::string, double>`—is a class template.

### Building a Generic Class

Suppose you want a container class that pairs a string label with a value of *any* data type:

```cpp
#include <iostream>
#include <string>

template <typename T>
class Item {
private:
    std::string name;
    T value;

public:
    Item(std::string item_name, T item_value)
        : name{std::move(item_name)}, value{std::move(item_value)} {}

    const std::string& get_name() const { return name; }
    const T& get_value() const { return value; }
};

int main() {
    // Explicitly specifying the template argument:
    Item<int> score_item{"High Score", 9500};
    Item<std::string> title_item{"Course", "C++ STL"};

    // Since C++17, Class Template Argument Deduction (CTAD) can also deduce T:
    Item auto_item{"Pi", 3.14159}; // Deduces Item<double>

    std::cout << score_item.get_name() << ": " << score_item.get_value() << "\n";
    std::cout << title_item.get_name() << ": " << title_item.get_value() << "\n";
    std::cout << auto_item.get_name() << ": " << auto_item.get_value() << "\n";
    return 0;
}

```

To the compiler, `Item<int>`, `Item<std::string>`, and `Item<double>` are **three completely distinct classes** generated from the same blueprint. You can also compose templates arbitrarily, such as `Item<Item<double>>` or `std::vector<Item<int>>`.

### Why Templates Must Live in Header Files

In standard C++ projects, class declarations go in a header file (`.h` / `.hpp`) and member function definitions go in a separate source file (`.cpp`). **Templates break this convention: both declarations and full definitions must reside in the header file.**

**Why?** Remember that C++ compiles each `.cpp` file independently (as a separate *translation unit*):

1. If `Item`'s member definitions are hidden inside `Item.cpp`, the compiler compiles `Item.cpp` on its own. Because `Item.cpp` does not know which concrete types `main.cpp` will use (e.g., `Item<int>`), it generates **zero machine code** for those member functions.
2. Later, when `main.cpp` instantiates `Item<int>`, it only sees the declarations from `Item.h` and trusts that the linker will find the compiled definitions elsewhere.
3. During linking, the linker searches for `Item<int>::get_value()`, finds nothing in `Item.o`, and fails with an **`undefined reference` (linker error)**.

Keeping the complete template definition inside the header ensures that every `.cpp` file including that header has access to the full blueprint at compile time. *(In larger codebases, C++11's `extern template` can also be used to control instantiation overhead, though header definitions remain the baseline rule.)*

---

## 6. Non-Type Template Parameters (`std::array` Motivation)

So far, our template parameters have represented **data types** (`typename T`). However, C++ templates can also accept **compile-time constant values** (such as integers, booleans, or pointers), known as **non-type template parameters**.

### The Motivation: Fixed-Size Stack Arrays

Suppose you want to design a safe, fixed-size array wrapper that stores its elements on the fast **stack** rather than allocating dynamic memory on the **heap**. To reserve stack memory, the compiler *must* know the exact byte size of the array **at compile time**.

Passing the size through a constructor parameter will not work because constructor arguments are evaluated at *runtime*:

```cpp
// ❌ Impossible for stack allocation:
class BrokenArray {
public:
    BrokenArray(std::size_t n) {
        int data[n]; // Standard C++ forbids variable-length stack arrays!
    }
};

```

The solution is to pass the size as a **non-type template parameter**, making the size part of the type itself:

```cpp
#include <cstddef> // for std::size_t
#include <iostream>
#include <string>

// T is a type parameter; N is a non-type compile-time constant parameter
template <typename T, std::size_t N>
class FixedArray {
private:
    T data[N]; // Valid! N is a compile-time constant.

public:
    constexpr std::size_t size() const { return N; }

    T& operator[](std::size_t index) { return data[index]; }
    const T& operator[](std::size_t index) const { return data[index]; }
};

int main() {
    FixedArray<int, 5> numbers;         // Allocates 5 ints on the stack
    FixedArray<std::string, 3> words;   // Allocates 3 strings on the stack

    numbers[0] = 42;
    words[0] = "Hello";

    std::cout << "numbers size: " << numbers.size() << ", first: " << numbers[0] << "\n";
    std::cout << "words size:   " << words.size()   << ", first: " << words[0]   << "\n";
    return 0;
}

```

Note that `FixedArray<int, 5>` and `FixedArray<int, 10>` are treated as **two different types** because their non-type template arguments differ.

### Real-World Application: `std::array<T, N>`

This exact mechanism powers C++11's **`std::array<T, N>`**. Whenever you manage a collection whose size is fixed and known at compile time (such as the 3 coordinates of a 3D vector or the 12 months of a year), `std::array` gives you the zero-overhead stack performance of a raw C array combined with STL safety features like `.size()`, `.at()` bounds checking, and iterator support.

---

## 7. Summary & Key Takeaways

1. **Generic Programming** lets you write a single type-independent blueprint that the compiler instantiates into concrete, type-safe code at compile time—eliminating code duplication with zero runtime overhead.
2. **Avoid Preprocessor Macros** for generic logic; macros perform blind text substitution without respecting C++ types, scoping, operator precedence, or single evaluation.
3. **Compile-Time Blueprints:** Function and class templates produce no machine code until they are instantiated with concrete types (either explicitly via `<T>` or via compiler type deduction).
4. **Type Requirements:** Any type substituted for a template parameter must support every operation (such as `>`, `==`, or `<<`) executed inside the template body.
5. **Header-Only Organization:** Keep both template declarations and implementations in header files (`.h` / `.hpp`) so the compiler has the full blueprint available in every translation unit that instantiates it.
6. **Non-Type Template Parameters** accept compile-time constant values (like array dimensions), enabling stack-allocated containers such as `std::array<T, N>`.
