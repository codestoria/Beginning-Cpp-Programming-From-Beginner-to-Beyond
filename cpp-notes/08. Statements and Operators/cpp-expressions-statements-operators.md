# C++ Expressions, Statements, and Operators

## Table of Contents

1. [Introduction](#introduction)
2. [Expressions and Statements](#expressions-and-statements)
3. [An Overview of C++ Operators](#an-overview-of-c-operators)
4. [The Assignment Operator](#the-assignment-operator)
5. [Arithmetic Operators](#arithmetic-operators)
6. [Increment and Decrement Operators](#increment-and-decrement-operators)
7. [Mixed-Type Expressions and Type Conversion](#mixed-type-expressions-and-type-conversion)
8. [Equality Operators](#equality-operators)
9. [Relational Operators](#relational-operators)
10. [Logical Operators](#logical-operators)
11. [Compound Assignment Operators](#compound-assignment-operators)
12. [Operator Precedence and Associativity](#operator-precedence-and-associativity)
13. [Summary](#summary)

---

## Introduction

Every C++ program, no matter how large, is assembled from a small set of fundamental building blocks. Before you can write loops, functions, or classes, you need to understand how the language represents computation: **expressions**, **statements**, and **operators**. Everything else in C++ is built on top of these concepts, so a solid understanding of them will pay off throughout your programming career.

This tutorial explains:

- What expressions and statements are, and how they differ.
- The main categories of C++ operators and what each one does.
- The rules that determine how operators combine (precedence and associativity).
- The subtle details — such as integer division, prefix vs. postfix increment, and short-circuit evaluation — that commonly trip up beginners but become second nature with practice.

The goal is not just to show you *what* C++ does, but to explain *why* it behaves the way it does. Once you understand the reasoning behind the rules, you will be able to read and write C++ expressions with confidence and avoid the most common pitfalls.

---

## Expressions and Statements

### What Is an Expression?

The C++ standard defines an expression as:

> A sequence of operators and operands that specifies a computation.

In simpler terms, an expression is anything that produces a **value**. That value might be a number, a character, a boolean, or something more complex. Here are some examples:

```cpp
34              // a literal: evaluates to 34
favorite_number // a variable: evaluates to its current value
1.5 + 2.8       // evaluates to 4.3
2 * 5           // evaluates to 10
a > b           // evaluates to true or false
a = b           // assignment: evaluates to the value assigned
```

Notice the last example. Assignment is itself an expression because it produces a value — the value that was just assigned. This fact becomes important later when we discuss chained assignments.

Expressions are the **most basic building blocks** in programming. They are the raw material from which statements are made.

### What Is a Statement?

A statement is a **complete unit of code that performs an action**. In C++, statements are almost always terminated with a semicolon (`;`). Forgetting that semicolon is one of the most common beginner errors.

A statement usually *contains* one or more expressions, but it does more than just compute a value — it *does* something: it declares a variable, assigns a value, reads input, prints output, or controls the flow of the program.

Here are some examples of statements:

```cpp
int x;                              // declaration statement
favorite_number = 12;               // assignment statement
2 + 3;                              // expression statement (computes 5, then discards it)
x = 2 * 5;                          // assignment statement
if (a > b) cout << "a is bigger";   // if statement (contains expressions)
;                                   // null statement (does nothing)
```

The third example is worth noting. `2 + 3;` is a legal statement — it is an expression followed by a semicolon. But it is also useless, because the computed result is thrown away. The compiler will not complain, but nothing observable happens.

The **null statement** (a lone semicolon) performs no action. It is rarely used, but it occasionally comes in handy where the grammar requires a statement and you have nothing to do.

### The Relationship Between Them

The relationship can be summarized as:

> **Expressions** are the building blocks of **statements**.
> **Statements** are the building blocks of **programs**.

Think of it this way: an expression is a *phrase* that has a value; a statement is a complete *sentence* that tells the program to do something.

---

## An Overview of C++ Operators

Operators are the symbols that let you combine operands (values or variables) into expressions. C++ has a rich set of operators. They are classified by how many operands they take:

| Arity | Name | Example |
|-------|------|---------|
| One | Unary | `-x` (negation), `++i` |
| Two | Binary | `a + b`, `a = b` |
| Three | Ternary | `cond ? x : y` |

Most C++ operators are **binary** — they operate on two operands. The multiplication operator `*` is a classic example. A few are **unary**: the unary minus `-` negates a single value, and the increment/decrement operators also take one operand. There is exactly one **ternary** operator in C++: the conditional operator `?:`.

C++ operators are grouped into several categories:

| Category | Purpose | Examples |
|----------|---------|----------|
| Assignment | Change the value of an object | `=`, `+=`, `-=` |
| Arithmetic | Perform mathematical operations | `+`, `-`, `*`, `/`, `%` |
| Increment / Decrement | Add or subtract 1 | `++`, `--` |
| Relational (Comparison) | Compare two values | `<`, `>`, `<=`, `>=` |
| Equality | Test equality / inequality | `==`, `!=` |
| Logical | Combine boolean conditions | `!`, `&&`, `\|\|` |
| Member access | Access parts of objects or arrays | `.`, `->`, `[]` |
| Others | Miscellaneous | `?:`, `sizeof`, casts |

In the following sections, we will examine each of the important categories in turn, starting with assignment.

---

## The Assignment Operator

### What `=` Really Means

The assignment operator is a single equal sign: `=`. This is *not* a test for equality. It does not mean "is equal to." It means:

> **Evaluate the right-hand side, and store the result into the object named on the left-hand side.**

For example:

```cpp
num1 = 100;
```

This reads as: "evaluate the literal `100`, then store that value into the variable `num1`." It does not assert that `num1` equals `100`; it *changes* `num1` so that it now holds `100`.

This distinction is crucial. Beginners often confuse `=` (assignment) with `==` (equality comparison). The compiler will not always catch the mistake, but the logic of your program will be wrong. We will return to this point in [Equality Operators](#equality-operators).

### lvalues and rvalues

To understand assignment fully, you need two terms:

- **rvalue** — the *contents* of a variable. In `num1 = 100`, the `100` is an rvalue (a value that can appear on the right-hand side).
- **lvalue** — the *location* of a variable. The `num1` on the left-hand side is an lvalue (something with a memory location that can be assigned to).

This “location vs. contents” description is an introductory simplification. Modern C++ value categories are more detailed (lvalue, xvalue, prvalue); you do not need the full model yet.

When you write `num1 = 100`, you are saying: "take the rvalue `100`, and store it into the lvalue `num1`."

This is why you cannot write:

```cpp
100 = num1;   // ERROR
```

A literal like `100` has no location in memory. It is not an lvalue, so it cannot be assigned to. The compiler will report an error such as "lvalue required as left operand of assignment."

Similarly, you cannot assign to a constant:

```cpp
const int num1 = 10;
num1 = 100;   // ERROR: assignment of read-only variable
```

### Initialization vs. Assignment

These two concepts look similar but are fundamentally different:

```cpp
int num1 = 10;   // initialization: the variable is created and given its first value
num1 = 100;      // assignment: an existing variable is given a new value
```

**Initialization** happens when a variable is declared — it receives its very first value. **Assignment** changes a value that already exists. In the example above, `num1` is initialized to `10` and later assigned `100`.

### Chained Assignment

Because an assignment expression evaluates to the value that was just assigned, you can chain assignments together:

```cpp
num1 = num2 = 1000;
```

Assignment associates **right to left**. The compiler first evaluates `num2 = 1000` (giving `num2` the value `1000`), and that expression evaluates to `1000`. Then it evaluates `num1 = 1000`, giving `num1` the value `1000`. Both variables end up with `1000`.

This is a convenient way to set several variables to the same value. However, it is considered poor practice to chain assignments containing complicated expressions. Use it only when assigning the *same simple value* to multiple variables:

```cpp
num1 = num2 = num3 = 0;   // fine: all set to zero
```

### Type Checking

C++ is **statically typed**. The compiler checks whether it makes sense to store the right-hand side into the left-hand side. If you try to assign a string to an integer:

```cpp
num1 = "Frank";   // ERROR: invalid conversion
```

the compiler will reject it. This is a feature, not a bug: many errors are caught at compile time rather than at runtime. Languages like Python and Ruby defer this checking until runtime; C++ does it before the program ever runs.

When types differ but a conversion exists, the compiler may convert automatically (see [Mixed-Type Expressions](#mixed-type-expressions-and-type-conversion)).

---

## Arithmetic Operators

C++ provides the five arithmetic operators you would expect:

| Operator | Meaning | Example |
|----------|---------|---------|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `%` | Modulo (remainder) | `a % b` |

For built-in types, the first four operators work with both integer and floating-point operands. The built-in modulo operator `%` works only with integral operands. (All of these operators can also be overloaded for user-defined types.)

### Integer Division

When both operands of `/` are integers, C++ performs **integer division**: the fractional part is discarded (truncated), not rounded.

```cpp
int numerator = 100, denominator = 200;
cout << numerator / denominator;   // prints 0, not 0.5
```

Since `100 / 200` is `0.5`, but both operands are integers, the result is truncated to `0`. This is a very common source of bugs. If you need a fractional result, at least one operand must be a floating-point type:

```cpp
cout << 5.0 / 10.0;   // prints 0.5
cout << 5 / 10;       // prints 0
```

### Modulo

The modulo operator `%` gives the **remainder** after division:

```cpp
cout << 200 % 100;  // 0  (200 ÷ 100 = 2 remainder 0)
cout << 10 % 3;     // 1  (10 ÷ 3 = 3 remainder 1)
```

Modulo is useful for tasks like determining whether a number is even (`n % 2 == 0`) or cycling through a fixed set of values.

Since C++11, integer division truncates toward zero, and `%` satisfies `(a / b) * b + a % b == a`. The sign of a nonzero result follows the left operand: `-7 % 3` is `-1`, and `7 % -3` is `1`. If you need a non-negative remainder, adjust it explicitly, e.g. `((n % m) + m) % m`.

### Operator Precedence in Arithmetic

Arithmetic follows the same precedence rules you learned in school: multiplication and division before addition and subtraction. Parentheses can override this:

```cpp
int num1 = 200, num2 = 100;

cout << num1 + num2 * 10;       // 200 + 1000 = 1200
cout << (num1 + num2) * 10;     // 300 * 10 = 3000
```

### Worked Example: Currency Converter

Here is a complete program that converts Euros to US dollars using a constant conversion factor:

```cpp
#include <iostream>
using namespace std;

int main() {
    const double dollars_per_euro = 1.19;
    double euros = 0.0;
    double dollars = 0.0;

    cout << "Welcome to the EUR to USD converter" << endl;
    cout << "Enter the value in Euros: ";
    cin >> euros;

    dollars = euros * dollars_per_euro;

    cout << euros << " Euros is equivalent to " << dollars << " dollars" << endl;
    return 0;
}
```

Running with `1` gives `1.19 dollars`; with `123.5` gives `146.965 dollars`.

This example demonstrates the typical structure of a program: **input → processing → output**. It also shows a good habit: declaring the conversion factor as `const` so it cannot accidentally change.

---

## Increment and Decrement Operators

The increment operator `++` adds `1` to its operand; the decrement operator `--` subtracts `1`. Both come in two forms:

- **Prefix**: `++counter` or `--counter` — the operator comes *before* the operand.
- **Postfix**: `counter++` or `counter--` — the operator comes *after* the operand.

When used alone on a line, the two forms behave identically:

```cpp
counter = counter + 1;
counter++;      // same effect
++counter;      // same effect
```

The difference appears when the operator is used **inside a larger expression**.

### Prefix: Increment Before Use

With prefix notation, the variable is incremented **before** its value is used:

```cpp
int counter = 10;
int result = ++counter + 10;
// ++counter yields the new value (11), so 11 + 10 = 21 is used for result.
// counter == 11, result == 21
```

### Postfix: Increment After Use

With postfix notation, the variable is incremented **after** its value is used:

```cpp
int counter = 10;
int result = counter++ + 10;
// counter++ yields the old value (10), so 10 + 10 = 20 is used for result.
// The increment is a side effect that completes by the end of the full expression.
// counter == 11, result == 20
```

### A Warning About Overuse

Increment and decrement are powerful and commonly used — especially in loops, where they are the natural way to advance a counter. But they can make code hard to read when embedded in complex expressions.

**Never modify the same variable more than once in a single full expression without sequencing between the modifications.** When two unsequenced modifications (or an unsequenced read and modification) apply to the same scalar object, the behavior is **undefined** — the standard does not say what should happen, and different compilers may produce different results:

```cpp
int i = 0;
cout << i++ + ++i;   // UNDEFINED BEHAVIOR — do not write this
```

The rule is about *unsequenced modification*, not about literally counting `++`/`--` tokens. For example, `(i++, ++i)` uses the comma operator, which sequences the two modifications, so that form is well-defined.

---

## Mixed-Type Expressions and Type Conversion

### Why Types Matter

C++ applies operators to operands of the **same type**. When you write `a + b` where `a` is an `int` and `b` is a `double`, the compiler must reconcile the types before it can add them. It does so by converting one operand to match the other. Understanding these conversions is essential, because *which* operand is converted affects the result.

### Higher vs. Lower Types

C++ does not define a single “information content” ordering across all arithmetic types. In mixed expressions, the **usual arithmetic conversions** apply: integral operands are first promoted (for example, `char` and `short` usually promote to `int`); if either operand is floating-point, the other is converted to a floating-point type, with `long double` taking precedence over `double`, and `double` over `float`; otherwise, the integer conversion ranks decide. A conversion can lose information even when it looks like “lower to higher”: converting a large `long` to `float` may lose precision, and converting `double` to an integer may truncate or overflow.

### Promotion and Demotion

- **Promotion**: usually a conversion to a type that can represent all values of the original type, such as integral promotion (`char`/`short` to `int`). This is generally safe, but “lower to higher” is only a rough intuition.
  ```cpp
  int i = 2;
  double d = 5.2;
  double result = i * d;   // i is converted to double, then multiplied
  ```
- **Demotion / narrowing**: a conversion to a type that may not represent all values of the original type. It may lose information.
  ```cpp
  int num = 100.2;     // copy-initialization: 100.2 is converted to int; .2 is truncated
  // int num2{100.2};  // ERROR: narrowing conversion in list-initialization
  ```

Most compilers warn about demotions that lose precision, but not all do. Be careful.

### Explicit Casting with `static_cast`

Sometimes you want to control the conversion yourself. Modern C++ provides `static_cast`:

```cpp
static_cast<double>(total)
```

This tells the compiler: "convert `total` to a `double`." The syntax is `static_cast<type>(expression)`.

### Worked Example: Average of Three Integers

Suppose you want the average of three integers. The natural approach:

```cpp
int total = 50;
int count = 3;
double average = total / count;   // BUG: integer division gives 16, not 16.667
```

Because both `total` and `count` are integers, `total / count` performs integer division and truncates. The fix is to cast one operand to `double`:

```cpp
double average = static_cast<double>(total) / count;   // 16.6667
```

Now the compiler sees a `double` divided by an `int`. It promotes the `int` to `double` automatically and performs floating-point division.

> **Note:** Older C code used a C-style cast `(double)total`. Modern C++ prefers `static_cast` because it is more explicit and safer. Use `static_cast`.

---

## Equality Operators

The equality operators compare two values and produce a **boolean** result (`true` or `false`):

| Operator | Meaning |
|----------|---------|
| `==` | Equal to |
| `!=` | Not equal to |

### The Classic Beginner Mistake

A single `=` is **assignment**; a double `==` is **comparison**. Writing `if (x = 100)` assigns `100` to `x`; the value of the assignment expression (`100`) is then converted to `bool` for the condition — for `int`, nonzero becomes `true`, so the branch always runs. This is almost never what you meant. Always double-check that you typed `==` when comparing.

### Using Boolean Results

Equality expressions are usually used inside statements, but you can also store their result:

```cpp
bool result = (100 == 50 + 50);   // true
bool not_equal = (num1 != num2);  // true if num1 and num2 differ
```

### Displaying `true` / `false` with `boolalpha`

By default, C++ prints booleans as `1` and `0`. To print the words `true` and `false`, use the `boolalpha` stream manipulator:

```cpp
cout << boolalpha << (10 == 10);   // prints "true"
cout << noboolalpha << (10 == 10); // prints "1" again
```

### A Caution About Floating-Point Equality

Floating-point numbers are stored as **approximations**. Many decimal values — `0.1`, `0.2`, and `0.3`, for example — cannot be represented exactly in binary floating point, so the stored value is the nearest representable number. As a result, comparing two floating-point values with `==` can produce surprising results:

```cpp
double a = 0.1 + 0.2;
double b = 0.3;
cout << (a == b);               // typically prints 0
cout << boolalpha << (a == b);  // typically prints "false"
```

The sum `0.1 + 0.2` rounds to a value slightly different from the stored value of `0.3`, so the two are not exactly equal.

For applications that require precise decimal arithmetic (scientific, medical, financial), use specialized libraries rather than built-in `double`. For everyday comparisons, avoid `==` on floating-point values and instead check whether the difference is below a small tolerance:

```cpp
#include <cmath>
bool close = std::fabs(a - b) < 1e-9;
```

---

## Relational Operators

Relational operators compare two values and produce a boolean:

| Operator | Meaning |
|----------|---------|
| `>` | Greater than |
| `>=` | Greater than or equal to |
| `<` | Less than |
| `<=` | Less than or equal to |

These are intuitive and work exactly as you would expect:

```cpp
10 > 20    // false
10 >= 20   // false
10 < 20    // true
10 <= 20   // true
20 >= 20   // true
```

> **C++20 note:** C++20 introduces a three-way comparison operator `<=>`. It returns a comparison category object (such as `std::strong_ordering`) that can be compared with `0` to determine whether the left operand is less than, equal to, or greater than the right operand. You will not need it as a beginner, but it is worth knowing it exists.

### Testing Edge Cases

When writing conditions like "is `x` between 10 and 20?", pay careful attention to whether the bounds are inclusive or exclusive, and test the boundary values:

```cpp
// "between 10 and 20" — exclusive on both ends
x > 10 && x < 20
```

Testing only the middle of a range hides bugs; always test the edges (`10` and `20`) as well.

---

## Logical Operators

Logical operators combine boolean expressions into more complex conditions. C++ has three:

| Operator | Keyword form | Meaning |
|----------|--------------|---------|
| `!` | `not` | Logical NOT (unary) |
| `&&` | `and` | Logical AND (binary) |
| `\|\|` | `or` | Logical OR (binary) |

The keyword forms (`not`, `and`, `or`) are valid C++ but rarely seen in practice. Most code uses the symbolic forms.

### Truth Tables

**NOT** negates its operand:

| `a` | `!a` |
|-----|------|
| true | false |
| false | true |

**AND** is true only when *both* operands are true:

| `a` | `b` | `a && b` |
|-----|-----|----------|
| true | true | true |
| true | false | false |
| false | true | false |
| false | false | false |

**OR** is true when *either* operand is true:

| `a` | `b` | `a \|\| b` |
|-----|-----|------------|
| true | true | true |
| true | false | true |
| false | true | true |
| false | false | false |

### Precedence

Logical operators have their own precedence order:

> `!` (highest) > `&&` > `||` (lowest)

Parentheses can override this, and using them is encouraged for clarity.

### Short-Circuit Evaluation

C++ stops evaluating a logical expression **as soon as the result is known**:

- In `a && b && c`, if `a` is false, the entire expression is false — so `b` and `c` are never evaluated.
- In `a || b || c`, if `a` is true, the entire expression is true — so `b` and `c` are never evaluated.

This is called **short-circuit evaluation**. It is not just an optimization; it affects program behavior. It applies to the built-in `&&` and `||` only — an overloaded `operator&&` / `operator||` is an ordinary function call and evaluates both operands. For example:

```cpp
if (ptr != nullptr && ptr->value > 0) { ... }
```

Here, `ptr->value` is only accessed if `ptr` is non-null. Without short-circuiting, this code would crash when `ptr` is null.

### Worked Examples

**Range check (inclusive):**

```cpp
int num = 15;
bool within = (num >= 10 && num <= 20);   // true
```

**Outside a range:**

```cpp
bool outside = (num < 10 || num > 20);    // false for num = 15
```

Using `&&` here would be a bug: no number can be both less than 10 *and* greater than 20 simultaneously.

**On the boundary:**

```cpp
bool on_bound = (num == 10 || num == 20); // true only at the endpoints
```

**Combining temperature and wind:**

```cpp
// Need a coat if it's cold OR windy
bool coat_or = (temp < 45 || wind > 25);

// Need a coat only if it's cold AND windy
bool coat_and = (temp < 45 && wind > 25);
```

A single change from `||` to `&&` completely changes the logic. This is why testing conditions carefully — including edge cases — is so important.

---

## Compound Assignment Operators

Compound assignment operators provide a shorthand for "perform an operation, then assign the result back to the left-hand side." The syntax is the operator immediately followed by `=`:

| Operator | Equivalent to |
|----------|---------------|
| `a += b` | `a = a + b` |
| `a -= b` | `a = a - b` |
| `a *= b` | `a = a * b` |
| `a /= b` | `a = a / b` |
| `a %= b` | `a = a % b` |

There are also bitwise compound operators (`&=`, `|=`, `^=`, `<<=`, `>>=`), which you will encounter later.

### Examples

```cpp
int a = 10;

a += 1;        // a = a + 1       → 11
a /= 5;        // a = a / 5       → 2
a *= b + c;    // a = a * (b + c) — the whole RHS is evaluated first
```

The last example is important. The right-hand side is always evaluated **in full** before the operation is applied. Think of the right-hand side as if it were wrapped in parentheses:

```cpp
a *= b + c;    // same as: a = a * (b + c)
```

Compound assignment is not just shorter — it also avoids repeating the left-hand side, which can matter when the left-hand side is a more complex expression. But its main benefit is readability.

---

## Operator Precedence and Associativity

### Precedence

When an expression contains multiple operators, **precedence** determines how the expression is grouped — i.e., which operator binds more tightly. Precedence by itself does not specify the runtime order in which operands with side effects are evaluated. Higher rows in the table below bind more tightly:

| Precedence | Operator | Associativity |
|------------|----------|---------------|
| Highest | `()` | Left to right |
| | `++` `--` (postfix) | Left to right |
| | `!` `++` `--` (prefix), unary `-` | Right to left |
| | `*` `/` `%` | Left to right |
| | `+` `-` | Left to right |
| | `<` `<=` `>` `>=` | Left to right |
| | `==` `!=` | Left to right |
| | `&&` | Left to right |
| | `\|\|` | Left to right |
| | `?:` | Right to left |
| Lowest | `=` `+=` `-=` `*=` `/=` `%=` | Right to left |

This table is a subset; complete tables are available online and in C++ reference books.

### Associativity

When two adjacent operators have the **same** precedence, **associativity** decides the grouping:

- **Left to right**: the leftmost operator is applied first.
- **Right to left**: the rightmost operator is applied first.

For example, `+` and `-` have the same precedence and associate left to right:

```cpp
a - b + c    // parsed as (a - b) + c
```

Assignment associates right to left, which is why `num1 = num2 = 1000` works: it is parsed as `num1 = (num2 = 1000)`.

### Using Parentheses

You can always use parentheses to make the order explicit, regardless of precedence rules. This is good practice for complex expressions — it makes your intent clear to both the compiler and anyone reading your code.

```cpp
// Without parentheses — relies on precedence
int result = a + b * c;

// With parentheses — explicit and unambiguous
int result = a + (b * c);
```

### A Worked Example

Consider:

```cpp
result = a + b * c;
```

Multiplication has higher precedence than addition, which has higher precedence than assignment. So the expression is grouped as:

1. `b * c`
2. `a + (b * c)`
3. `result = (a + (b * c))`

With parentheses:

```cpp
result = a + (b * c);   // same thing, but clearer
```

Now consider:

```cpp
result = a + b - c;
```

`+` and `-` have equal precedence and associate left to right, so:

1. `a + b`
2. `(a + b) - c`
3. `result = ((a + b) - c)`

Again, parentheses can make this explicit if the expression is at all complicated.

---

## Summary

This tutorial covered the foundational elements of C++ expressions, statements, and operators:

- **Expressions** are sequences of operators and operands that produce values. **Statements** are complete units of action, usually terminated by semicolons. Expressions are the building blocks of statements.
- **Operators** come in unary, binary, and ternary forms, and fall into categories: assignment, arithmetic, increment/decrement, equality, relational, logical, and compound assignment.
- The **assignment operator** `=` stores a value into an lvalue. It is not a test for equality. Understand lvalues (locations) vs. rvalues (contents), and the difference between initialization and assignment.
- **Arithmetic operators** behave as expected, but **integer division** truncates the fractional part. Use floating-point types or `static_cast` when you need a fractional result.
- **Increment and decrement** have prefix and postfix forms whose behavior differs inside larger expressions. Never modify the same variable more than once in one full expression without sequencing — `i++ + ++i` is undefined behavior.
- **Mixed-type expressions** trigger automatic promotion or demotion. Use `static_cast` to control conversions explicitly.
- **Equality** (`==`, `!=`) and **relational** (`<`, `>`, `<=`, `>=`) operators produce booleans. Use `boolalpha` to print `true`/`false`, and be cautious when comparing floating-point values.
- **Logical operators** (`!`, `&&`, `||`) combine boolean conditions. Understand short-circuit evaluation, and watch for the common error of using `&&` where `||` is needed (or vice versa).
- **Compound assignment** (`+=`, `-=`, etc.) combines an operation with assignment. The entire right-hand side is evaluated first.
- **Precedence and associativity** determine the order in which operators are applied. When in doubt, use parentheses.

These rules may seem like a lot to remember at first, but they quickly become second nature. The key habits to build are: know the difference between `=` and `==`; watch for integer division; be careful with increment/decrement inside expressions; and test edge cases whenever you write a condition. With these fundamentals in place, you are ready to move on to control flow statements — `if`, `switch`, loops — where expressions and operators do their real work.
