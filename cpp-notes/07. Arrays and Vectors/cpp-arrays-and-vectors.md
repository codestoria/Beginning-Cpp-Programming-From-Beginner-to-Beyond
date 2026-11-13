# C++ Arrays and Vectors

## Table of Contents

1. [Introduction: Why We Need Compound Data Types](#1-introduction-why-we-need-compound-data-types)
2. [Arrays](#2-arrays)
   - [2.1 What Is an Array?](#21-what-is-an-array)
   - [2.2 Why Do We Need Arrays?](#22-why-do-we-need-arrays)
   - [2.3 Characteristics of Arrays](#23-characteristics-of-arrays)
   - [2.4 Declaring Arrays](#24-declaring-arrays)
   - [2.5 Initializing Arrays](#25-initializing-arrays)
   - [2.6 Accessing Array Elements](#26-accessing-array-elements)
   - [2.7 How Arrays Work in Memory](#27-how-arrays-work-in-memory)
   - [2.8 Multi-dimensional Arrays](#28-multi-dimensional-arrays)
3. [Vectors](#3-vectors)
   - [3.1 The Problem with Fixed-Size Arrays](#31-the-problem-with-fixed-size-arrays)
   - [3.2 What Is a Vector?](#32-what-is-a-vector)
   - [3.3 Declaring and Initializing Vectors](#33-declaring-and-initializing-vectors)
   - [3.4 Accessing Vector Elements](#34-accessing-vector-elements)
   - [3.5 Growing a Vector Dynamically](#35-growing-a-vector-dynamically)
   - [3.6 Bounds Checking and Exceptions](#36-bounds-checking-and-exceptions)
   - [3.7 Two-dimensional Vectors](#37-two-dimensional-vectors)
4. [Arrays vs. Vectors: A Comparison](#4-arrays-vs-vectors-a-comparison)
5. [Summary](#5-summary)

---

## 1. Introduction: Why We Need Compound Data Types

So far, you have worked with **primitive** (also called **fundamental**) data types such as `int`, `double`, and `char`. Each variable of these types holds exactly one value:

```cpp
int score1 = 100;
int score2 = 95;
int score3 = 99;
```

This approach works fine when you have only a few values. But what if you need to store 100 test scores? Or 10,000? Declaring and naming 10,000 individual variables is impractical, tedious, and error-prone. You would have to keep track of 10,000 different variable names, and any operation that applies to “all scores” would require writing 10,000 lines of code. Clearly, we need a better way to group related values together.

This is where **compound data types** (also called **structured data types**) come in. A compound type is a data type built from other data types. For example, a collection of test scores — each of which is an integer — is a compound type made up of integers.

In this tutorial, we will study two of the most important compound data types in C++:

- **Arrays** — the traditional, fixed-size collection type inherited from C
- **Vectors** — the modern, dynamic, object-oriented collection type from the C++ Standard Library

Understanding both is essential. Arrays help you read and maintain legacy C++ code and give you a conceptual foundation for how collections work. Vectors are what you will actually use in most modern C++ programs.

> **Prerequisites:** You should be comfortable with basic data types (`int`, `double`, `char`), variables, `if` statements, and loops. Loops and collections pair naturally, so if you are shaky on loops, review that chapter first.

> **A note on namespaces:** The examples in this tutorial use fully qualified names such as `std::cout` and `std::vector` to make it clear where each name comes from. In your own programs, you can write `using namespace std;` at the top of the file to omit the `std::` prefix. In larger programs — or when using multiple namespaces — the explicit `std::` form is often preferred because it avoids name conflicts.

---

## 2. Arrays

### 2.1 What Is an Array?

An **array** is a collection of elements that:

- Are all of the **same type** (e.g., all `int`, all `double`, all `char`)
- Are stored **contiguously** in memory (one after another, as a single block)
- Are accessed by a **single name** plus an **index** (position number)

Think of an array as a row of numbered lockers. The entire row has one name (for example, `test_scores`), and each locker is identified by its position number (0, 1, 2, …). All lockers must be the same size and hold the same kind of item.

```cpp
int test_scores[5];  // An array of 5 integers, all sharing the name "test_scores"
```

### 2.2 Why Do We Need Arrays?

Suppose you are a school administrator who needs to model the test scores of 100 students so you can compute statistics. With individual variables, you would write:

```cpp
int student1_score = 87;
int student2_score = 92;
// ... 98 more lines ...
int student100_score = 78;
```

This becomes unmanageable quickly. If you then need 10,000 scores, the approach collapses entirely.

An array lets you tell the compiler: *“Give me a collection of as many elements as I want, and give the entire collection a single name.”* You no longer have to name each individual element — you access them by position.

```cpp
int test_scores[100];        // 100 integers, one name
int many_scores[10000];      // 10,000 integers, still one name
```

Because the elements are numbered, you can use a loop to process all of them with a single block of code. This is the real power of arrays: they make it possible to write code that works on an entire collection without repeating yourself.

### 2.3 Characteristics of Arrays

Understanding these properties is essential to using arrays correctly.

| Characteristic | Meaning |
|---|---|
| **Fixed size** | Once you create an array of a specific size, the size cannot change. Arrays do not grow or shrink while the program runs. |
| **Homogeneous** | All elements must be of the same type. You can have arrays of `int`, arrays of `double`, arrays of `char`, etc. — but not a mix. |
| **Contiguous memory** | Elements are stored back-to-back in a single chunk of memory. |
| **Individually accessible** | Any element can be accessed directly using its index (position). |
| **Zero-based indexing** | The first element is at index `0`, not `1`. The last element is at index `size - 1`. |
| **No bounds checking** | C++ does not verify that your index is within the valid range. Accessing out of bounds causes **undefined behavior**. |
| **Uninitialized by default** | Local arrays contain garbage values until you explicitly initialize them. |

> **What is undefined behavior?** Undefined behavior (UB) means the C++ standard does not specify what your program will do. The result may vary from run to run, may crash, or may appear to work correctly — until it silently corrupts data. Well-written code must avoid any situation that causes undefined behavior.

> **Important:** Because the size of an array is fixed, adding or removing elements at runtime is impossible. If you create an array of 100 scores and later need room for more, you must change the size in the source code and recompile the program.

### 2.4 Declaring Arrays

The syntax for declaring an array is:

```
type name[size];
```

- **type** — the type of each element
- **name** — the name of the array variable
- **size** — the number of elements, written in square brackets `[]` (the array indexing operator)

The size must be known at **compile time** — the moment the compiler translates your source code into a program. The compiler needs this number to reserve the correct amount of memory before the program ever runs. The size can be:

- A **constant literal**: `5`, `365`
- A **declared constant**: `const int DAYS = 365;`
- A **constant expression**: `const int YEARS = 5; const int DAYS = YEARS * 365;`

Examples:

```cpp
int test_scores[5];              // 5 integers
int high_scores_per_level[10];   // 10 integers
double high_temperatures[365];   // 365 doubles

const int DAYS = 365;
double temperatures[DAYS];       // size determined by a constant
```

> **Best practice:** Always initialize your arrays when you declare them. An uninitialized **local automatic** array contains unknown (“garbage”) values left over from previous memory use. (Global and static arrays are zero-initialized, but relying on that is still poor style.)

### 2.5 Initializing Arrays

You can initialize an array at the point of declaration using an **initializer list** — a comma-separated list of values inside curly braces `{}`.

#### Full initialization

```cpp
int test_scores[5] {100, 95, 99, 97, 88};
```

Element `0` becomes `100`, element `1` becomes `95`, and so on. This is the safest and most predictable form.

#### Partial initialization

```cpp
int high_scores_per_level[10] {3, 5};
```

The first two elements become `3` and `5`. **The remaining eight elements are automatically initialized to `0`.** This is a useful and reliable behavior: as soon as you provide at least one initializer, the compiler guarantees that every element gets a defined value.

#### Zero initialization

```cpp
double high_temperatures[365] {0};   // all 365 elements set to 0.0
```

Providing a single `0` (or an empty `{}`) initializes **all** elements to zero. This is extremely common and highly recommended whenever you do not have specific starting values.

#### Compiler-determined size

```cpp
int another_array[] {1, 2, 3, 4, 5};   // compiler infers size = 5
```

If you omit the size, the compiler counts the initializers and sizes the array accordingly. This is useful when the number of elements is obvious from the data, and it avoids the risk of an accidental mismatch between the declared size and the number of values.

#### A common beginner pitfall

```cpp
int scores[5] {100};   // NOT all 100s!
```

This sets `scores[0]` to `100` and the remaining four elements to `0`. To set every element to `100`, you must either list all five values or use a loop (covered later).

> **Key rule:** Always initialize your arrays. Never assume they contain any particular value until you put it there yourself.

### 2.6 Accessing Array Elements

You access an individual element by writing the array name followed by the element’s index in square brackets. This is called **array subscripting**:

```cpp
array_name[index]
```

Given:

```cpp
int test_scores[5] {100, 95, 99, 97, 88};
```

| Expression | Value |
|---|---|
| `test_scores[0]` | `100` |
| `test_scores[1]` | `95` |
| `test_scores[4]` | `88` |

You can read from and write to elements using the same syntax:

```cpp
std::cout << test_scores[0];   // read element 0 → prints 100
test_scores[0] = 90;           // write 90 into element 0
```

Reading values from the console into an array:

```cpp
for (int i = 0; i < 5; ++i) {
    std::cin >> test_scores[i];
}
```

> **Remember:** Indexing starts at `0`. For an array of size `5`, the valid indices are `0, 1, 2, 3, 4`. There is no index `5`. Accessing `test_scores[5]` is **out of bounds** and causes undefined behavior.

### 2.7 How Arrays Work in Memory

To use arrays safely and efficiently, it helps to understand what happens behind the scenes.

**In most expressions, the array name decays to a pointer to its first element** (index `0`). (Exceptions include `sizeof(array)` and `&array`.) When you write `test_scores[2]`, the compiler performs a simple calculation:

```
address of element[i] = base address + (i × size of each element)
```

For example, if `test_scores` starts at address `1000` and each `int` occupies 4 bytes:

| Index | Address | Calculation |
|---|---|---|
| `[0]` | 1000 | base + 0×4 |
| `[1]` | 1004 | base + 1×4 |
| `[2]` | 1008 | base + 2×4 |
| `[3]` | 1012 | base + 3×4 |
| `[4]` | 1016 | base + 4×4 |

*(This simplified example uses small numbers; real memory addresses are much larger, such as `0x7ffee4a3c8a0`.)*

This is why arrays are so efficient: accessing any element is a single arithmetic operation, regardless of the array’s size. The computer does not need to search through the array — it computes the address directly.

**There is no bounds checking.** If you ask for `test_scores[10]` on a 5-element array, the compiler will happily compute the address (base + 10×4) and read or write whatever happens to be there. That memory may belong to another variable, or it may be inaccessible — either way, the result is **undefined behavior**. Your program may produce strange results, silently corrupt data, or crash.

> **It is the programmer’s responsibility** to ensure that every index is between `0` and `size - 1`.

When you print the array name itself:

```cpp
std::cout << test_scores;   // prints a hexadecimal address, e.g. 0x7ffd...
```

For a non-character array, you get the memory address of the first element — not the contents of the array. (A `char` array is printed as a C-style string only if it is null-terminated; otherwise, printing it causes undefined behavior.) This is a direct consequence of the memory model described above: the array name decays to the starting address.

### 2.8 Multi-dimensional Arrays

Arrays can have more than one dimension. A **two-dimensional array** is useful for modeling grids, tables, or spreadsheets. The syntax adds another set of square brackets:

```cpp
type name[rows][columns];
```

Example:

```cpp
int movie_ratings[3][4];   // 3 rows × 4 columns = 12 integers total
```

This can model data such as three reviewers each rating four movies:

|  | Movie 0 | Movie 1 | Movie 2 | Movie 3 |
|---|---|---|---|---|
| **Reviewer 0** | 1 | 2 | 3 | 4 |
| **Reviewer 1** | 2 | 3 | 4 | 5 |
| **Reviewer 2** | 3 | 4 | 5 | 6 |

#### Accessing elements

You provide both indices — first the row, then the column:

```cpp
movie_ratings[1][2] = 5;              // reviewer 1, movie 2
std::cout << movie_ratings[1][2];     // prints 5
```

#### Initializing a 2D array

Use nested initializer lists, one set of braces per row:

```cpp
int movie_ratings[3][4] {
    {1, 2, 3, 4},
    {2, 3, 4, 5},
    {3, 4, 5, 6}
};
```

Writing the data across multiple lines is much more readable than a single line, and it makes the row-and-column structure visually obvious.

#### Higher dimensions

The same pattern extends to three or more dimensions:

```cpp
int cube[3][4][5];   // 3 × 4 × 5 = 60 integers
```

> **Note:** Most compilers impose practical limits on the number of dimensions, though these limits are very high and rarely reached.

All the characteristics of one-dimensional arrays (fixed size, homogeneous type, contiguous storage, zero-based indexing, no bounds checking, garbage values until initialized) apply equally to multi-dimensional arrays.

---

## 3. Vectors

### 3.1 The Problem with Fixed-Size Arrays

Suppose you want to store test scores for students in a class, but registration hasn’t started — you don’t know how many students there will be. Students may also drop or add as the semester progresses.

Arrays are **fixed in size**. You must specify the size at compile time, and it cannot change at runtime. Your options with arrays are unsatisfactory:

1. **Over-allocate:** Set the size large enough (for example, 50 for a class of 30). This wastes memory, and you still must track how many elements are actually in use. Eventually, a student will exceed even the most generous size.
2. **Recompile:** Change the size and rebuild the program whenever enrollment changes — not acceptable for a running program.

We need a collection that can **grow and shrink at runtime**. This is exactly what a vector provides.

### 3.2 What Is a Vector?

A **vector** is a dynamic array provided by the C++ Standard Library (it is commonly described as an STL-style container). It is a **container** — an object that holds a collection of elements and provides methods for working with them.

Key properties of vectors:

| Property | Meaning |
|---|---|
| **Dynamic size** | Grows and shrinks automatically at runtime as you add or remove elements. |
| **Contiguous memory** | Like arrays, elements are stored back-to-back in memory. |
| **Individually accessible** | Elements are accessed by index, starting at `0`. |
| **Value initialization with size constructor** | When a vector is constructed with a size, its elements are value-initialized; for fundamental types such as `int`, this means zero. A default-constructed vector is empty, and `push_back(x)` inserts `x`. |
| **Rich functionality** | Supports member functions such as `push_back`, `at`, `size`, and `erase`, and works with standard algorithms such as `std::sort`, `std::reverse`, and `std::find`. |
| **Bounds-checked access available** | The `at()` method throws an exception if you go out of bounds; the `[]` operator does not check. |
| **Object-oriented** | A vector is an object; you can ask it to perform operations (call methods) on your behalf. |

> **Why vectors matter:** In modern C++, we rarely use raw arrays. Vectors are safer, more flexible, and more powerful. However, arrays appear extensively in legacy code, so understanding them remains important.

> **Modern alternative:** C++ also provides `std::array`, a fixed-size container with bounds-checked access via `at()` (but `operator[]` is still unchecked) and other conveniences. It is a good replacement for raw arrays in new code when the size is known at compile time. Vectors remain the right choice when the size must change at runtime.

### 3.3 Declaring and Initializing Vectors

Because vectors are part of the standard library, the syntax differs slightly from arrays.

**Step 1:** Include the vector header.

```cpp
#include <vector>
```

**Step 2:** Use the `std` namespace (or qualify every use with `std::`).

```cpp
using namespace std;
```

**Step 3:** Declare the vector. The element type goes inside **angle brackets** `<>`:

```cpp
vector<type> name;
```

#### Common declaration styles

**Empty vector:**

```cpp
vector<char> vowels;        // empty vector of characters (size 0)
vector<int> test_scores;    // empty vector of integers
```

**Vector with a specified size (elements initialized to zero):**

```cpp
vector<char> vowels(5);          // 5 characters, each initialized to '\0'
vector<int> test_scores(10);     // 10 integers, each initialized to 0
```

> Unlike local arrays, elements created by the size constructor are **value-initialized**. For fundamental types such as `int` and `double`, this means zero, so you do not get garbage values. This removes an entire category of bugs that beginners often encounter with arrays.

**Vector with a size and an initial value:**

```cpp
vector<double> temperatures(365, 80.0);   // 365 doubles, all set to 80.0
```

The first argument is the size; the second is the value to fill every element with.

**Vector with an initializer list:**

```cpp
vector<char> vowels {'a', 'e', 'i', 'o', 'u'};
vector<int> test_scores {100, 98, 89, 85, 93};
```

> **Note:** Character literals use **single quotes** (`'a'`), not double quotes (`"a"`). Double quotes create string literals, which are a different type.

**Copying from another vector:**

```cpp
vector<int> copy_of_scores(test_scores);   // copy of another vector
```

#### Summary of declaration forms

| Form | Example | Result |
|---|---|---|
| Empty | `vector<int> v;` | Size 0, no elements |
| Size only | `vector<int> v(10);` | 10 elements, all `0` |
| Size + value | `vector<double> v(365, 80.0);` | 365 elements, all `80.0` |
| Initializer list | `vector<int> v {1, 2, 3};` | 3 elements: `1, 2, 3` |
| Copy | `vector<int> v2(v1);` | Copy of `v1` |

### 3.4 Accessing Vector Elements

There are two primary ways to access vector elements.

#### Method 1: Array-style subscripting

```cpp
vector<int> test_scores {100, 95, 99, 87, 88};

std::cout << test_scores[0];   // 100
std::cout << test_scores[4];   // 88

test_scores[0] = 90;           // modify element 0
```

The syntax is identical to arrays. Indexing starts at `0`.

> **Caution:** The `[]` operator performs **no bounds checking**. Accessing an out-of-range index causes undefined behavior, just as it does with arrays.

#### Method 2: The `at()` method

Because a vector is an object, you can call **methods** on it. The syntax uses the dot operator `.`:

```cpp
object_name.method_name(arguments)
```

The `at()` method returns the element at a given index:

```cpp
std::cout << test_scores.at(0);   // 100
std::cout << test_scores.at(4);   // 88

test_scores.at(0) = 90;           // modify element 0
```

The name `at` makes the intent clear and readable. More importantly, **`at()` performs bounds checking**. If the index is out of range, it throws an exception (see [Section 3.6](#36-bounds-checking-and-exceptions)).

#### Useful vector methods

| Method | Purpose | Example |
|---|---|---|
| `at(i)` | Access element at index `i` (bounds-checked) | `v.at(2)` |
| `size()` | Return the number of elements | `v.size()` |
| `push_back(x)` | Add element `x` to the end | `v.push_back(80)` |
| `pop_back()` | Remove the last element | `v.pop_back()` |
| `clear()` | Remove all elements | `v.clear()` |
| `empty()` | Return `true` if the vector has no elements | `v.empty()` |

Example using `size()`:

```cpp
vector<int> test_scores {100, 98, 89};
std::cout << "There are " << test_scores.size() << " scores.";   // prints 3
```

Because vectors can grow and shrink, asking a vector for its current size is a common and important operation. With an array, the size is fixed and known from the declaration, but a vector’s size changes over time, so you must ask it rather than assume.

> **Note:** `erase` is a vector member function and needs no extra header. Algorithms such as `std::sort`, `std::reverse`, and `std::find` are **not** vector methods; they come from `<algorithm>`. We mention them here so you know they exist; you will meet them in later chapters.

### 3.5 Growing a Vector Dynamically

The `push_back()` method adds a new element to the **end** of the vector. The element you add must be of the same type as the vector’s elements.

```cpp
vector<int> test_scores {100, 95, 99};   // size 3

test_scores.push_back(80);   // now: 100, 95, 99, 80     (size 4)
test_scores.push_back(90);   // now: 100, 95, 99, 80, 90 (size 5)
```

If the vector needs more memory to hold the new element, it automatically allocates additional space, copies the existing elements if necessary, and updates its internal state. **You do not need to manage memory yourself.** This is one of the major advantages of vectors over arrays: you focus on your data, and the vector handles the details of memory management.

A typical pattern for reading an unknown number of values:

```cpp
vector<int> scores;
int score_to_add {0};

// Read values until the user signals end of input (Ctrl+Z / Ctrl+D)
while (std::cin >> score_to_add) {
    scores.push_back(score_to_add);   // vector grows as needed
}
```

> **Key insight:** With an array, adding a fourth element to a 3-element array is impossible without recompiling. With a vector, `push_back` handles it automatically.

### 3.6 Bounds Checking and Exceptions

Recall that arrays perform no bounds checking. Vectors give you a choice:

- `v[i]` — **no** bounds checking (fast, but dangerous)
- `v.at(i)` — **bounds-checked** (safer)

If you use `at()` with an invalid index, C++ **throws an exception** — an indication that an exceptional condition has occurred and normal execution cannot continue. If the exception is not caught, an error message is displayed and the program terminates.

Example:

```cpp
vector<int> test_scores {100, 95, 99, 80, 90};   // size 5

std::cout << test_scores.at(10);   // index 10 is out of range → exception
```

The error message will resemble:

```
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check: __n (which is 10) >= this->size() (which is 5)
```

Read this message in two parts:

- `terminate called after throwing an instance of 'std::out_of_range'` — the program stopped because an exception of type `std::out_of_range` was thrown and never caught.
- `what(): vector::_M_range_check: __n (which is 10) >= this->size() (which is 5)` — the specific detail: you requested index `10`, but the vector’s size is `5`.

Different compilers may format the message differently, but the information is the same.

**Why is crashing during testing good?** It surfaces bugs immediately, while you can still fix them. What you do **not** want is a crash in production. Later in your C++ journey, you will learn **exception handling** — catching these exceptions at runtime and responding gracefully so the program does not crash. That is a more advanced topic, but the key point for now is:

> Use `at()` when you want safety; use `[]` when you have verified the index yourself and want maximum performance.

### 3.7 Two-dimensional Vectors

Just as you can have multi-dimensional arrays, you can have multi-dimensional vectors. A two-dimensional vector is simply a **vector of vectors**:

```cpp
vector<vector<int>> movie_ratings {
    {1, 2, 3, 4},
    {2, 3, 4, 5},
    {3, 4, 5, 6}
};
```

- The **outer** vector holds three elements, each of which is a `vector<int>` (one per reviewer).
- Each **inner** vector holds four integers (one per movie).

> **Note:** Modern C++ (C++11 and later) accepts `vector<vector<int>>` directly. In older compilers, `>>` could be misread as the right-shift operator, so a space was required: `vector<vector<int> >`. Adding the space is still harmless, but it is no longer necessary.

#### Accessing elements with array syntax

```cpp
std::cout << movie_ratings[0][0];   // 1  (reviewer 0, movie 0)
std::cout << movie_ratings[0][1];   // 2  (reviewer 0, movie 1)
```

#### Accessing elements with vector syntax

Apply `at()` twice — once for the outer vector, once for the inner:

```cpp
std::cout << movie_ratings.at(0).at(0);   // 1
std::cout << movie_ratings.at(0).at(1);   // 2
```

Read it left to right: *“Give me the element at index 0 of `movie_ratings` (which is a vector), then give me the element at index 0 of that vector.”*

---

## 4. Arrays vs. Vectors: A Comparison

| Feature | Array | Vector |
|---|---|---|
| **Size** | Fixed at compile time | Dynamic — grows and shrinks at runtime |
| **Header required** | None | `#include <vector>` |
| **Namespace** | None | `std` |
| **Initialization** | Local automatic arrays are uninitialized unless initialized; global/static arrays are zero-initialized. | A default-constructed vector is empty. The size constructor value-initializes elements (zero for `int`); `push_back(x)` inserts `x`. |
| **Bounds checking** | Never | Available via `at()` |
| **Adding elements** | Impossible after declaration | `push_back()` |
| **Memory management** | Manual (you choose the size) | Automatic |
| **Performance** | Extremely efficient (bare-bones) | Very efficient, with a small overhead |
| **Use in modern C++** | Rare (mostly legacy code) | Standard practice |

**When to use arrays:**

- You are working with legacy C++ code that uses them.
- You need the absolute maximum performance and know the size is fixed.
- You are learning fundamental concepts that underlie all containers.

**When to use vectors:**

- Almost always, in modern C++.
- The number of elements is unknown at compile time or may change.
- You want safety features such as bounds checking.
- You want access to powerful algorithms (`sort`, `find`, etc.).

**A simple decision guide:**

- If the size is known at compile time and will never change → consider `std::array` (or a raw array if you must).
- If the size may change at runtime, or you are unsure → use `std::vector`.
- If you are reading or maintaining older C++ code → expect raw arrays and know how they work.

---

## 5. Summary

In this tutorial, you learned:

1. **Compound data types** are built from other types. Arrays and vectors are the two fundamental collection types in C++.

2. **Arrays** are fixed-size, homogeneous, contiguous collections with zero-based indexing and no bounds checking.
   - Declare with `type name[size];`
   - Initialize with `{value1, value2, ...}` — partial initialization fills the rest with zeros.
   - Access with `name[index]`.
   - Understand the memory model: the array name is the address of the first element, and indexing is simple pointer arithmetic.
   - Multi-dimensional arrays use additional square brackets: `name[rows][cols]`.

3. **Vectors** are dynamic, object-oriented containers from the Standard Template Library.
   - Include `<vector>` and use the `std` namespace.
   - Declare with `vector<type> name;`
   - Initialize with a size, a size and value, or an initializer list.
   - Access with `[]` (no bounds checking) or `.at()` (bounds-checked).
   - Grow with `.push_back()`, query with `.size()`.
   - Out-of-range `at()` calls throw exceptions.
   - Two-dimensional vectors are vectors of vectors: `vector<vector<int>>`.

4. **Always initialize your collections.** Uninitialized arrays contain garbage; vectors of a specified size are zero-initialized automatically.

5. **In modern C++, prefer vectors over raw arrays.** They are safer, more flexible, and more powerful. But understand arrays, because they appear throughout legacy code and illuminate how vectors work under the hood.

6. **Looping and iteration** pair naturally with both arrays and vectors — you will frequently loop through a collection to process every element.

---

## Quick Reference Card

| Task | Array | Vector |
|---|---|---|
| Declare (empty) | — | `vector<int> v;` |
| Declare (fixed size) | `int a[5];` | `vector<int> v(5);` |
| Declare + initialize | `int a[3] {1, 2, 3};` | `vector<int> v {1, 2, 3};` |
| Access element | `a[i]` | `v[i]` or `v.at(i)` |
| Get size | `sizeof(a) / sizeof(a[0])` (only for a true array in the same scope, not a pointer) | `v.size()` (simple) |
| Add element | Not possible | `v.push_back(x);` |
| Remove last element | Not possible | `v.pop_back();` |
| Bounds-checked access | Not available | `v.at(i)` |
| 2D version | `int a[3][4];` | `vector<vector<int>> v;` |

With arrays and vectors in your toolkit, you can now model collections of data efficiently and write programs that handle many values at once — a major step beyond individual variables.
