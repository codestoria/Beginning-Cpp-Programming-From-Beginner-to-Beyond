# C++ Control Flow: Selection Structures

## Table of Contents

1. [Introduction: Why Control Structures Matter](#1-introduction-why-control-structures-matter)
   - [How to Read This Tutorial](#how-to-read-this-tutorial)
   - [About the Code Examples](#about-the-code-examples)
2. [How C++ Evaluates Conditions (Truthiness & Short-Circuiting)](#2-how-c-evaluates-conditions-truthiness--short-circuiting)
3. [The `if` Statement](#3-the-if-statement)
   - [Grouping Statements with Curly Braces (Block Scope)](#grouping-statements-with-curly-braces-block-scope)
4. [The `if-else` Statement](#4-the-if-else-statement)
5. [Chaining Decisions with `else if`](#5-chaining-decisions-with-else-if)
6. [Nested `if` Statements and the Dangling `else` Problem](#6-nested-if-statements-and-the-dangling-else-problem)
7. [The `switch` Statement](#7-the-switch-statement)
   - [Understanding Fall-Through Behavior](#understanding-fall-through-behavior)
   - [Variable Scope Inside `switch` Cases](#variable-scope-inside-switch-cases)
   - [Using `switch` with Enumerations](#using-switch-with-enumerations)
   - [`switch` vs. `if-else`: How to Choose](#switch-vs-if-else-how-to-choose)
8. [The Conditional Operator (`?:`)](#8-the-conditional-operator)
9. [Summary and Best Practices](#9-summary-and-best-practices)

---

## 1. Introduction: Why Control Structures Matter

Every basic program executes **sequentially** — one statement after another, from top to bottom, without skipping or repeating any lines.
In computer science, this is called **sequence**, the first of the three fundamental building blocks of structured programming.

While sequence alone can handle straightforward formulas, real-world software must react dynamically to changing inputs and states.
An ATM cannot dispense cash if an account balance is insufficient; a game cannot end a match until a player's health drops to zero.
To build responsive programs, you need two additional building blocks:

| Building Block | Purpose | C++ Constructs |
| :--- | :--- | :--- |
| **Sequence** | Execute statements one after another in order | Any list of statements |
| **Selection** | Choose which path of code to run based on a condition | `if`, `if-else`, `else if`, `switch`, `?:` |
| **Iteration** | Repeat a block of code multiple times | `for`, range-based `for`, `while`, `do-while` |

> **The Fundamental Theorem of Structured Programming:** Using *only* sequence, selection, and iteration, you can express any computable algorithm, regardless of its complexity.

This tutorial focuses entirely on **selection structures** — how C++ evaluates conditions and routes execution down different paths.
For every construct, we will answer three core questions:
1. **Why does it exist?** (What limitation of previous tools does it solve?)
2. **How does it work?** (What are its exact syntax and runtime mechanics?)
3. **When should you use it?** (How does it compare to alternatives?)

### How to Read This Tutorial

- **Grasp the "Why" before memorizing the "How."** Each section starts with a concrete programming problem. Understanding why sequential code falls short will make the new syntax feel logical rather than arbitrary.
- **Trace execution like a CPU.** When reading code examples, evaluate the condition first, then follow only the active branch.
- **Compile and experiment.** Modify values, test boundary cases (such as `0` or negative numbers), and intentionally introduce syntax errors to learn how your compiler reports them.

### About the Code Examples

To keep the focus on control flow, most examples in this guide are **code fragments** rather than full source files. To run any fragment on your machine, place it inside a `main()` function and include the `<iostream>` header for console input and output:

```cpp
#include <iostream>  // Required for std::cout and std::cin

int main() {
    // Paste any code fragment here
    return 0;
}
```

Throughout this tutorial, we explicitly write the `std::` namespace prefix (such as `std::cout` instead of `cout`). This is a standard C++ best practice: it prevents naming collisions and makes it immediately clear which components belong to the C++ Standard Library.

---

## 2. How C++ Evaluates Conditions (Truthiness & Short-Circuiting)

Before writing selection statements, you need to understand what C++ expects inside a condition. Every selection statement relies on a **control expression** enclosed in parentheses:

```cpp
if (control_expression)
```

When C++ evaluates `control_expression`, it performs a **contextual conversion to `bool`**:

- **Truthy values (`true`):** The boolean literal `true`, or any **non-zero** numeric value (e.g., `1`, `-5`, `3.14`), or a non-null pointer.
- **Falsy values (`false`):** The boolean literal `false`, or **zero** (e.g., `0`, `0.0`, `'\0'`), or the null pointer `nullptr`.

In practice, you will most often build control expressions using **relational operators** (`==`, `!=`, `<`, `<=`, `>`, `>=`) and **logical operators** (`&&` for logical AND, `||` for logical OR, `!` for logical NOT), all of which directly produce a `bool` result.

### Short-Circuit Evaluation

C++ evaluates logical `&&` and `||` from left to right and stops as soon as the final outcome is guaranteed. This behavior is called **short-circuit evaluation**:

- In `A && B`: If `A` is `false`, the entire expression cannot be `true`, so `B` is **never evaluated**.
- In `A || B`: If `A` is `true`, the entire expression is already `true`, so `B` is **never evaluated**.

This is not just a performance optimization — it is a vital safety mechanism. For example, in `if (divisor != 0 && (total / divisor) > 5)`, if `divisor` is `0`, the right side is skipped entirely, preventing a crash from division by zero.

> **Common Beginner Pitfall (`=` vs. `==`):**
> - `=` is the **assignment operator** (it stores a value into a variable).
> - `==` is the **equality operator** (it compares two values).
>
> If you accidentally write `if (x = 5)`, C++ assigns `5` to `x`, evaluates the result of the assignment (`5`), sees that `5` is non-zero, and **always** treats the condition as `true`. Always double-check your equality comparisons.

---

## 3. The `if` Statement

### Motivation

Often, an action should occur **only when** a specific condition is met. For example, in a role-playing game, a healing potion should restore health *only if* the player's health is below maximum. If health is already full, the program should do nothing. Because sequential code cannot skip statements on its own, we need a conditional gate: the `if` statement.

### Syntax and Mechanics

```cpp
if (control_expression) {
    // Statements executed ONLY when control_expression is true
}
```

**Step-by-step execution:**

1. The CPU evaluates `control_expression` and converts the result to `bool`.
2. If `true`, execution enters the curly braces and runs the statements inside.
3. If `false`, execution skips the entire block and resumes immediately after the closing brace `}`.

### Example

```cpp
int temperature = 32;

if (temperature <= 32) {
    std::cout << "Warning: Freezing temperatures detected.\n";
}

std::cout << "Weather check complete.\n";
```

**Execution trace:**

- With `temperature = 32`: `32 <= 32` is `true`. The program prints `"Warning: Freezing temperatures detected."` followed by `"Weather check complete."`
- With `temperature = 70`: `70 <= 32` is `false`. The warning block is skipped, and only `"Weather check complete."` is printed.

### Grouping Statements with Curly Braces (Block Scope)

By rule, an `if` statement controls only the **single** statement immediately following it. To make multiple statements depend on the same condition, you must wrap them in **curly braces `{}`**. In C++, curly braces create a **compound statement** (commonly called a **block**), which the compiler treats as a single unit.

```cpp
int score = 120;

if (score > 100) {
    std::cout << "New high score!\n";
    score = 100;  // Cap the score at 100
}
```

#### How Blocks Govern Variable Lifetime (Scope)

In addition to grouping statements, curly braces define a **local scope**. Any variable declared inside an `if` block is created when execution enters the block and is automatically destroyed the moment execution exits the closing brace `}`:

```cpp
int num = 25;

if (num > 10) {
    int difference = num - 10;  // 'difference' exists ONLY inside this block
    std::cout << "Exceeds threshold by: " << difference << "\n";
}

// std::cout << difference;  // COMPILER ERROR: 'difference' was destroyed at '}'
```

This strict lifetime rule prevents temporary helper variables from cluttering the rest of your function or being accidentally reused later.

> **Best Practice:** Always use curly braces `{}` for `if` bodies — even when there is only one statement. Omitting braces saves two keystrokes today, but if you add a second statement later and forget the braces, that new statement will run unconditionally, introducing a silent bug.

---

## 4. The `if-else` Statement

### Motivation

Many decisions are not "act or do nothing," but rather **"choose between Action A and Action B."** Consider verifying a user's password: if it matches, grant access; *otherwise*, display an error message. While you could write two separate `if` statements (`if (valid)` and `if (!valid)`), doing so evaluates the condition twice and fails to communicate that the two outcomes are mutually exclusive. The `if-else` statement models a binary choice directly.

### Syntax and Mechanics

```cpp
if (control_expression) {
    // Branch A: Runs when control_expression is true
} else {
    // Branch B: Runs when control_expression is false
}
```

**Execution guarantee:**

1. C++ evaluates `control_expression` once.
2. If `true`, Branch A runs and Branch B is skipped.
3. If `false`, Branch A is skipped and Branch B runs.
4. **Exactly one** of the two branches executes — never both, and never neither.

### Example

```cpp
int number = 0;
std::cout << "Enter an integer: ";
std::cin >> number;

if (number % 2 == 0) {
    std::cout << number << " is even.\n";
} else {
    std::cout << number << " is odd.\n";
}
```

Here, the modulo operator (`%`) computes the remainder of `number / 2`. If the remainder equals `0`, the first branch runs; otherwise, the `else` branch runs.

---

## 5. Chaining Decisions with `else if`

### Motivation

Real-world logic frequently involves more than two mutually exclusive paths. For example, converting a numeric exam score into a letter grade (`A`, `B`, `C`, `D`, or `F`) requires five distinct ranges. A single `if-else` only provides two branches. To handle multiple exclusive conditions cleanly, C++ allows you to chain `else if` clauses.

### Syntax and Mechanics

```cpp
if (condition_1) {
    // Runs if condition_1 is true
} else if (condition_2) {
    // Runs if condition_1 is false AND condition_2 is true
} else if (condition_3) {
    // Runs if conditions 1 & 2 are false AND condition_3 is true
} else {
    // Fallback: Runs if ALL preceding conditions are false
}
```

**How it executes:**
C++ tests the conditions from top to bottom. **The first condition that evaluates to `true` wins:** its block executes, and the rest of the chain is skipped entirely. If every condition fails, the final `else` block (if provided) executes as a default fallback.

### Example: Letter Grade Calculator

```cpp
int score = 85;

if (score >= 90) {
    std::cout << "Grade: A\n";
} else if (score >= 80) {
    std::cout << "Grade: B\n";
} else if (score >= 70) {
    std::cout << "Grade: C\n";
} else if (score >= 60) {
    std::cout << "Grade: D\n";
} else {
    std::cout << "Grade: F\n";
}
```

**Why order matters:** When `score` is `85`, the expressions `score >= 70` and `score >= 60` are also mathematically true. However, because `score >= 80` is tested first and succeeds, C++ prints `"Grade: B"` and immediately exits the structure. Always order `else if` conditions from most restrictive (or highest threshold) to least restrictive.

---

## 6. Nested `if` Statements and the Dangling `else` Problem

### Motivation

Sometimes one question is only relevant *after* another condition has already been satisfied. For instance: *"If the user is logged in, check whether they have administrator privileges."* Because an `if` statement is itself a valid C++ statement, you can place one `if` (or `if-else`) inside the body of another `if`. This technique is called **nesting**.

### Example

```cpp
bool is_logged_in = true;
bool is_admin = false;

if (is_logged_in) {
    if (is_admin) {
        std::cout << "Welcome, Administrator.\n";
    } else {
        std::cout << "Welcome, Standard User.\n";
    }
} else {
    std::cout << "Please log in to continue.\n";
}
```

If `is_logged_in` is `false`, the entire outer block — including the inner `if (is_admin)` check — is skipped.

### The Dangling `else` Problem

If you nest `if` statements *without* curly braces, it becomes ambiguous which `if` owns an `else` clause. Examine the indentation in the following broken example:

```cpp
// DANGEROUS CODE: Visual indentation lies!
if (is_logged_in)
    if (is_admin)
        std::cout << "Welcome, Administrator.\n";
else
    std::cout << "Please log in to continue.\n";  // BUG!
```

Even though `else` is visually aligned under the outer `if (is_logged_in)`, **C++ ignores whitespace and indentation**. Instead, the compiler follows a strict syntactic rule:

> **An `else` clause always binds to the nearest preceding unmatched `if` within the same block.**

Consequently, the compiler attaches `else` to `if (is_admin)`. If `is_logged_in` is `true` and `is_admin` is `false`, the program erroneously outputs `"Please log in to continue."`

**The Solution:** Always wrap nested `if` statements in curly braces `{}` so ownership is explicit to both the compiler and human readers:

```cpp
if (is_logged_in) {
    if (is_admin) {
        std::cout << "Welcome, Administrator.\n";
    } else {
        std::cout << "Welcome, Standard User.\n";
    }
} else {
    std::cout << "Please log in to continue.\n";  // Explicitly bound to outer 'if'
}
```

---

## 7. The `switch` Statement

### Motivation

When you need to compare a single variable against a list of discrete constant values — such as menu choices (`'1'`, `'2'`, `'3'`) or movement keys (`'w'`, `'a'`, `'s'`, `'d'`) — a long `if / else if` chain becomes repetitive and verbose. The `switch` statement is a specialized selection construct designed for **multi-way branching on a single discrete value**.

### Syntax and Mechanics

```cpp
switch (control_expression) {
    case constant_1:
        // Statements for constant_1
        break;
    case constant_2:
        // Statements for constant_2
        break;
    default:
        // Statements if no case matches
        break;
}
```

**Strict Language Rules for `switch`:**

1. **Integral or Enumeration Types Only:** `control_expression` must evaluate to an integer type (`int`, `char`, `short`, `long`, `bool`) or an `enum`. You **cannot** use `switch` with floating-point numbers (`double`, `float`) or strings (`std::string`).
2. **Compile-Time Constants in `case` Labels:** Every `case` label must be a constant expression known at compile time (such as `1`, `'A'`, or a `constexpr` variable) and must be unique within that `switch`. Runtime variables cannot be used as `case` labels.
3. **The `break` Statement:** `break` immediately terminates the `switch` statement and transfers control to the line after its closing brace `}`.
4. **The `default` Label:** Optional, though strongly recommended. It functions like the final `else` in an `if-else` chain, handling any value that did not match a `case`.

### Example: Menu Selection

```cpp
char choice = '2';

switch (choice) {
    case '1':
        std::cout << "Starting new game...\n";
        break;
    case '2':
        std::cout << "Loading saved game...\n";
        break;
    case '3':
    case '4':
        std::cout << "Opening settings or help menu...\n";
        break;
    default:
        std::cout << "Invalid choice. Please try again.\n";
        break;
}
```

Notice how `case '3':` and `case '4':` are stacked together. Because there is no `break` after `case '3':`, both `'3'` and `'4'` execute the exact same code — functioning like a logical OR (`choice == '3' || choice == '4'`).

### Understanding Fall-Through Behavior

Unlike `if-else` branches, `case` labels in C++ are merely **entry points** (jump targets), not self-contained blocks.
Once execution jumps to a matching `case`, the CPU continues running line by line downward — even crossing right through subsequent `case` labels — until it encounters a `break` statement or reaches the closing brace `}` of the `switch`.
This behavior is known as **fall-through**.

```cpp
int level = 1;

switch (level) {
    case 1:
        std::cout << "Unlocked basic sword.\n";
        // Missing break! Execution falls straight into case 2:
    case 2:
        std::cout << "Unlocked wooden shield.\n";
        break;
    case 3:
        std::cout << "Unlocked magic staff.\n";
        break;
}
// Output when level == 1:
// Unlocked basic sword.
// Unlocked wooden shield.
```

Unintentional fall-through caused by a forgotten `break` is one of the most common bugs in C and C++.

- **Rule of thumb:** End *every* non-empty `case` with `break`.
- **Intentional fall-through:** If you deliberately omit `break` (for example, to stack cases or accumulate behavior), document it explicitly using the standard C++17 `[[fallthrough]];` attribute (or a clear comment) so both the compiler and other developers know the omission is intentional.

### Variable Scope Inside `switch` Cases

Because all `case` labels share the single overarching scope of the `switch` block, declaring and initializing a variable inside one `case` without braces can trigger a compiler error (`jump to case label crosses initialization`):

```cpp
switch (choice) {
    case '1': {  // Wrapping the case in braces creates a safe local scope
        int bonus = 50;
        std::cout << "Bonus awarded: " << bonus << "\n";
        break;
    }
    case '2':
        std::cout << "No bonus.\n";
        break;
}
```

Whenever you need to declare local variables inside a `case`, wrap that `case` body in curly braces `{}`.

### Using `switch` with Enumerations

`switch` pairs naturally with C++ scoped enumerations (`enum class`), which represent a fixed set of named constants:

```cpp
enum class TrafficLight { Red, Yellow, Green };

TrafficLight current = TrafficLight::Red;

switch (current) {
    case TrafficLight::Red:
        std::cout << "Stop.\n";
        break;
    case TrafficLight::Yellow:
        std::cout << "Prepare to stop.\n";
        break;
    case TrafficLight::Green:
        std::cout << "Go.\n";
        break;
}
```

Most modern compilers emit a helpful warning if you `switch` on an `enum class` and omit one of its enumerators (provided you do not mask missing cases with a `default` branch).

### `switch` vs. `if-else`: How to Choose

| Criterion | Choose `switch` when... | Choose `if-else` when... |
| --- | --- | --- |
| **Data Type** | Testing a single integer, `char`, or `enum`. | Testing `double`, `std::string`, or custom objects. |
| **Condition Type** | Checking exact equality (`x == 5`). | Checking ranges (`x >= 90`) or inequalities (`x < 0`). |
| **Complexity** | Dispatching across 3 or more discrete constants. | Combining multiple variables (`x > 0 && y < 10`). |

---

## 8. The Conditional Operator (`?:`)

### Motivation

Sometimes you need to choose between **two simple values** directly inside an expression — for instance, when initializing a variable or formatting text output. Writing a multi-line `if-else` statement just to pick between two values can add unnecessary clutter. C++ provides a compact inline alternative called the **conditional operator** (often called the **ternary operator**, as it is the only C++ operator that takes three operands).

### Syntax and Mechanics

```cpp
condition ? expression_if_true : expression_if_false
```

**How it evaluates:**

1. `condition` is evaluated and converted to `bool`.
2. If `true`, C++ evaluates *only* `expression_if_true`, which becomes the result of the entire expression.
3. If `false`, C++ evaluates *only* `expression_if_false`, which becomes the result of the entire expression.

Crucially, `?:` is an **expression** (it produces a value that can be assigned or passed to a function), whereas `if-else` is a **statement** (it controls execution flow without yielding a value).
Note that `expression_if_true` and `expression_if_false` must evaluate to compatible types so the compiler can determine a single result type for the expression.

### Examples

**1. Initializing a variable based on a condition:**

```cpp
int a = 15, b = 28;
int max_val = (a > b) ? a : b;  // max_val is initialized directly to 28
```

Compare this with `if-else`, which forces you to declare `max_val` uninitialized first and assign to it afterward:

```cpp
int max_val;
if (a > b) {
    max_val = a;
} else {
    max_val = b;
}
```

**2. Inline output formatting (pluralization):**

```cpp
int items = 1;
std::cout << "You have " << items << (items == 1 ? " item" : " items") << " in your cart.\n";
```

*(Note: Always wrap a conditional expression in parentheses when embedding it inside a larger expression like `std::cout << ...`, because `?:` has very low operator precedence.)*

**3. Guarding against division by zero:**

```cpp
int numerator = 100, denominator = 0;
int quotient = (denominator != 0) ? (numerator / denominator) : 0;
```

### Best Practices for `?:`

- **Use it to select values, not to trigger actions.** Avoid calling functions with side effects inside `?:`; use `if-else` when executing statements.
- **Never nest conditional operators.** Chained ternaries like `a ? b : c ? d : e` are notoriously hard to read. Whenever logic involves more than two outcomes, use `if / else if`.

---

## 9. Summary and Best Practices

### Quick Reference Guide

| Construct | Best Used For | Key Takeaway |
| --- | --- | --- |
| **`if`** | Running code only when a condition is true | Always use `{}` braces, even for single statements. |
| **`if-else`** | Choosing between two mutually exclusive paths | Exactly one branch is guaranteed to run. |
| **`else if`** | Testing a sequence of mutually exclusive conditions | Top-down evaluation; the first `true` condition wins. |
| **Nested `if`** | Dependent decisions (a check inside another check) | Use `{}` braces to avoid the dangling `else` bug. |
| **`switch`** | Dispatching on discrete `int`, `char`, or `enum` values | End every non-empty case with `break` to prevent fall-through. |
| **`?:`** | Choosing between two simple values inline | Keep it short and wrap in `()`; never nest ternary operators. |

### Golden Rules for Clean Selection Logic

1. **Always use curly braces `{}`.** They define explicit block scope, prevent dangling `else` bugs, and safeguard your code when new lines are added later.
2. **Indent consistently.** Indent every level inside `{}` uniformly (typically 4 spaces). While the compiler ignores whitespace, human readers depend on indentation to verify logical structure at a glance.
3. **Keep conditions positive and clear.** Prefer `if (is_ready)` over `if (!is_not_ready)`. Double negatives force readers to pause and mentally invert the logic.
4. **Verify `=` vs. `==`.** Double-check that every equality test uses two equals signs (`==`), not assignment (`=`).
5. **Minimize variable scope.** Declare variables inside the smallest block that needs them so they cannot be misused after that block ends.
