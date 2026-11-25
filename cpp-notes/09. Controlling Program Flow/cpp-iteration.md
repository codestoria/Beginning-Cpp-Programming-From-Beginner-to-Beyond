# C++ Control Flow: Iteration Structures

## Table of Contents

1. [Introduction: The Power of Repetition](#1-introduction-the-power-of-repetition)
   - [Anatomy of a Loop](#anatomy-of-a-loop)
   - [How to Run the Code Examples](#how-to-run-the-code-examples)
2. [The `for` Loop (Counter-Controlled Iteration)](#2-the-for-loop-counter-controlled-iteration)
   - [Step-by-Step Execution Trace](#step-by-step-execution-trace)
   - [Loop Variable Scope and Common Patterns](#loop-variable-scope-and-common-patterns)
   - [Tracking Multiple Variables in `for` Headers](#tracking-multiple-variables-in-for-headers)
3. [The Range-Based `for` Loop (C++11)](#3-the-range-based-for-loop-c11)
   - [Values vs. References (`auto`, `auto&`, and `const auto&`)](#values-vs-references-auto-auto-and-const-auto)
4. [The `while` Loop (Pre-Test Conditional Iteration)](#4-the-while-loop-pre-test-conditional-iteration)
   - [Input Validation and Boolean Flags](#input-validation-and-boolean-flags)
5. [The `do-while` Loop (Post-Test Conditional Iteration)](#5-the-do-while-loop-post-test-conditional-iteration)
   - [`while` vs. `do-while`: How to Choose](#while-vs-do-while-how-to-choose)
6. [Fine-Tuning Loops with `continue` and `break`](#6-fine-tuning-loops-with-continue-and-break)
7. [Infinite Loops: Accidental Bugs vs. Intentional Design](#7-infinite-loops-accidental-bugs-vs-intentional-design)
8. [Nested Loops](#8-nested-loops)
   - [Working with Two-Dimensional Data](#working-with-two-dimensional-data)
9. [Summary and Best Practices](#9-summary-and-best-practices)

---

## 1. Introduction: The Power of Repetition

In programming, **sequence** executes statements one after another in order, while **selection** (such as `if` and `switch` statements) chooses which execution path to take based on a condition.
While those two mechanisms allow a program to make decisions, they are not enough on their own: if you needed to process 10,000 customer records or render 60 frames per second in a video game, writing out the same code thousands of times by hand would be impractical and impossible to maintain.

**Iteration** — commonly referred to as **looping** — solves this problem by allowing a block of code to execute repeatedly until a specific condition is met. Together with sequence and selection, iteration forms one of the three fundamental control structures of structured programming.

### Anatomy of a Loop

Although C++ provides different loop syntaxes for different situations, every well-behaved loop relies on four logical components:

1. **Initialization:** Establishing the starting state before the loop begins (for example, setting a counter variable to `0`).
2. **Condition (Termination Test):** Evaluating a boolean expression (`true` or `false`) on each cycle to decide whether the loop should continue or stop.
3. **Body:** Executing the statement(s) that perform the repeated work.
4. **Update (Progress):** Modifying the loop's state during each cycle so that the condition eventually evaluates to `false` and the loop terminates.

C++ offers four loop constructs, each tailored to a specific programming scenario:

| Loop Construct | Best Used When... |
| :--- | :--- |
| **`for`** | You know in advance how many iterations are needed, or you need a numeric index. |
| **Range-based `for`** | You want to visit every element in a collection (such as an array, `std::vector`, or `std::string`). |
| **`while`** | You want to repeat as long as a condition holds, and the loop might need to run **zero** times. |
| **`do-while`** | You want to repeat as long as a condition holds, and the loop must run **at least once**. |

### How to Run the Code Examples

To keep this tutorial concise and focused on control flow, the code examples are presented as self-contained **code fragments** rather than complete programs.
Because range-based `for` loops and braced initializer lists require C++11 or later, compile with a modern standard flag (such as `-std=c++17` or `-std=c++20`) and paste any snippet inside `main()` using the following template:

```cpp
#include <initializer_list>  // Required for braced lists like {10, 20, 30}
#include <iostream>          // Required for std::cout and std::cin
#include <string>            // Required for std::string
#include <vector>            // Required for std::vector

int main() {
    // Paste any code fragment from this tutorial here
    return 0;
}
```

Throughout this guide, Standard Library names are explicitly prefixed with `std::` (for example, `std::cout` instead of `cout`).
This avoids naming collisions and is considered best practice in modern C++.

---

## 2. The `for` Loop (Counter-Controlled Iteration)

### Motivation

When you know *exactly* how many times an action must repeat — such as printing the numbers 1 through 10 or inspecting the first 50 elements of an array — spreading your counter setup, exit check, and increment logic across separate lines makes the code harder to read and easier to break.
The `for` loop solves this by consolidating all loop-control logic into a single, compact header.

### Syntax and Mechanics

```cpp
for (initialization; condition; update) {
    // Loop body
}
```

All three clauses inside the parentheses are optional, though the two semicolons (`;`) are always required. When all three clauses are present, execution follows a strict five-step cycle:

1. **`initialization`** executes **once** at the very beginning before the loop starts.
2. **`condition`** is evaluated before each iteration. If it evaluates to `false`, the loop terminates immediately and execution continues after the closing brace (`}`).
3. If **`condition`** evaluates to `true`, the **loop body** executes.
4. **`update`** executes immediately after the loop body finishes.
5. Control jumps back to **Step 2** to re-evaluate `condition`.

### Step-by-Step Execution Trace

Consider a `for` loop that prints the integers from `1` through `5`:

```cpp
for (int i = 1; i <= 5; i++) {
    std::cout << i << " ";
}
// Output: 1 2 3 4 5
```

Tracing the state of the control variable step by step shows *why* the loop stops when it does:

| Cycle | Phase | Value of `i` | `i <= 5`? | Action Taken |
| :--- | :--- | :--- | :--- | :--- |
| **Start** | Initialization | `1` | — | `i` is created and initialized to `1`. |
| **1** | Condition & Body | `1` | `true` | Prints `1 `. |
| **1** | Update | `2` | — | `i++` increments `i` from `1` to `2`. |
| **2** | Condition & Body | `2` | `true` | Prints `2 `. |
| **2** | Update | `3` | — | `i++` increments `i` from `2` to `3`. |
| **3** | Condition & Body | `3` | `true` | Prints `3 `. |
| **3** | Update | `4` | — | `i++` increments `i` from `3` to `4`. |
| **4** | Condition & Body | `4` | `true` | Prints `4 `. |
| **4** | Update | `5` | — | `i++` increments `i` from `4` to `5`. |
| **5** | Condition & Body | `5` | `true` | Prints `5 `. |
| **5** | Update | `6` | — | `i++` increments `i` from `5` to `6`. |
| **End** | Condition | `6` | `false` | **Loop terminates** without running the body. |

Notice that `i` reaches `6` before the loop terminates, while the body executes five times (for values `1` through `5`).

### Loop Variable Scope and Common Patterns

When you declare a variable inside the `for` header (`int i = 0`), C++ restricts its **scope** (visibility) strictly to that loop:

```cpp
for (int i = 0; i < 3; i++) {
    std::cout << i << "\n";
}
// std::cout << i;  // COMPILER ERROR: 'i' is out of scope here
```

This rule protects you from bugs: once the loop finishes, `i` is destroyed, allowing you to safely declare a fresh `int i` in a later loop without accidentally reusing a leftover value.

Because `initialization`, `condition`, and `update` can be any valid C++ expressions, `for` loops easily adapt to different counting directions and step sizes:

```cpp
// Counting backward
for (int i = 5; i > 0; i--) {
    std::cout << i << " ";
}
std::cout << "Liftoff!\n";  // Output: 5 4 3 2 1 Liftoff!

// Stepping by increments other than 1
for (int i = 0; i <= 100; i += 25) {
    std::cout << i << " ";  // Output: 0 25 50 75 100
}
```

### Tracking Multiple Variables in `for` Headers

Occasionally, an algorithm needs to move two counters in tandem (for instance, working inward from both ends of a sequence).
You can declare multiple variables of the same type using a comma separator in the initialization clause, and update both using C++'s **comma operator** (`,`):

```cpp
for (int left = 1, right = 5; left <= 5; left++, right--) {
    std::cout << left << " + " << right << " = " << (left + right) << "\n";
}
```

At the end of each iteration, `left++, right--` evaluates `left++` first and `right--` second.
*(Note: If you need to track two variables of different types, declare both variables before the `for` statement and assign their starting values inside the initialization clause using the comma operator: `for (i = 0, factor = 0.5; ...)`.)*

> **Avoiding Off-by-One Errors:** Because C++ arrays and containers are zero-indexed, a collection of size `N` has valid indices from `0` through `N - 1`. Always use the standard idiomatic pattern `for (int i = 0; i < N; i++)` (or `std::size_t` when matching unsigned container sizes such as `vec.size()`). Mixing `< N` and `<= N - 1` across your code is one of the most common sources of off-by-one bugs.

---

## 3. The Range-Based `for` Loop (C++11)

### Motivation

One of the most frequent tasks in programming is visiting every item in a collection — such as a built-in array, a `std::vector` (C++'s resizable dynamic array), or a `std::string`.
Using a traditional counter-controlled `for` loop requires managing an index variable, querying the container's size, checking boundaries, and subscripting (`scores[i]`).
Every extra piece of boilerplate is an opportunity for a typo or an out-of-bounds memory access.

Introduced in the C++11 standard, the **range-based `for` loop** eliminates index management altogether: you specify the collection you want to traverse, and C++ automatically hands you each element in order.

### Syntax and Mechanics

```cpp
for (element_declaration : collection) {
    // Loop body using the element variable
}
```

Behind the scenes, the compiler automatically locates the beginning and end of `collection`, steps through every element sequentially, and binds each element to your declared loop variable.

### Examples

**1. Traversing a `std::vector`:**

```cpp
std::vector<double> temperatures = {72.5, 68.0, 75.2, 80.1};
double sum = 0.0;

for (auto temp : temperatures) {
    sum += temp;
}

if (!temperatures.empty()) {
    std::cout << "Average temperature: " << (sum / temperatures.size()) << "\n";
}
```

Here, the `auto` keyword instructs the compiler to automatically deduce the element type (`double`) from `temperatures`.
Using `auto` keeps your loops concise and ensures they continue compiling cleanly even if you later change the container's element type.
*(Checking `!temperatures.empty()` before dividing also prevents a division-by-zero error if the vector happens to have zero elements.)*

**2. Traversing a `std::string` character by character:**

```cpp
std::string message = "C++ is powerful";

for (auto ch : message) {
    if (ch != ' ') {
        std::cout << ch;
    }
}
// Output: C++ispowerful (spaces are skipped)
```

**3. Iterating over a braced initializer list:**

```cpp
for (auto num : {10, 20, 30, 40}) {
    std::cout << num << " ";
}
```

### Values vs. References (`auto`, `auto&`, and `const auto&`)

How you declare the loop variable determines whether C++ copies each element or accesses the original element directly in memory.
Choosing the right form matters for both correctness and performance:

- **`for (auto x : collection)` (By Value — Makes a Copy):**
  Each element is *copied* into `x`. Modifying `x` inside the loop only changes the local copy, leaving the original container untouched. This is ideal for small primitive types like `int`, `double`, or `char`, but inefficient for larger objects like `std::string` because copying allocates extra memory on every iteration.
- **`for (auto& x : collection)` (By Reference — Modifies Originals):**
  The ampersand (`&`) makes `x` a direct alias (reference) to the element inside the container. Use this when you intend to **modify** the elements in place:
  ```cpp
  std::vector<int> values = {1, 2, 3};
  for (auto& val : values) {
      val *= 10;  // Directly updates 'values' to {10, 20, 30}
  }
  ```
- **`for (const auto& x : collection)` (By Const Reference — Read-Only, No Copy):**
  `x` refers directly to the original element without copying it, while `const` prevents you from accidentally modifying it. **This is the recommended default when reading collections of objects (such as `std::string` or nested vectors).**

---

## 4. The `while` Loop (Pre-Test Conditional Iteration)

### Motivation

You do not always know ahead of time how many iterations a task will require.
For instance, you might need to read lines from a file until reaching the end, or keep prompting a user until they enter a valid number. Because there is no fixed iteration count, a counter-controlled `for` loop is a poor fit.
The `while` loop solves this by repeating *as long as* a condition remains `true`.

### Syntax and Mechanics

```cpp
while (condition) {
    // Loop body
}
```

The `while` loop is a **pre-test loop**:
1. `condition` is evaluated **before** each iteration.
2. If `condition` is `true`, the loop body executes, and control returns to Step 1.
3. If `condition` is `false`, the loop terminates immediately.

> **Key Takeaway:** Because the condition is checked *before* the body ever runs, if `condition` is initially `false`, the body of a `while` loop will execute **zero times**.

### Example: Counting with `while`

```cpp
int i = 1;                  // 1. Initialization
while (i <= 5) {            // 2. Condition
    std::cout << i << " ";  // 3. Body
    i++;                    // 4. Update (CRITICAL: omitting this causes an infinite loop!)
}
```

Unlike a `for` loop, where the update sits prominently in the header, a `while` loop requires you to update the control state manually inside the body.

### Input Validation and Boolean Flags

A classic use case for `while` is rejecting invalid user input until acceptable data is entered:

```cpp
int age = 0;
std::cout << "Enter your age (1-120): ";
std::cin >> age;

while (age < 1 || age > 120) {
    std::cout << "Invalid age. Please enter a number between 1 and 120: ";
    std::cin >> age;
}

std::cout << "Recorded age: " << age << "\n";
```

Notice a minor drawback in this pattern: the prompt and `std::cin >> age` appear **twice** — once before the loop to prime the condition, and once inside the loop to read subsequent attempts.

When validation rules grow more complex, developers often introduce a **boolean flag** (a `bool` variable tracking state) to clarify the loop's purpose and avoid duplicating input statements:

```cpp
bool is_valid = false;
int choice = 0;

while (!is_valid) {
    std::cout << "Select an option (1-3): ";
    std::cin >> choice;

    if (choice >= 1 && choice <= 3) {
        is_valid = true;
    } else {
        std::cout << "Out of range. Please try again.\n";
    }
}
```

---

## 5. The `do-while` Loop (Post-Test Conditional Iteration)

### Motivation

Consider an interactive command-line menu: you *always* want to display the options and capture the user's choice at least once before checking whether they selected "Quit."
Using a `while` loop forces you to either duplicate the prompt or assign a dummy starting value just to get inside the loop.
The `do-while` loop removes this awkwardness by guaranteeing that the body runs **at least once**.

### Syntax and Mechanics

```cpp
do {
    // Loop body
} while (condition);  // <-- Trailing semicolon is mandatory!
```

The `do-while` loop is a **post-test loop**:
1. The **loop body** executes unconditionally on the first pass.
2. **`condition`** is evaluated **after** the body completes.
3. If `condition` is `true`, control jumps back to `do` and repeats the body.
4. If `condition` is `false`, the loop terminates.

### Example: Clean Input Validation and Menu System

Compare how cleanly `do-while` handles the input validation task from Section 4:

```cpp
int age = 0;

do {
    std::cout << "Enter your age (1-120): ";
    std::cin >> age;
} while (age < 1 || age > 120);

std::cout << "Recorded age: " << age << "\n";
```

There is no duplicated `std::cin` statement and no extra boolean flag required. Here is the same pattern applied to an interactive menu:

```cpp
char selection = ' ';

do {
    std::cout << "\n=== MAIN MENU ===\n"
              << "1. View Profile\n"
              << "2. Edit Settings\n"
              << "Q. Quit\n"
              << "Selection: ";
    std::cin >> selection;

    switch (selection) {
        case '1':
            std::cout << "Displaying profile...\n";
            break;
        case '2':
            std::cout << "Opening settings...\n";
            break;
        case 'q':
        case 'Q':
            std::cout << "Exiting program. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid option. Please try again.\n";
            break;
    }
} while (selection != 'q' && selection != 'Q');
```

### `while` vs. `do-while`: How to Choose

| Feature | `while` Loop | `do-while` Loop |
| :--- | :--- | :--- |
| **Test Timing** | **Pre-test** (before body) | **Post-test** (after body) |
| **Minimum Executions** | **0** times | **1** time |
| **Best Scenario** | Data might already satisfy the exit condition (e.g., reading an empty file). | The condition depends on values produced *inside* the loop body (e.g., menus and prompts). |

---

## 6. Fine-Tuning Loops with `continue` and `break`

### Motivation

Normally, a loop evaluates its exit condition only at the boundary of each full cycle. Occasionally, however, an event mid-iteration requires altering that normal flow: you might need to skip invalid data and move immediately to the next item, or stop the loop early because you already found what you were searching for. C++ provides `continue` and `break` for these exact scenarios.

### The `continue` Statement (Skip Current Iteration)

When `continue` executes, C++ immediately skips any remaining statements in the current iteration and moves to the next cycle:
- In a **`for` loop**, control jumps directly to the **update** clause (e.g., `i++`), and then evaluates the condition.
- In a **`while` or `do-while` loop**, control jumps directly to the **condition** test. *(Warning: In a `while` loop, ensure your loop counter is updated before `continue` runs; otherwise, the variable never changes and you will trigger an infinite loop.)*

```cpp
// Print only odd numbers between 1 and 10
for (int i = 1; i <= 10; i++) {
    if (i % 2 == 0) {
        continue;  // Skip even numbers; jumps straight to i++
    }
    std::cout << i << " ";
}
// Output: 1 3 5 7 9
```

### The `break` Statement (Exit Loop Immediately)

When `break` executes, C++ immediately terminates the **innermost enclosing loop** and transfers execution to the first statement following the loop's closing brace (`}`).

```cpp
// Search for the first negative number
std::vector<int> readings = {14, 22, 19, -5, 30, -2};

for (auto val : readings) {
    if (val < 0) {
        std::cout << "Alert: First negative reading found: " << val << "\n";
        break;  // Stop searching immediately; no need to inspect 30 or -2
    }
}
```

### Combined Example: Filtering a Sensor Stream

Imagine processing a stream of sensor readings where `-1` represents temporary static (ignore it) and `-999` signals end-of-transmission (stop reading):

```cpp
std::vector<int> stream = {10, 20, -1, 30, -1, 40, -999, 50, 60};

for (auto signal : stream) {
    if (signal == -999) {
        break;  // End-of-transmission signal: terminate loop immediately
    }
    if (signal == -1) {
        continue;  // Noise: skip this value and check the next one
    }
    std::cout << signal << " ";
}
// Output: 10 20 30 40
```

> **Best Practice:** Use `break` and `continue` judiciously. A loop with one clear entry point and one clear exit condition is much easier to trace and debug than a loop scattered with multiple early jumps.

---

## 7. Infinite Loops: Accidental Bugs vs. Intentional Design

An **infinite loop** is a loop whose terminating condition never evaluates to `false`, causing it to execute endlessly unless interrupted externally or exited via a jump statement such as `break` or `return`.

### Accidental Infinite Loops (Bugs)

Most infinite loops are unintentional bugs caused by one of three common mistakes:

1. **Forgetting to update the control variable in a `while` loop:**
   ```cpp
   int count = 1;
   while (count <= 5) {
       std::cout << count << "\n";
       // BUG: Forgot count++! 'count' stays 1 forever.
   }
   ```
2. **Updating the control variable in the wrong direction:**
   ```cpp
   for (int i = 10; i > 0; i++) {  // BUG: i++ moves away from 0 instead of toward it!
       std::cout << i << " ";
   }
   ```
3. **Placing a stray semicolon after a `while` or `for` header:**
   ```cpp
   int i = 1;
   while (i <= 5); {  // BUG: The semicolon acts as an empty loop body!
       std::cout << i << "\n";
       i++;           // Never reached because the empty body loops forever
   }
   ```

### Intentional Infinite Loops

In systems and application programming, certain loops are *designed* to run continuously until an external event stops them:
- **Game loops:** Continuously process player input, update physics, and render frames 60 or more times per second until the player closes the application.
- **Web servers and embedded microcontrollers:** Continuously listen for incoming network requests or hardware sensor interrupts.

In C++, intentional infinite loops are idiomatically written using `while (true)` or `for (;;)`:

```cpp
while (true) {
    std::string command;
    std::cout << "> ";
    std::cin >> command;

    if (command == "exit") {
        break;  // Explicit exit point
    }
    std::cout << "Executing: " << command << "\n";
}
```

**Rule of Thumb:** Reserve `while (true)` for situations where the exit condition genuinely cannot be expressed cleanly at the top or bottom of the loop. Whenever a standard boolean condition works, write that condition explicitly in the loop header.

---

## 8. Nested Loops

### Motivation

Real-world data is frequently two-dimensional or hierarchical: spreadsheets organize data into rows and columns, digital images consist of `x` and `y` pixels, and weekly schedules span days and hours. Just as `if` statements can be placed inside other `if` statements, **a loop can be placed inside the body of another loop**. This structure is known as **nesting**.

### Mechanics: How Outer and Inner Loops Interact

```cpp
for (int outer = 1; outer <= 2; outer++) {        // Outer loop
    for (int inner = 1; inner <= 3; inner++) {    // Inner loop
        std::cout << "(" << outer << "," << inner << ") ";
    }
    std::cout << "\n";
}
```

**Output:**

```text
(1,1) (1,2) (1,3)
(2,1) (2,2) (2,3)
```

**The Golden Rule of Nested Loops:**

For **every single iteration** of the outer loop, the inner loop runs **completely from start to finish**.
If the outer loop runs $M$ times and the inner loop runs $N$ times, the inner body executes a total of $M \times N$ times.

### Working with Two-Dimensional Data

Nested loops are the standard tool for processing grids, matrices, and two-dimensional vectors.

**1. Classic Row-by-Column Grid Traversal:**

```cpp
const int ROWS = 3;
const int COLS = 4;
int grid[ROWS][COLS];

// Populate every cell
for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
        grid[row][col] = (row + 1) * 10 + col;
    }
}

// Display the grid
for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
        std::cout << grid[row][col] << "\t";
    }
    std::cout << "\n";  // Print a newline after completing each row
}
```

**2. Traversing a Jagged 2D `std::vector` with Range-Based `for`:**

A `std::vector<std::vector<int>>` is a vector whose elements are themselves vectors, meaning each row can have a different length (known as a *jagged* 2D structure).
Range-based `for` loops handle varying row lengths automatically:

```cpp
std::vector<std::vector<int>> matrix = {
    {1, 2, 3},
    {40, 50},
    {600, 700, 800, 900}
};

for (const auto& row : matrix) {    // Outer loop: each row (by const reference to avoid copying)
    for (auto val : row) {          // Inner loop: each integer inside that row
        std::cout << val << " ";
    }
    std::cout << "\n";
}
```

**3. Generating a Text-Based Bar Chart:**

Nested loops are also useful for visual pattern generation.
Below, an outer loop steps through each sales figure, while an inner loop prints a bar of corresponding length:

```cpp
std::vector<int> sales = {3, 7, 5};

for (auto count : sales) {
    for (int i = 1; i <= count; i++) {
        // Mark every 5th unit with '*' to make counting easier
        std::cout << (i % 5 == 0 ? '*' : '-');
    }
    std::cout << " (" << count << ")\n";
}
```

**Output:**

```text
--- (3)
----*-- (7)
----* (5)
```

---

## 9. Summary and Best Practices

### Choosing the Right Loop

| Construct | Choose When... | Key Characteristic |
| :--- | :--- | :--- |
| **`for`** | You know the exact iteration count or need a numeric index. | Bundles initialization, condition, and update in one header. |
| **Range-based `for`** | You need to visit every element of a container. | Eliminates index arithmetic and off-by-one bugs. |
| **`while`** | The iteration count is unknown and the body might run 0 times. | Pre-test loop; remember to update state inside the body. |
| **`do-while`** | The iteration count is unknown and the body must run $\ge 1$ time. | Post-test loop; ideal for menus and interactive prompts. |
| **`continue`** | You want to skip the rest of the current iteration. | Jumps to update (`for`) or condition (`while` / `do-while`). |
| **`break`** | You want to exit the innermost loop immediately. | Transfers control directly after the loop's closing `}`. |

### Universal Best Practices for Loops

1. **Prefer range-based `for` for containers.** Whenever you do not need the numeric index, `for (const auto& item : container)` is cleaner, safer, and less error-prone than index-based loops.
2. **Declare loop counters inside the `for` header.** Keeping `int i` scoped strictly to the loop prevents accidental reuse and stale-value bugs later in your function.
3. **Use descriptive names in nested loops.** In single-level loops, `i` is universally understood. In nested loops, descriptive names like `row` and `col` prevent you from accidentally swapping indices (such as writing `grid[col][row]`).
4. **Keep nesting shallow.** Two levels of loops are common when working with 2D data; three or more levels quickly become difficult to read and reason about. If you find yourself nesting three or four loops deep, consider extracting the inner logic into a dedicated helper function.
5. **Trace boundary cases by hand.** Before considering a loop finished, ask yourself three questions: *What happens if the input collection is empty? What happens on the very first iteration? What happens on the very last iteration?* Checking these edge cases catches the vast majority of loop bugs.
