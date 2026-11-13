# C++ Control Structures: Selection and Iteration

## Table of Contents

**Introduction**
- [Why Control Structures Matter](#introduction-why-control-structures-matter)
- [How to Read This Tutorial](#how-to-read-this-tutorial)
- [About the Code Examples](#about-the-code-examples)

**Part 1: Selection Structures**
- [1. The `if` Statement](#1-the-if-statement)
- [2. The `if-else` Statement](#2-the-if-else-statement)
- [3. Nested `if` Statements](#3-nested-if-statements)
- [4. The `switch-case` Statement](#4-the-switch-case-statement)
- [5. The Conditional Operator (`?:`)](#5-the-conditional-operator-)

**Part 2: Iteration Structures**
- [6. The `for` Loop](#6-the-for-loop)
- [7. The Range-Based `for` Loop (C++11)](#7-the-range-based-for-loop-c11)
- [8. The `while` Loop](#8-the-while-loop)
- [9. The `do-while` Loop](#9-the-do-while-loop)
- [10. `continue` and `break`](#10-continue-and-break)
- [11. Infinite Loops](#11-infinite-loops)
- [12. Nested Loops](#12-nested-loops)

**Conclusion**
- [Summary: Choosing the Right Tool](#summary-choosing-the-right-tool)
- [Universal Best Practices](#universal-best-practices)
- [The Big Picture](#the-big-picture)

---

## Introduction: Why Control Structures Matter

Every program you have written so far executes **sequentially** — one statement after another, from top to bottom. This is the *sequence* building block of programming. Sequence alone can solve many problems, but real programs must also make decisions and repeat actions.

This tutorial introduces the two remaining building blocks:

| Building Block | Purpose | C++ Constructs |
|---|---|---|
| **Sequence** | Execute statements one after another | Any list of statements |
| **Selection** | Choose which code to run based on conditions | `if`, `if-else`, `switch`, conditional operator |
| **Iteration** | Repeat code multiple times | `for`, `while`, `do-while`, range-based `for` |

The remarkable insight is this: **with only sequence, selection, and iteration, you can implement any algorithm**. These three building blocks are the foundation of all programming.

We will learn them one at a time. For each construct, we will ask three questions:

1. **Why does it exist?** (What problem does it solve?)
2. **How does it work?** (What is the syntax and semantics?)
3. **When should I use it?** (How do I choose between alternatives?)

### How to Read This Tutorial

- **Read the motivation first.** Each section begins with a problem that the construct solves. Understanding the problem makes the syntax meaningful.
- **Trace the examples by hand.** For loops, write down the value of each variable at every step. This builds intuition faster than reading alone.
- **Type the code and run it.** Modify the examples. Break them intentionally to see what happens. Experimentation is how programming becomes intuitive.
- **Use the summary table** at the end as a quick reference when you write your own programs.

### About the Code Examples

The examples in this tutorial are **code fragments**, not complete programs. They assume the following:

- `#include <iostream>` is present when `std::cout` or `std::cin` is used.
- `#include <vector>` is present when `std::vector` is used.
- `#include <string>` is present when `std::string` is used.
- `#include <initializer_list>` is present when a braced initializer list is used directly, for example in `for (auto val : {1, 2, 3})`.
- The `std::` prefix is used explicitly (e.g., `std::cout` rather than `cout`). This is a good habit: it makes clear where each name comes from.

You can paste any example into a `main` function, add the necessary `#include` directives, and compile it.

---

# Part 1: Selection Structures

Selection structures let your program *choose* a path. They evaluate a condition and, based on whether it is true or false, execute different code.

## 1. The `if` Statement

### Motivation

Sometimes you want code to run **only if** some condition is met. For example, only heal the player if their health is below 100. Without `if`, you would have no way to express this conditional behavior.

### Syntax

```cpp
if (control_expression) {
    // statements executed only when control_expression is true
}
```

The control expression is contextually converted to `bool`. For scalar types, any nonzero value is treated as `true`, and zero as `false`. If the result is `true`, the statement (or block of statements) is executed. If it is `false`, the statement is skipped entirely.

### Example

```cpp
int num = 20;

if (num > 10) {
    std::cout << num << " is greater than 10\n";
}
```

Here is what happens conceptually:

1. The expression `num > 10` is evaluated → `true` (since 20 > 10).
2. Because it is `true`, the output statement runs.
3. If `num` had been 5, the expression would be `false`, and the output would be skipped.

### Executing Multiple Statements: Block Statements

If the condition is true and you want to run *more than one* statement, you must wrap them in **curly braces** `{}`. This creates a **block statement**.

```cpp
if (num > 10) {
    num++;
    std::cout << "Incremented to " << num << "\n";
}
```

Without the braces, only the first statement after the `if` would be part of the conditional logic. The braces make the grouping explicit.

### Scope and Block Statements

Blocks also introduce **scope**. A variable declared inside a block is only visible within that block:

```cpp
if (num > 10) {
    int diff = num - 10;   // diff exists only here
    std::cout << "Difference: " << diff << "\n";
}
// diff is not accessible here — compiler error
```

This is a *feature*, not a limitation: it prevents variables from leaking into places where they should not be used, making your code easier to reason about and debug.

### Best Practice: Indentation

Always indent the body of your `if` statement:

```cpp
if (condition) {
    statement1;    // indented
    statement2;    // indented
}
```

The compiler does not care about indentation, but **humans do**. Good indentation makes the structure of your code visually obvious. As you nest constructs (we will see this soon), indentation becomes absolutely essential.

---

## 2. The `if-else` Statement

### Motivation

Often, when a condition is false, you want to do something *else* — not just skip ahead. For example, if the player's health is below 100, heal them; **otherwise**, increment their health normally. This is a binary decision, and `if-else` expresses it directly.

### Syntax

```cpp
if (control_expression) {
    // executes when true
} else {
    // executes when false
}
```

Exactly one of the two branches will execute — never both, never neither.

### Example

```cpp
int num = 0;
std::cout << "Enter a number: ";
std::cin >> num;

if (num >= 10) {
    std::cout << num << " is >= 10. Difference: " << (num - 10) << "\n";
} else {
    std::cout << num << " is < 10. Difference: " << (10 - num) << "\n";
}
```

### Conceptual Flow

1. Evaluate `num >= 10`.
2. If `true` → execute the `if` block, skip the `else` block.
3. If `false` → skip the `if` block, execute the `else` block.
4. Either way, continue with whatever follows.

---

## 3. Nested `if` Statements

### Motivation

Real decisions often have layers. "If my team is playing today, *then* if the game is on now, record it." This is a decision *inside* a decision. Since an `if` statement is itself a statement, you can place it inside another `if` — this is **nesting**.

### Example

```cpp
if (score > 90) {
    if (score > 95) {
        std::cout << "A+\n";
    } else {
        std::cout << "A\n";
    }
} else {
    std::cout << "Not an A\n";
}
```

Note that the inner condition (`score > 95`) is only checked when the outer condition (`score > 90`) is true. This is the essence of nesting: the inner decision depends on the outer one.

### The Dangling `else` Problem

When you have nested `if` statements, an `else` is ambiguous: which `if` does it belong to?

```cpp
if (expr1)
    if (expr2)
        statement1;
    else
        statement2;   // Does this else belong to the inner or outer if?
```

**C++ resolves this by attaching the `else` to the closest unmatched `if`** — in this case, the inner one. This is called the *dangling else problem*. To avoid confusion, **always use curly braces** when nesting, even for single statements:

```cpp
if (expr1) {
    if (expr2) {
        statement1;
    } else {
        statement2;   // Unambiguously belongs to the inner if
    }
}
```

### Using Blocks with Nested `if`

Since each branch can be a block, you can execute multiple statements at any level:

```cpp
if (num != 0) {
    if (num > 0) {
        std::cout << "Positive\n";
        num *= 2;
    } else {
        std::cout << "Negative\n";
        num = -num;
    }
}
```

---

## 4. The `switch-case` Statement

### Motivation

When you compare an integer or character against many specific constant values, a long chain of `if-else if` statements becomes tedious and hard to read. The `switch` statement is designed exactly for this scenario: **dispatch on the value of an integral expression**.

### Syntax

```cpp
switch (control_expression) {
    case value1:
        // statements
        break;
    case value2:
        // statements
        break;
    // ...
    default:
        // statements when no case matches
}
```

**Key requirements:**

- The control expression must be of an **integral type** (e.g., `int`, `char`, `bool`) or an **enumeration type**.
- Each `case` value must be a **compile-time constant expression** — for example, a literal, a `constexpr` variable, or a `const` variable initialized by a constant expression. Ordinary non-constant variables are not allowed.
- The `break` statement exits the `switch`. Without it, execution "falls through" to the next case (explained below).
- The `default` case (optional but recommended) handles all values that do not match any case.

### Example: Character Selection

```cpp
char selection = '2';

switch (selection) {
    case '1':
        std::cout << "One selected\n";
        break;
    case '2':
        std::cout << "Two selected\n";
        break;
    case '3':
    case '4':
        std::cout << "Three or four selected\n";
        break;
    default:
        std::cout << "Invalid selection\n";
}
```

Notice how `case '3':` and `case '4':` share the same code — this is a useful pattern equivalent to an OR condition.

### Fall-Through Behavior

If you omit `break`, execution continues into the next case's code — this is called **fall-through**. Consider:

```cpp
switch (selection) {
    case '1':
        std::cout << "One selected\n";
        // no break!
    case '2':
        std::cout << "Two selected\n";
        break;
    case '3':
        std::cout << "Three selected\n";
        break;
}
```

If `selection` is `'1'`, both "One selected" and "Two selected" are printed before the `break` at the end of case `'2'` stops execution. Once a match occurs, *all* code from that point forward runs until a `break` is reached — no further case conditions are checked.

### Best Practice

- **Always include `break` in each case** unless you have a specific, documented reason for fall-through.
- **Always provide a `default` case**, even if you think it is unreachable. It protects against unexpected values.
- If you rely on fall-through intentionally, **document it clearly** so future readers (including yourself) understand the intent.

### Switch with Enumerations

`switch` is especially elegant with `enum` types:

```cpp
enum Direction { LEFT, RIGHT, UP, DOWN };

Direction heading = LEFT;

switch (heading) {
    case LEFT:   std::cout << "Going left\n";   break;
    case RIGHT:  std::cout << "Going right\n";  break;
    case UP:     std::cout << "Going up\n";     break;
    case DOWN:   std::cout << "Going down\n";   break;
}
```

A compiler warning may appear if you do not handle every enumerator — this is useful feedback. Adding a `default` case also silences it, but be aware that it can then hide the warning when you later add a new enumerator and forget to handle it.

### When to Use `switch` vs `if-else`

| Use `switch` when... | Use `if-else` when... |
|---|---|
| Comparing against integer/char constants | Comparing ranges or non-integral types |
| Many discrete values | Complex boolean expressions |
| Clarity matters (a list is easier to read) | You need logical operators (`&&`, `||`) |

---

## 5. The Conditional Operator (`?:`)

### Motivation

Sometimes you need a simple **either-or** value inside a larger expression — for example, in an output statement or an assignment. Writing an `if-else` for such a small decision feels heavy. The conditional operator is a *ternary operator* (it takes three operands) that expresses an `if-else` inline.

### Syntax

```cpp
condition ? expression_if_true : expression_if_false
```

1. Evaluate `condition`; it is contextually converted to `bool`.
2. If `true`, the result is `expression_if_true`.
3. If `false`, the result is `expression_if_false`.

### Example: Finding the Maximum

```cpp
int a = 10, b = 20;
int result = (a > b) ? a : b;   // result becomes 20
```

This is equivalent to:

```cpp
int result;
if (a > b) {
    result = a;
} else {
    result = b;
}
```

But more concise. The operator shines when used inside an expression:

```cpp
std::cout << "The larger value is: " << ((a > b) ? a : b) << "\n";
```

### Example: Avoiding Division by Zero

```cpp
int result = (b != 0) ? (a / b) : 0;
```

This divides `a` by `b` only when `b` is non-zero — preventing a runtime crash. Note that the conditional operator is *not* a substitute for full `if-else` logic; it is for simple value selection.

### Best Practice

- **Do not nest conditional operators.** Nested `?:` quickly becomes unreadable. Use an `if` / `else if` chain instead.
- **Prefer using it where it clarifies** — e.g., in output statements, simple assignments, or selecting between two values.
- Keep it short. If the expressions are long or complex, use `if-else` for readability.

---

# Part 2: Iteration Structures

Loops let you **repeat** a statement or block of statements. Every loop has two parts:

1. **Loop condition** — determines whether to continue looping.
2. **Loop body** — the code that gets repeated.

C++ provides four loop constructs: `for`, range-based `for`, `while`, and `do-while`.

## 6. The `for` Loop

### Motivation

When you know *exactly* how many times you need to repeat something (e.g., "do this 10 times"), the `for` loop is the natural choice. It bundles all loop management — initialization, condition, and increment — into one compact line.

### Syntax

```cpp
for (initialization; condition; increment) {
    // loop body
}
```

All three expressions are **optional** (though semicolons are required). In its standard form:

1. **Initialization** runs *once*, before the first iteration.
2. **Condition** is checked before each iteration. If `false`, the loop terminates.
3. **Body** executes.
4. **Increment** runs after each iteration.
5. Return to step 2.

### Example: Counting to 5

```cpp
for (int i = 1; i <= 5; i++) {
    std::cout << i << " ";
}
// Output: 1 2 3 4 5
```

Step by step:

| Step | `i` value | `i <= 5`? | Action |
|---|---|---|---|
| Init | 1 | — | — |
| Check | 1 | true | Print 1 |
| Increment | 2 | — | — |
| Check | 2 | true | Print 2 |
| ... | ... | ... | ... |
| Check | 6 | false | Loop ends |

### Declaring the Loop Variable Inline

You can declare the loop variable directly in the initialization expression:

```cpp
for (int i = 0; i < 3; i++) {
    // i exists only inside the loop
}
// i is not accessible here
```

This is the **preferred style**: the loop variable's scope is confined to the loop, preventing accidental use elsewhere and avoiding stale values.

### Variations

**Counting down:**

```cpp
for (int i = 10; i > 0; i--) {
    std::cout << i << " ";
}
std::cout << "Blast off!\n";
```

**Skipping values:**

```cpp
for (int i = 0; i <= 100; i += 10) {
    std::cout << i << " ";   // 0 10 20 ... 100
}
```

**Using `double`:**

```cpp
for (double d = 0.0; d <= 1.0; d += 0.25) {
    std::cout << d << " ";   // 0 0.25 0.5 0.75 1
}
```

### The Comma Operator in `for` Loops

C++ has a comma operator that lets you separate multiple expressions. While rarely used elsewhere, it appears in `for` loops to initialize or increment multiple variables:

```cpp
for (int i = 1, j = 5; i <= 5; i++, j++) {
    std::cout << i << " + " << j << " = " << (i + j) << "\n";
}
```

In this example, the comma operator is used in the increment expression `i++, j++`. Note that the comma in `int i = 1, j = 5` is a declaration separator, not the comma operator. If you want to demonstrate the comma operator in the initialization expression as well, write:

```cpp
int i, j;
for (i = 1, j = 5; i <= 5; i++, j++) {
    std::cout << i << " + " << j << " = " << (i + j) << "\n";
}
```

### Best Practices for `for` Loops

- **Keep all three expressions present** when possible. A loop with missing expressions is harder to understand and may be an accidental infinite loop.
- **Stick to one comparison style** for array bounds. If you use `i < size`, use it everywhere. Mixing `< size` and `<= size - 1` invites off-by-one errors.
- **Name your loop variables sensibly**: `i`, `j`, `k` are conventional for simple loops; use descriptive names for complex logic.

---

## 7. The Range-Based `for` Loop (C++11)

### Motivation

Iterating over every element in a collection (array, vector, string) is extremely common. The classic `for` loop requires you to manage indexes, bounds, and increments — all of which can go wrong. The range-based `for` loop eliminates this bookkeeping: it simply visits each element in turn.

### Syntax

```cpp
for (type variable_name : collection) {
    // body, with `variable_name` bound to each element
}
```

### Example: Iterating an Array

```cpp
int scores[] = {100, 90, 97};

for (int score : scores) {
    std::cout << score << " ";   // 100 90 97
}
```

That is it. No index, no size calculation, no bounds checking — the compiler handles it.

### Using `auto` to Deduce the Type

You do not have to spell out the element type. The `auto` keyword asks the compiler to deduce it:

```cpp
for (auto score : scores) {
    std::cout << score << " ";
}
```

This becomes increasingly valuable with complex collection types (e.g., iterating over a `vector` of `pair`s of `string`s).

### Example: Computing an Average

```cpp
std::vector<double> temps = {87.9, 77.9, 80.0, 72.5};
double total = 0.0;

for (auto temp : temps) {
    total += temp;
}

if (temps.size() != 0) {
    double average = total / temps.size();
    std::cout << "Average: " << average << "\n";
}
```

Notice the guard against division by zero: *always* check that a collection is not empty before dividing by its size.

### Iterating Over a String

A `string` is a collection of characters, so the range-based `for` loop works naturally:

```cpp
std::string text = "this is a test";
for (auto c : text) {
    std::cout << c;
}
// Output: this is a test
```

You can filter or transform characters on the fly:

```cpp
std::string text = "this is a test";
for (auto c : text) {
    if (c != ' ') {
        std::cout << c;
    }
}
// Output: thisisatest
```

### Initializer Lists as Collections

You can even use a braced initializer list directly:

```cpp
for (auto val : {1, 2, 3, 4, 5}) {
    std::cout << val << " ";
}
```

This is handy for quick loops over a known set of values.

### Best Practices

- Prefer the range-based `for` loop **whenever you need every element** and do not need the index.
- Use `auto` to avoid typing complex type names.
- Remember: the loop variable is a **copy** of each element. If you need to modify the original elements, use a reference (`auto&`). If you want to avoid copying large elements without modifying them, use `const auto&`.

---

## 8. The `while` Loop

### Motivation

Sometimes you do not know in advance how many iterations you will need. You want to loop *as long as* a condition holds — for example, keep asking the user for input until they enter a valid value. The `while` loop is designed for this.

### Syntax

```cpp
while (condition) {
    // loop body
}
```

The `while` loop is a **pre-test loop**: the condition is checked *before* each iteration. If the condition is false initially, the body never executes.

### Example: Counting with a `while` Loop

```cpp
int i = 1;
while (i <= 5) {
    std::cout << i << " ";
    i++;   // CRITICAL: must update the loop variable
}
```

**Warning:** If you forget to change `i` inside the loop, the condition never becomes false, and you get an **infinite loop**. With a `for` loop, the increment is written in the header; with `while`, you are responsible for updating the state yourself.

### Example: Input Validation

A very common pattern: keep prompting until the user provides valid input.

```cpp
int number = 0;
std::cout << "Enter an integer less than 100: ";
std::cin >> number;

while (number >= 100) {
    std::cout << "Invalid. Enter an integer less than 100: ";
    std::cin >> number;
}

std::cout << "Thanks!\n";
```

Here, the input prompt appears *twice* — once before the loop and once inside. This duplication is a clue that the code could be improved. We will see how `do-while` fixes it in the next section.

### Example: While Loop with an Array

```cpp
int scores[] = {100, 90, 87};
int i = 0;

while (i < 3) {
    std::cout << scores[i] << " ";
    i++;
}
```

### Using a Boolean Flag

A common pattern is to control the loop with a `bool` flag:

```cpp
bool done = false;
int number = 0;

while (!done) {
    std::cout << "Enter an integer between 2 and 4: ";
    std::cin >> number;

    if (number <= 1 || number >= 5) {
        std::cout << "Out of range. Try again.\n";
    } else {
        done = true;
    }
}
```

This avoids duplicating the prompt. The flag clearly communicates *why* the loop continues.

### KISS Principle

Keep It Simple. There are many ways to write the same loop; choose the one that is **easiest to read and reason about**. Clever code is not a virtue if it confuses the next person (who might be you in six months).

---

## 9. The `do-while` Loop

### Motivation

Sometimes you need the loop body to execute **at least once**, regardless of the condition. A menu system is the classic example: you must display the menu once before checking whether the user wants to quit. The `do-while` loop is a **post-test loop** — the condition is checked *after* each iteration.

### Syntax

```cpp
do {
    // loop body
} while (condition);   // note the semicolon!
```

### Example: Input Validation Without Duplication

Compare this to the `while` version earlier:

```cpp
int number = 0;

do {
    std::cout << "Enter an integer between 2 and 4: ";
    std::cin >> number;
} while (number <= 1 || number >= 5);

std::cout << "Thanks!\n";
```

The prompt appears only once, because the body runs first and the condition is checked afterward. The logic is cleaner, and there is no duplicated input statement.

### Example: Menu System

```cpp
char selection = ' ';

do {
    std::cout << "\n--- MENU ---\n";
    std::cout << "1. Do this\n";
    std::cout << "2. Do that\n";
    std::cout << "3. Do something else\n";
    std::cout << "Q. Quit\n";
    std::cout << "Enter your selection: ";
    std::cin >> selection;

    if (selection == '1') {
        std::cout << "You chose 1 — doing this.\n";
    } else if (selection == '2') {
        std::cout << "You chose 2 — doing that.\n";
    } else if (selection == '3') {
        std::cout << "You chose 3 — doing something else.\n";
    } else if (selection == 'q' || selection == 'Q') {
        std::cout << "Goodbye.\n";
    } else {
        std::cout << "Unknown option. Try again.\n";
    }
} while (selection != 'q' && selection != 'Q');
```

This is the **menu-driven program** pattern, one of the most common uses of `do-while`. You could replace the `if` / `else if` chain with a `switch` on `selection` — same logic, often clearer.

### Choosing Between `while` and `do-while`

| Use `while` when... | Use `do-while` when... |
|---|---|
| The loop may need to run zero times | The loop must run at least once |
| The condition can be checked upfront | The condition depends on the loop body's results |

---

## 10. `continue` and `break`

### Motivation

Sometimes you need finer control inside a loop: skip the rest of the current iteration, or exit the loop entirely before the condition naturally becomes false. C++ provides two statements for this.

### `continue`

When executed, `continue` stops the current iteration **immediately** and jumps back to the top of the loop. For a `for` loop, this means the increment expression runs, and then the condition is checked. For a `while` or `do-while` loop, this means the condition is checked.

```cpp
for (int i = 1; i <= 10; i++) {
    if (i % 2 != 0) {
        continue;   // skip odd numbers
    }
    std::cout << i << " ";   // 2 4 6 8 10
}
```

Think of it as: *"Skip the rest of this iteration and move on to the next."*

### `break`

When executed, `break` **terminates the loop entirely** and transfers control to the statement immediately following the loop.

```cpp
for (int val : data) {
    if (val == -99) {
        break;      // stop processing entirely
    }
    if (val == -1) {
        continue;   // skip this noisy value
    }
    std::cout << val << " ";
}
```

### Combined Example

Suppose a data stream contains valid values, noise (`-1`), and a stop signal (`-99`):

```cpp
std::vector<int> data = {1, 2, -1, 3, -1, 4, -99, 5, 6};

for (auto val : data) {
    if (val == -99) {
        break;          // stop signal: exit the loop
    } else if (val == -1) {
        continue;       // noise: skip this value
    } else {
        std::cout << val << " ";   // valid data: process it
    }
}
// Output: 1 2 3 4
```

### Best Practice: Do Not Overuse

`continue` and `break` are powerful, but too many of them make a loop hard to follow. A loop with ten `break`s and twelve `continue`s is a debugging nightmare. **The fewer exit points a loop has, the easier it is to understand.** If your loop becomes tangled, consider refactoring it into a clearer structure.

---

## 11. Infinite Loops

### Definition

An **infinite loop** (or endless loop) has a condition that always evaluates to `true`, so it never terminates on its own.

### Accidental Infinite Loops

Most infinite loops are bugs. Classic causes:

```cpp
// Forgetting to update i — the condition never becomes false
int i = 1;
while (i <= 5) {
    std::cout << i << " ";   // infinite: i stays 1
}

// Off-by-one in a down-counting loop
for (int i = 10; i > 0; i--) {
    std::cout << i << " ";
    i++;   // incrementing in a decrementing loop — never reaches 0
}
```

When you suspect an infinite loop, print debug output or use a debugger to inspect the loop variable.

### Intentional Infinite Loops

Some programs *should* loop forever. Two legitimate examples:

- **Event loops** in GUI applications, embedded systems, and game engines: they loop continuously, waiting for mouse clicks, key presses, or sensor input, and react when events occur.
- **Operating systems**: the OS kernel loops continuously, handling I/O, scheduling processes, and managing resources — until you shut down the machine.

In C++, all three loop forms can express an infinite loop:

```cpp
for (;;) { /* ... */ }           // for with no expressions
while (true) { /* ... */ }       // condition is always true
do { /* ... */ } while (true);   // same
```

### Breaking Out of an Infinite Loop

Intentional infinite loops use `break` to exit under specific conditions:

```cpp
while (true) {
    char again;
    std::cout << "Loop again? (y/n): ";
    std::cin >> again;

    if (again == 'n' || again == 'N') {
        break;
    }
}
```

This works, but it is often considered better style to write the loop condition so it *expresses* the termination criterion:

```cpp
char again;
do {
    std::cout << "Loop again? (y/n): ";
    std::cin >> again;
} while (again != 'n' && again != 'N');
```

**Rule of thumb:** Use an infinite loop only when the loop's terminating condition genuinely cannot be evaluated at the top (e.g., it depends on an event that arrives asynchronously). Otherwise, prefer a descriptive condition.

---

## 12. Nested Loops

### Motivation

Just as `if` statements can be nested, loops can be nested. Nested loops are the natural tool for **multi-dimensional structures** — 2D arrays, grids, tables, matrices — and for generating combinations (like a multiplication table).

### The Basic Pattern

```cpp
for (int outer = 1; outer <= 2; outer++) {
    for (int inner = 1; inner <= 3; inner++) {
        std::cout << "(" << outer << "," << inner << ") ";
    }
    std::cout << "\n";
}
```

Output:

```
(1,1) (1,2) (1,3)
(2,1) (2,2) (2,3)
```

**Key insight:** For *each* iteration of the outer loop, the inner loop runs to completion. So the inner loop runs `outer_count × inner_count` times total. We say the **inner loop iterates faster** than the outer loop.

### Example: Multiplication Table

```cpp
for (int num1 = 1; num1 <= 10; num1++) {
    for (int num2 = 1; num2 <= 10; num2++) {
        std::cout << num1 << " * " << num2 << " = " << (num1 * num2) << "\n";
    }
    std::cout << "----------------\n";   // separator after each table
}
```

The separator line executes after the inner loop completes — that is, once per outer iteration.

### Example: 2D Array

```cpp
const int ROWS = 5;
const int COLS = 3;
int grid[ROWS][COLS];

// Initialize every element
for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
        grid[row][col] = 1000;
    }
}

// Display every element
for (int row = 0; row < ROWS; row++) {
    for (int col = 0; col < COLS; col++) {
        std::cout << grid[row][col] << " ";
    }
    std::cout << "\n";
}
```

This **row-then-column** traversal is the standard pattern for 2D data. You will use it again and again.

### Example: Vector of Vectors

A `vector<vector<int>>` is a 2D structure whose inner vectors can have *different sizes* (unlike a fixed-size array). Range-based `for` loops with `auto` make traversal trivial:

```cpp
std::vector<std::vector<int>> vec2d = {
    {1, 2, 3},
    {10, 20, 30, 40},
    {100, 200, 300, 400, 500}
};

for (const auto& vec : vec2d) {       // outer: each inner vector, no copy
    for (auto val : vec) {            // inner: each integer
        std::cout << val << " ";
    }
    std::cout << "\n";
}
```

Output:

```
1 2 3
10 20 30 40
100 200 300 400 500
```

For the outer loop, `auto` is deduced as `std::vector<int>` (so `vec` has type `const std::vector<int>&`); for the inner loop, `auto` is deduced as `int`.

### Example: Histogram

Nested loops are also useful for visualizations. Suppose you have a vector of integers, and you want to print a bar of dashes for each:

```cpp
std::vector<int> data;
// ... fill data with user input ...

for (auto val : data) {           // for each data value
    for (int i = 0; i < val; i++) {   // print val dashes
        std::cout << "-";
    }
    std::cout << "\n";
}
```

You can add visual cues — for example, replace every fifth dash with an asterisk:

```cpp
for (auto val : data) {
    for (int i = 1; i <= val; i++) {
        if (i % 5 == 0) {
            std::cout << "*";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "\n";
}
```

Notice the nesting hierarchy: an `if-else` inside a `for` inside another `for`. Proper indentation makes this structure visible at a glance.

### Best Practices for Nested Loops

- **Keep nesting shallow** when possible. Two levels is common; three is manageable; four or more usually signals that refactoring would help.
- **Use descriptive loop variable names**: `row`/`col`, `outer`/`inner` — better than `i`/`j` for non-trivial logic.
- **Use block statements** `{}` for loop bodies, even single statements, so you can add code later without introducing bugs.

---

# Summary: Choosing the Right Tool

| Construct | Use When... |
|---|---|
| `if` | Execute code only if a condition holds |
| `if-else` | Choose between two alternatives |
| Nested `if` | Conditions depend on other conditions |
| `switch` | Dispatch on many discrete integral/char values |
| `?:` | Select between two values inside an expression |
| `for` | Loop a known number of times; index-based access |
| Range-based `for` | Visit every element of a collection |
| `while` | Loop while a condition holds (possibly zero times) |
| `do-while` | Loop while a condition holds, but execute at least once |
| `continue` | Skip the rest of the current iteration |
| `break` | Exit the loop early |
| Infinite loop | Continuous event processing (with a `break` to exit) |
| Nested loops | Multi-dimensional data, tables, grids |

## Universal Best Practices

1. **Indent consistently.** The structure of your code should be visually obvious.
2. **Use curly braces** around conditional and loop bodies — even single statements. It prevents bugs during future edits.
3. **Declare loop variables in the loop header** (`for (int i = ...)`) to limit their scope.
4. **Test as you go.** Write a small piece, test it, then add more. Do not build a hundred-line function and debug it all at once.
5. **Keep it simple.** Avoid clever tricks. If you have to think hard to understand your own loop, rewrite it.
6. **Use a debugger** when loops go wrong. Watching variables change step-by-step reveals what is actually happening — often different from what you assume.
7. **Prefer the range-based `for`** for iterating over collections. It is cleaner and less error-prone.
8. **Always check collection sizes** before dividing (e.g., computing an average) to avoid division by zero.

## The Big Picture

You now have the three fundamental building blocks of programming:

- **Sequence** — statements run in order.
- **Selection** — `if`, `else`, `switch`, `?:` choose which statements run.
- **Iteration** — `for`, `while`, `do-while` repeat statements.

With these three tools, **you can implement any algorithm**. The challenge is not memorizing syntax — it is learning to decompose problems into these building blocks. Practice by writing small programs, tracing loops by hand, and experimenting with variations. The more you build, the more fluent these constructs become.
