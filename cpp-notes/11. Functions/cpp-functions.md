# C++ Functions

## Table of Contents

1. [Why Functions Exist](#1-why-functions-exist)
2. [Using Built-in Functions (The Standard Library)](#2-using-built-in-functions-the-standard-library)
3. [Defining Your Own Functions](#3-defining-your-own-functions)
4. [Function Prototypes (Forward Declarations)](#4-function-prototypes-forward-declarations)
5. [Parameters, Arguments, and the Return Statement](#5-parameters-arguments-and-the-return-statement)
6. [Pass by Value: The Default](#6-pass-by-value-the-default)
7. [Default Argument Values](#7-default-argument-values)
8. [Overloading Functions](#8-overloading-functions)
9. [Passing Arrays to Functions](#9-passing-arrays-to-functions)
10. [Pass by Reference](#10-pass-by-reference)
11. [Scope Rules](#11-scope-rules)
12. [How Function Calls Actually Work (The Call Stack)](#12-how-function-calls-actually-work-the-call-stack)
13. [Inline Functions](#13-inline-functions)
14. [Recursive Functions](#14-recursive-functions)
15. [Quick Reference / Cheat Sheet](#15-quick-reference--cheat-sheet)

---

## 1. Why Functions Exist

When you first learn to program, everything lives inside `main()`. One statement after another, top to bottom. For a 30-line program, this works fine. For a 3,000-line program, it falls apart:

- The code becomes hard to read and navigate.
- The same logic gets copy-pasted in many places.
- Fixing a bug means hunting down every copy.
- Adding a feature becomes risky, because a change in one place may silently break another.

### The Solution: Abstraction

**Abstraction** means hiding implementation details behind a clear, simple interface. Instead of reading ten low-level statements, you read *"read input, process input, provide output."* That is exactly what functions give you.

A function is a **named, self-contained, reusable unit of code**. It packages a task behind a name, and you "call" that name whenever you need the task performed.

### Modularization in Practice

Without functions, `main` looks like this:

```cpp
int main() {
    // 10 statements to read input ...
    // 10 statements to process ...
    // 10 statements to output ...
}
```

With functions:

```cpp
int main() {
    read_input();
    process_input();
    provide_output();
}
```

The work is identical, but now:

- `main` reads like a **summary** of what the program does.
- Each function has a **single responsibility**.
- Each function can be **tested, debugged, and reused** independently.

### The Boss/Worker Analogy

Think of `main()` as a **boss** and each function as a **worker**.

- The boss does not perform subtasks personally; the boss **coordinates** them.
- Each worker knows exactly how to do their own job.
- The boss must know *what* each worker needs (inputs) and *what* they hand back (outputs).
- The boss does **not** need to know *how* the worker does it internally.

That last point has a name: **information hiding**. When you call `std::sqrt(400.0)`, you don't know (or care) what algorithm computes square roots. You only need to know that it takes a number, returns the square root, and is named `sqrt`. This is what makes large programs tractable: you can reason about a program in terms of *what* each piece does, without constantly worrying about *how* it does it.

**In short:** functions turn a long, fragile sequence of statements into a set of named, independent building blocks. Everything else in this document builds on that idea.

---

## 2. Using Built-in Functions (The Standard Library)

C++ ships with extensive libraries of **pre-written, pre-tested** functions and classes. You should use them instead of "reinventing the wheel." Library functions have been reviewed by many experts, optimized for performance, and tested on many platforms — your hand-written version is unlikely to be better, and writing it costs time and introduces bugs.

### Common Libraries

| Header | Purpose | Examples |
|--------|---------|----------|
| `<iostream>` | Console I/O | `std::cout`, `std::cin` |
| `<string>` | C++ string class | `std::string`, `getline` |
| `<cmath>` | Math functions | `std::sqrt`, `std::pow`, `std::sin`, `std::floor` |
| `<cstdlib>` | General utilities | `std::rand`, `std::srand` |
| `<ctime>` | Time / date | `std::time` |

### Example: Math Functions

```cpp
#include <cmath>
#include <iostream>

int main() {
    double num = 125.6;
    std::cout << std::sqrt(num)  << '\n';     // 11.2071
    std::cout << std::cbrt(num)  << '\n';     // ~5.008
    std::cout << std::sin(num)   << '\n';
    std::cout << std::ceil(num)  << '\n';     // 126
    std::cout << std::floor(num) << '\n';     // 125
    std::cout << std::round(num) << '\n';     // 126
    std::cout << std::pow(num, 4.2) << '\n';  // 6.54269e+08
}
```

**Key idea:** you use these functions without knowing how they are implemented. That is the point of abstraction: you only need to know the **name**, the **arguments the function expects**, and the **value it returns**.

### Example: Random Numbers with `rand` / `srand`

```cpp
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
    std::srand(std::time(nullptr));  // seed the generator
    const int lower = 1, upper = 6;  // inclusive range

    for (int i = 0; i < 10; ++i) {
        int roll = std::rand() % (upper - lower + 1) + lower;
        std::cout << roll << ' ';
    }
    std::cout << '\n';
}
```

Why does this work?

- `std::rand()` returns a pseudo-random integer in `[0, RAND_MAX]`.
- `std::rand() % (max - min + 1)` produces values in `[0, max - min]`.
- Adding `min` shifts that range to `[min, max]`, which is `[1, 6]` here.

**Why seed at all?** A pseudo-random generator is a *deterministic sequence*: the same seed always produces the same sequence. Calling `std::srand(std::time(nullptr))` uses the current time as the seed, so each run produces a different sequence. Without a seed, every run would produce identical "random" numbers — which is rarely what you want.

> **A note on quality:** the `% max` trick is simple but slightly biased (some values occur a little more often than others). For serious statistical work, use the modern `<random>` header instead. For simple games and exercises, `rand()` is fine.

---

## 3. Defining Your Own Functions

A function definition has **four parts**:

1. **Return type** — the type of value the function hands back to the caller.
2. **Name** — an identifier that describes what the function does.
3. **Parameter list** — the variables the function expects the caller to supply.
4. **Body** — the statements to execute, enclosed in `{ }`.

### Syntax

```cpp
return_type function_name(parameter_list) {
    // statements
    return value;   // required if return_type is not void
}
```

### Examples

**No parameters, returns nothing:**

```cpp
void say_hello() {
    std::cout << "Hello\n";
}
```

**One parameter, returns an `int`:**

```cpp
int double_it(int x) {
    return x * 2;
}
```

**One parameter, returns a `double`:**

```cpp
double area_of_circle(double radius) {
    const double pi = 3.14159;
    return pi * radius * radius;
}
```

### Naming Conventions

Function names follow the same rules as variable names (letters, digits, underscores; cannot start with a digit). More importantly, **they should be meaningful and describe the operation**. A function typically has a **verb or verb phrase** name: `calculate_area`, `print_report`, `is_valid`.

> **Anti-pattern:** `func1`, `func2` — the reader cannot tell what these do.
> **Better:** `read_input`, `process_input`, `provide_output`.

### Executing a Program with Multiple Functions

```cpp
#include <iostream>

void say_world() { std::cout << "World\n"; }

void say_hello() {
    std::cout << "Hello\n";
    say_world();  // a function can call another function
    std::cout << "Bye from say_hello\n";
}

int main() {
    say_hello();
    std::cout << "Bye from main\n";
}
```

Execution always begins at `main`. When `main` calls `say_hello`, control jumps to `say_hello`; when `say_hello` calls `say_world`, control jumps there; when a function finishes, control returns to the exact spot from which it was called. This back-and-forth is the essence of the **call stack** (Section 12).

### A Constraint You Will Hit Immediately

The compiler reads your file **top to bottom**. Before it can check a call to `say_hello`, it must have already seen the *declaration* of `say_hello`. If you write:

```cpp
int main() {
    say_hello();  // ERROR: 'say_hello' was not declared in this scope
}

void say_hello() { /* ... */ }
```

...the compiler stops. You have two possible fixes:

1. Define every function *before* it is used.
2. **Declare** the function first (a *prototype*) and define it later — see Section 4.

Option 2 is the standard approach. It also works when two functions call each other, which option 1 cannot handle.

---

## 4. Function Prototypes (Forward Declarations)

### Why They Exist

To verify a call like `add_numbers(100, 200)`, the compiler must know:

- how many arguments `add_numbers` expects,
- their types,
- their order,
- the function's return type.

If the compiler has not seen the function before the call, it **cannot type-check** the call. A **function prototype** tells the compiler: "this function will exist later with this exact signature." The compiler can then verify every call to it.

### Syntax

A prototype is just the function *header* followed by a semicolon — no body:

```cpp
int add_numbers(int, int);      // parameter names optional
int add_numbers(int a, int b);  // easier to read; best practice
```

Parameter names are **ignored by the compiler** — only the types matter for matching. But include them anyway: they document the intent for human readers.

### Typical Layout

In a small program, put prototypes near the top, after the `#include` lines:

```cpp
#include <iostream>

// ---- function prototypes ----
void  say_hello();
void  say_world();
int   add_numbers(int a, int b);
double area_of_circle(double radius);

// ---- function definitions ----
int main() {
    say_hello();
    std::cout << add_numbers(100, 200) << '\n';
    return 0;
}

void say_hello() { /* ... */ }
void say_world() { /* ... */ }
int  add_numbers(int a, int b) { return a + b; }
```

Now the order of *definitions* no longer matters, and **mutually recursive functions** become possible (each declares the other first).

### How Prototypes Catch Mistakes

Given `void say_hello();`, the compiler will reject all of these:

```cpp
say_hello();               // OK
say_hello(42);             // ERROR: too many arguments
std::cout << say_hello();  // ERROR: cannot use the "value" of a void function
```

Likewise, given `int add_numbers(int, int);`:

```cpp
add_numbers(1, 2);        // OK
add_numbers(1);           // ERROR: too few arguments
add_numbers(1, 2, 3);     // ERROR: too many arguments
add_numbers(1.0, 2.0);    // Legally compiles. The doubles are converted to int
                          // by truncation toward zero (1 and 2). If the
                          // truncated value cannot be represented in int,
                          // the behavior is undefined. Compilers usually warn
                          // only when conversion warnings are enabled
                          // (e.g. -Wconversion).
```

> **Takeaway:** prototypes let the compiler catch mistakes *before* the program runs. Let the compiler help you every chance it gets.

### Where Prototypes Live in Larger Programs

In small single-file programs, prototypes sit at the top of the `.cpp` file. In larger programs, prototypes move into **header files** (`.h`), which are `#include`d by every source file that needs them. This is the same mechanism by which `<iostream>` makes `std::cout` available.

---

## 5. Parameters, Arguments, and the Return Statement

### Vocabulary: Formal vs. Actual Parameters

| Term | Where it appears | Example |
|------|------------------|---------|
| **Formal parameter** | In the function definition | `int add(int a, int b)` — `a` and `b` are formals |
| **Actual parameter** (a.k.a. **argument**) | In the function call | `add(100, 200)` — `100` and `200` are actuals |

The compiler checks that the arguments can be matched to the formal parameters **in order**, and that each argument can be converted to the corresponding formal parameter type. Because of default arguments (Section 7), the number of arguments may be **fewer** than the number of parameters — but never more. If no valid match exists, you get a compile error. Allowed implicit conversions are applied automatically — see below.

### Type Conversion at the Call Site

C++ will implicitly convert an argument to the formal parameter's type when a suitable implicit conversion exists. This includes both standard conversions and user-defined conversions:

```cpp
greet("Frank");  // const char[6] → const char*  (array-to-pointer conversion)
                 // const char*  → std::string   (user-defined conversion)
```

```cpp
void truncate(int x);
truncate(3.9);   // passes 3; the fractional part is lost
```

Conversions that **lose information** (for example, `double` → `int`) are a different story. They are still legal in a function call — calling `truncate(3.9)` passes `3` — but they silently truncate. Compilers can warn about such conversions when conversion warnings are enabled (`-Wconversion`), and you should convert explicitly (`static_cast<int>(3.9)`) to make your intent clear.

### The Return Statement

If the declared return type is **not `void`**, the function **must** return a value of that type (or of a type convertible to it). The one exception is `main`: if control reaches the end of `main` without a `return`, it is equivalent to `return 0;`.

```cpp
int add_numbers(int a, int b) {
    return a + b;  // required
}
```

If the return type **is `void`**, the `return` statement is **optional**. A bare `return;` is allowed but rare:

```cpp
void say_hello() {
    std::cout << "Hello\n";
    // no return statement needed
}
```

Facts about `return`:

- `return` **immediately terminates** the function and returns control to the caller.
- The returned value becomes the **value of the call expression**: in `int r = add_numbers(100, 200);`, the call evaluates to `300`, which is assigned to `r`.
- Multiple `return` statements are legal, but many programmers prefer **one return per function** because it makes the flow of control easier to follow. Use your judgment: early returns are fine for error handling and for very small functions.

---

## 6. Pass by Value: The Default

When you pass a variable to a function, what actually reaches the function?

**By default, C++ passes arguments by value: the function receives a copy of the argument's value.**

### What "by Value" Means, Concretely

```cpp
void by_value(int num) {
    num = 1000;  // modifies the local copy, NOT the caller's variable
}

int main() {
    int num = 10;
    std::cout << num << '\n';  // 10
    by_value(num);
    std::cout << num << '\n';  // 10  ← unchanged
}
```

Step-by-step:

1. `main`'s `num` holds `10`.
2. Calling `by_value(num)` copies `10` into a **freshly allocated** local variable `num` inside `by_value`.
3. The assignment `num = 1000` writes to that *copy*, at a *different memory address*.
4. When `by_value` returns, the copy is destroyed; `main`'s `num` was never touched.

### Pros and Cons of Pass by Value

**Pro:** the function **cannot accidentally modify** the caller's variable. This makes reasoning easy: whatever the caller passes in stays as it was.

**Con:** copies can be **expensive** — especially for large objects like `std::vector` or `std::string`. Copying a million-element vector costs memory and time.

**Con:** sometimes you *want* the function to modify the caller's variable. Pass by value cannot do that. (See Section 10 on pass by reference.)

### Same Behavior for Objects

Pass by value is not special-cased for primitives. Objects are copied too:

```cpp
#include <string>

void by_value(std::string s) {
    s = "changed";  // modifies the local copy
}

int main() {
    std::string name = "Frank";
    by_value(name);
    std::cout << name << '\n';  // still "Frank"
}
```

```cpp
#include <string>
#include <vector>

void clear_copy(std::vector<std::string> v) {
    v.clear();  // empties the local copy
}

int main() {
    std::vector<std::string> stooges = {"Larry", "Moe", "Curly"};
    clear_copy(stooges);
    // stooges is still {"Larry", "Moe", "Curly"}
}
```

The rule is uniform: **the formal parameter is always a separate variable**, initialized from the argument.

---

## 7. Default Argument Values

### Motivation

Consider a function that computes a total cost:

```cpp
double calc_cost(double base_cost, double tax_rate);
```

If **98% of your customers** live in a 6% tax region, you will write `calc_cost(100.0, 0.06)` *thousands of times*. Why should you be forced to spell out `0.06` every time?

**Default arguments** let the compiler insert a value automatically when you omit the argument at the call site.

### Syntax

```cpp
double calc_cost(double base_cost,
                 double tax_rate = 0.06);  // default
```

Now:

```cpp
calc_cost(200.0);        // tax_rate is 0.06
calc_cost(200.0, 0.08);  // tax_rate is 0.08 (the default is overridden)
```

### Rules

1. **Defaults go at the tail end of the parameter list.** You cannot default a parameter and then require a later parameter without a default.

   ```cpp
   void f(int a = 1, int b);      // ERROR
   void f(int a = 1, int b = 2);  // OK
   void f(int a, int b = 2);      // OK
   ```

2. **Put each default in exactly one place — either the prototype or the definition, never both.** If the function has a separate prototype (the usual case), put the default there, because that is what every calling file sees. If there is no separate prototype (the function is defined where it is declared), the default goes in the definition.

   ```cpp
   // header
   double calc_cost(double base_cost, double tax_rate = 0.06);
   // .cpp file
   double calc_cost(double base_cost, double tax_rate) { /* ... */ }
   ```

3. **You can have multiple defaults.**

   ```cpp
   double calc_cost(double base_cost,
                    double tax_rate = 0.06,
                    double shipping = 3.50);
   ```

Now all of these are valid:

```cpp
calc_cost(100.0);              // 100 × 1.06 + 3.50
calc_cost(100.0, 0.08);        // 100 × 1.08 + 3.50
calc_cost(100.0, 0.08, 4.25);  // fully specified
```

### When Defaults Make Sense

Defaults are appropriate when an argument **usually** has one value but **occasionally** needs another. Typical examples:

- Tax rates in a known region.
- Default printer, paper size, or resolution for a print function.
- Default prefix (`"Mr."`) or suffix (empty) for a name formatter.

Defaults make your code **less verbose** and **less error-prone**: it's easy to mistype `0.06` as `0.006`, but impossible to mistype an argument you never write.

---

## 8. Overloading Functions

### Motivation

Suppose you want a `display` function that can print an `int`, a `double`, and a `std::string`. Without overloading you would need three different names:

```cpp
void display_int(int);
void display_double(double);
void display_string(std::string);
```

Not only is that ugly — it forces *you*, the caller, to remember which spelling goes with which type. **Overloading** lets you use **one name** for many versions of the same operation:

```cpp
void display(int);
void display(double);
void display(std::string);
void display(std::string, std::string);
void display(std::vector<std::string>);
```

Now you just write `display(x)` and the compiler picks the right version based on the argument types. This is an example of **polymorphism** — "many forms for the same concept."

### How the Compiler Chooses

Given an overloaded name, the compiler **matches the arguments of the call against each candidate's parameter list**:

```cpp
display(100);            // calls display(int)
display(123.5);          // calls display(double)
display("hi");           // "hi" is const char* → converted to std::string
display("hi", "there");  // calls display(std::string, std::string)
```

If **exactly one** candidate matches (after allowed conversions), that one is used. If **none** match, you get a compiler error. If **multiple** match equally well, you get an **ambiguity error** (see below).

### Key Facts

1. **Every overloaded version must be implemented.** A prototype without a body produces a linker error if that version is used.

2. **The return type is NOT part of the signature for overload resolution.** This is illegal:

   ```cpp
   int    get_value();
   double get_value();  // ERROR: cannot overload on return type alone
   ```

   *Why?* Consider `std::cout << get_value();` — the compiler cannot know which version you meant, because both would make sense in that expression. Rather than guess, C++ forbids the situation. Overload resolution must be decidable from the **arguments alone**.

3. **Parameter lists must differ in type, count, or order.** These are all fine:

   ```cpp
   void print(int);
   void print(int, int);
   void print(double, int);
   void print(int, double);
   ```

A top-level `const` on a parameter does **not** make a new overload: `void f(int)` and `void f(const int)` declare the same function. (Member functions can additionally be overloaded on `const`/reference qualifiers — a topic covered later.)

### Interaction with Default Arguments

Overloading and default arguments **can conflict dangerously**. Consider:

```cpp
void print(int x = 100);
void print(double x = 125.5);

int main() {
    print();  // AMBIGUOUS — both candidates match
}
```

Both `print(int)` and `print(double)` accept zero arguments because both have defaults. The compiler refuses to guess and issues an **ambiguous call error**. When you combine overloading with defaults, make sure no call can match more than one overload.

---

## 9. Passing Arrays to Functions

### The Critical Difference: Arrays Are Not Copied

Most types are passed by value: the function gets a copy. **Arrays are the exception: C++ cannot pass an array by value.** When a parameter is written as `T arr[]` or `T*`, the compiler *adjusts* it to `T*`, and what is actually passed is **a pointer to the first element** — not a copy of the array's contents. (This adjustment is often called array-to-pointer "decay.") The pointer itself is copied, but the elements it points to are not.

```cpp
void print_array(int arr[], int size);  // 'arr' is really an int*
```

Inside the function, `arr` behaves like a pointer to the first element; `arr[i]` accesses the element `i` positions further along. The `[]` notation in the parameter list is just a convenient spelling for `int*` — it does not mean the function receives a copy of the array.

### Consequence 1: You Must Pass the Size

Because the function receives only an address, it **has no way to know how many elements the array has**. So you must pass the size as a second parameter:

```cpp
void print_array(int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
}

int main() {
    int my_numbers[] = {1, 2, 3, 4, 5};
    print_array(my_numbers, 5);
}
```

> **Note:** A C-style string doesn't need an explicit size parameter, because it is terminated by the sentinel character `'\0'`. Ordinary arrays have no such sentinel; pass the size explicitly.

### Consequence 2: The Function Can Modify the Original Array

Because the function holds the *address* of the caller's array, any write through that address **changes the caller's array**:

```cpp
void zero_array(int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = 0;  // modifies the caller's array!
    }
}

int main() {
    int scores[] = {100, 98, 90, 86, 84};
    zero_array(scores, 5);
    // scores is now {0, 0, 0, 0, 0}
}
```

This is useful for *intentional* in-place updates. It is dangerous for functions that shouldn't modify the array — like a `print_array`. If a print function accidentally writes into the array, the caller's data is corrupted.

### Consequence 3: `const` Protects Against Accidental Modification

Mark the array parameter `const` to tell the compiler: *"this function must not modify the array."*

```cpp
void print_array(const int arr[], int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << ' ';
        // arr[i] = 0;  // ← COMPILE ERROR: assignment to read-only location
    }
}
```

The caller still passes an address (no copy), but the compiler now prevents writes. **Best practice:** every function that receives an array and does not intend to modify it should declare the parameter `const`.

### Arrays vs. Pass by Value

Arrays don't break the pass-by-value rule so much as sidestep it: the array itself is never copied. What is passed *by value* is **the pointer** (a single address like `0x7ffe...`). Copying a pointer is cheap; copying a whole array would be expensive. The function then *dereferences* that pointer to access elements. This is the conceptual bridge to the next section.

---

## 10. Pass by Reference

### Motivation

Pass by value gives the function a **copy**. Pass by array gives the function an **address** (and therefore write access), but it only works for arrays. We need a general mechanism that lets a function:

1. **Modify the caller's variable** (any type, not just arrays), *and/or*
2. **Avoid the cost of copying** a large object.

C++ provides this through **reference parameters**.

### Syntax and Semantics

A reference parameter is declared with `&`:

```cpp
void scale_number(int& num) {
    if (num > 100) num = 100;
}
```

The `int&` means "*a reference to an int*." When called, the formal parameter `num` becomes an **alias** for the actual argument — the same storage, a different name:

```cpp
int main() {
    int number = 1000;
    scale_number(number);
    std::cout << number << '\n';  // 100  ← modified!
}
```

Because `num` *is* `number` (same memory), assigning `num = 100` inside the function changes `number` in `main`.

### Classic Example: `swap`

```cpp
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 10, y = 20;
    std::cout << x << ' ' << y << '\n';  // 10 20
    swap(x, y);
    std::cout << x << ' ' << y << '\n';  // 20 10
}
```

If `swap` used pass-by-value, it would swap its *copies* and `main`'s `x` and `y` would remain unchanged.

### The Big Benefit: Avoiding Copies

Consider printing a `std::vector<std::string>`:

```cpp
void print(std::vector<std::string> v) { /* ... */ }  // copies the whole vector!
```

For a large vector this is costly. Using a reference avoids the copy:

```cpp
void print(std::vector<std::string>& v) { /* ... */ }  // no copy
```

### The Risk: Unwanted Modification

But now `print` *can* modify the vector — even call `v.clear()` and empty it! A print function should never do that. **Fix:** declare the reference `const`.

```cpp
void print(const std::vector<std::string>& v) { /* ... */ }
```

This is called **pass by const reference**, and it is the best of both worlds:

- **No copy** (efficient).
- **Read-only** (safe — the compiler rejects any attempt to modify `v`).

> **Rule of thumb:**
> - Small primitives (`int`, `double`, `char`): pass **by value**.
> - Large objects (`std::string`, `std::vector`, user classes): pass **by const reference** if you only need to read.
> - Anything you need to modify: pass **by non-const reference** (and document this clearly).

### Reference vs. Value: Same Syntax at the Call Site

Notice that the *call* syntax is identical whether the parameter is by value or by reference:

```cpp
scale_number(number);  // 'number' is passed either way
```

The difference is entirely in the **declaration**. This is convenient but also a subtle trap: a reader looking only at the call site cannot tell whether a function will modify the argument. **Design with clear parameter types and names, and document modifying functions.**

---

## 11. Scope Rules

### What Scope Means

**Scope** determines *when and where* a name (variable, function, parameter) can be used. C++ uses **static (lexical) scoping**: scope is determined by the **text of the program** — where a declaration appears in the source file — not by the runtime call stack.

### Local (Block) Scope

A variable declared inside a block `{ ... }` is visible only from its declaration **to the end of that block**:

```cpp
int main() {
    int x = 10;          // local to main
    {
        int y = 20;      // local to the inner block
        std::cout << x;  // OK — inner blocks see outer variables
        std::cout << y;  // OK
    }
    std::cout << y;      // ERROR — y is not visible outside its block
}
```

**Function parameters also have block scope**: they are visible throughout the function body but nowhere else.

### Lifetime: Block Scope ⇒ Function Activation

- When a function is **called**, it is "activated," and its local variables and parameters are bound to **fresh storage**.
- When the function **returns**, that storage is released (conceptually) and the names are no longer visible.
- Therefore **local variables do not retain their values between calls**.

### Shadowing

When an inner block declares a variable with the same name as an outer block's variable, the inner declaration **shadows** the outer one inside that inner block:

```cpp
int main() {
    int num = 100;
    {
        int num = 200;     // shadows the outer 'num'
        std::cout << num;  // 200
    }
    std::cout << num;      // 100 — the outer one again
}
```

Name resolution always starts at the **innermost scope** and works **outward**: local → enclosing blocks → global. It never looks *inward*.

### Global Scope

Variables declared **outside any function or class** have **global scope**: within the same source file, they are visible to every function from their declaration onward. To use a global variable from another source file, you need a matching `extern` declaration.

```cpp
int global_counter = 0;  // global

void increment() {
    ++global_counter;
}
```

> **Best practice:** *Global constants* (like `const double PI = 3.14159;`) are fine. Note that a namespace-scope `const` has **internal linkage** by default — each `.cpp` file that includes it gets its own copy. To share a single constant across files, either use `inline constexpr` (C++17):
>
> ```cpp
> // header
> inline constexpr double PI = 3.14159;
> ```
>
> or declare it `extern` in the header and provide its definition in exactly one `.cpp`
> (writing `extern` on the definition is optional, but makes the intent clear):
>
> ```cpp
> // header
> extern const double PI;
> // one .cpp
> extern const double PI = 3.14159;
> ```
>
> In this pattern, the `extern` declaration in the header gives `PI` external linkage.
> A plain namespace-scope `const double PI = ...;` would have internal linkage only
> if there were no prior `extern` declaration making it external.

### Static Local Variables

A **static local variable** is declared inside a function with the `static` keyword. It has:

- **Local scope** — visible only within that function body.
- **Program lifetime** — initialized once (the first time the function runs) and **retained across subsequent calls**.

```cpp
void counter() {
    static int count = 0;  // initialized only once
    ++count;
    std::cout << count << '\n';
}

int main() {
    counter();  // 1
    counter();  // 2
    counter();  // 3
}
```

Compare with a non-static local variable, which would print `1` every time.

If no initializer is given, a `static` local is **zero-initialized** (unlike ordinary locals, which start with indeterminate values).

Static locals are useful when you need a function to "remember" something between calls **without exposing that state to the rest of the program** (which a global variable would).

### Summary of Scope Categories

| Kind | Declared where | Visible where | Lifetime |
|------|----------------|---------------|----------|
| **Local** | Inside `{ }` | That block only | While block executes |
| **Parameter** | Function header | Function body only | While function executes |
| **Static local** | Inside function with `static` | Function body only | Entire program |
| **Global** | Outside any function | Entire program after declaration | Entire program |

---

## 12. How Function Calls Actually Work (The Call Stack)

### Memory Layout

A running C++ program's memory is divided into segments:

```
┌──────────────────┐
│   Code area      │   compiled instructions
├──────────────────┤
│   Static/Global  │   static & global variables
├──────────────────┤
│   Stack          │   function call frames  ← focus of this section
├──────────────────┤
│   Heap           │   dynamic memory (covered later in the course)
└──────────────────┘
```

### The Stack: LIFO

The **function call stack** (or simply **call stack**) works like a stack of books: last in, first out (**LIFO**). To remove the second book you must remove the first. The two operations are:

- **push** — put an item on top.
- **pop** — remove the top item.

LIFO is not arbitrary: it matches the way function calls nest. A function that is called last must finish first, because its caller is waiting for its result.

### Activation Records (Stack Frames)

Each time a function is called, a **stack frame** (also called an **activation record**) is **pushed** onto the stack. It contains:

- The function's parameters.
- Its local variables.
- The **return address** — where to resume execution when the function finishes.
- Saved register values and other bookkeeping.

When the function returns, its frame is **popped**, and control resumes in the frame now on top — the caller.

**Crucial properties:**

- You cannot jump into the *middle* of the stack; strict LIFO must be obeyed. In normal control flow, `return` is the standard way out of a function. (Exceptions, `std::exit`, and similar mechanisms are separate topics.)
- The stack has **finite size**. Activating too many functions (e.g., through infinite recursion) produces a **stack overflow error**, which is usually fatal.

### A Worked Example

Consider:

```cpp
int function2(int& x, int y, int z) {
    return x = x + y + z;
}

int function1(int a, int b) {
    int result = a + b;
    function2(result, a, b);
    return result;
}

int main() {
    int x = 10, y = 20;
    int z = function1(x, y);
    std::cout << z << '\n';
}
```

**Tracing the stack:**

1. `main` is activated. Space for `x = 10`, `y = 20`, `z` is allocated.
2. `main` calls `function1(x, y)`. `function1`'s frame is pushed: `a` gets **a copy** of `x` (`10`), `b` gets **a copy** of `y` (`20`).
3. `function1` computes `result = a + b = 30`.
4. `function1` calls `function2(result, a, b)`. `function2`'s frame is pushed:
   - `x` is a **reference** to `result` (an alias — no copy).
   - `y` is a **copy** of `a` (`10`).
   - `z` is a **copy** of `b` (`20`).
5. `function2` computes `x = x + y + z = 30 + 10 + 20 = 60`. Because `x` is an alias for `result`, **`result` becomes `60`**.
6. `function2` returns; its frame is popped.
7. `function1` returns `result` (`60`); its frame is popped.
8. `main` assigns `60` to `z`, prints `60`, and exits.

**What this illustrates:**

- **Pass by value** creates a copy at call time (`a ← x`, `b ← y`).
- **Pass by reference** creates an alias — no copy, and writes go straight to the caller's variable (`x ≡ result`).
- **The stack enforces the order** in which frames are pushed and popped. The frame of `function1` cannot be popped until `function2`'s frame (pushed later) has been popped.

### Function Call Overhead

Everything described above — pushing parameters, saving registers, saving the return address, jumping to the function, restoring registers, jumping back, popping the frame — is **function call overhead**. For most functions this overhead is negligible compared to the work the function does. But for very small functions called in tight loops, it can matter. This motivates the next section.

---

## 13. Inline Functions

### Motivation

For a *tiny* function (e.g., `int square(int x) { return x * x; }`), the call overhead can dominate the actual computation. **Inline expansion** replaces the function call with the function's **body** at the call site:

```cpp
// Conceptually:
int y = square(x);
// becomes:
int y = x * x;
```

This eliminates the call overhead entirely.

### Syntax

```cpp
inline int square(int x) {
    return x * x;
}
```

The `inline` keyword does two separate things:

1. **Optimization hint (non-binding).** It suggests that the function is a good candidate for inline expansion. This is only a hint: the standard does not guarantee that the function will actually be inlined, and compilers may also inline functions that are not marked `inline` if they judge it profitable.
2. **Linkage / ODR rule (binding).** It allows the same function to be defined in multiple translation units without violating the one-definition rule (ODR). This is what makes it legal to put the function's definition in a header that many `.cpp` files include.

Most modern compilers make their own inlining decisions; the linkage rule is the part you must actually get right.

### Trade-off

- **Upside:** no call overhead → faster execution for small, hot functions.
- **Downside:** the function's code is **duplicated at every call site**. If you inline a function called 500 times, the resulting binary may be noticeably larger (this is called **code bloat**).

### Practical Advice

- Do not micro-optimize with `inline` at the start; write clear code.
- Modern compilers are excellent at deciding when to inline. Most short functions you write will be inlined automatically if profitable.
- Inline functions are usually **defined in header files**. Because a header is included by many source files, the function ends up defined in several translation units; `inline` is what makes that legal. Defining a *non-`inline`* function in a header typically produces multiple-definition linker errors.

---

## 14. Recursive Functions

### Definition

A **recursive function** is a function that **calls itself** — either directly, or indirectly through other functions. If two or more activation records for the same function are on the stack at once, recursion is in progress.

### Why Recursion Matters

Certain problems are naturally defined **in terms of themselves**. For those problems, recursion produces code that mirrors the mathematical definition and is much easier to read and verify than an equivalent loop.

**Classic examples:**

| Domain | Recursive problems |
|--------|-------------------|
| Mathematics | Factorial, Fibonacci, fractals |
| Searching / sorting | Binary search, tree traversal, depth-first search |
| Puzzles | Towers of Hanoi, permutations |

### Anatomy of a Recursive Function

Every correct recursive function has two parts:

1. **Base case(s)** — conditions under which the function returns **without** recursing. This is what **stops** the recursion.
2. **Recursive case(s)** — the part that calls the function again on a **smaller** subproblem.

If the base case is missing or wrong, the function recurses forever, exhausts the stack, and crashes with a **stack overflow**.

### Example 1: Factorial

Mathematical definition:

```
n! = 1                if n = 0     (base case)
n! = n × (n-1)!       if n > 0     (recursive case)
```

C++ translation:

```cpp
unsigned long long factorial(unsigned long long n) {
    if (n == 0) {
        return 1;  // base case
    }
    return n * factorial(n - 1);  // recursive case
}
```

Called from `main`:

```cpp
int main() {
    std::cout << factorial(8) << '\n';  // 40320
}
```

Notice how closely the code mirrors the mathematical definition. That is the main advantage of recursion — **direct correspondence between the problem statement and the code**.

### Tracing `factorial(3)` on the Stack

Let's walk through the stack by hand. Factorial's only local variable is `n`.

1. `main` calls `factorial(3)`. Frame 1 is pushed: `n = 3`. Since `n ≠ 0`, we need `3 * factorial(2)`.
2. `factorial(2)` is called. Frame 2 is pushed: `n = 2`. We need `2 * factorial(1)`.
3. `factorial(1)` is called. Frame 3 is pushed: `n = 1`. We need `1 * factorial(0)`.
4. `factorial(0)` is called. Frame 4 is pushed: `n = 0`. **Base case!** Return `1`.
5. Frame 4 is popped. Frame 3 computes `1 * 1 = 1`, returns `1`.
6. Frame 3 is popped. Frame 2 computes `2 * 1 = 2`, returns `2`.
7. Frame 2 is popped. Frame 1 computes `3 * 2 = 6`, returns `6`.
8. Frame 1 is popped. `main` receives `6`.

The push phase goes down until the base case; the **unwinding** phase goes back up, multiplying as it returns.

### Example 2: Fibonacci

Mathematical definition:

```
fib(0) = 0
fib(1) = 1
fib(n) = fib(n-1) + fib(n-2)      for n > 1
```

C++:

```cpp
unsigned long long fibonacci(unsigned long long n) {
    if (n <= 1) {
        return n;  // base cases: 0 → 0, 1 → 1
    }
    return fibonacci(n - 1) + fibonacci(n - 2);  // recursive case
}
```

The `n <= 1` check handles both base cases in one test.

### Why `fibonacci(40)` Is Slow

`fibonacci(n)` makes **two** recursive calls, each of which makes two more, and so on. The number of calls grows exponentially — roughly with the golden ratio `φ ≈ 1.618` raised to the power `n`. Calculating `fibonacci(40)` requires on the order of **300 million** function calls, which takes noticeable wall-clock time even on a fast machine.

This is the classic warning: **recursion can be expensive** because every call pushes a new frame, and poorly-structured recursive functions re-solve the same subproblems over and over. (A technique called *memoization* — caching previously computed results — fixes this, and is covered later in the course.)

### Practical Advice

- **Recursion is a form of iteration.** Anything you can write recursively, you can write iteratively, and vice versa.
- **Use recursion only when the problem naturally lends itself to a recursive formulation** (tree traversal, divide-and-conquer, the Towers of Hanoi). Do not use recursion to count from 1 to 10.
- **Always write the base case first** and convince yourself it terminates. It is the single most important part of a recursive function.
- **Watch stack depth.** Deep recursion (thousands of nested calls) can overflow the stack. In that case, a **rewritten iterative solution** may be necessary even if it is less elegant.
- **Overflow is a real concern** for factorial and Fibonacci. `unsigned long long` is large, but even it overflows for `factorial(21)` and beyond.

---

## 15. Quick Reference / Cheat Sheet

### Function Declaration & Definition

```cpp
// Prototype (in the header or top of file)
int add(int a, int b);

// Definition
int add(int a, int b) {
    return a + b;
}
```

### Parameters

| Form | Syntax | Copy made? | Can modify caller's variable? |
|------|--------|-----------|-------------------------------|
| By value | `T x` | Yes | No |
| By reference | `T& x` | No (alias) | Yes |
| By const reference | `const T& x` | No (alias) | No (compile error) |
| Array | `T arr[]` (plus size) | Address only | Yes (use `const` to prevent) |

### Default Arguments

```cpp
void greet(std::string name,
           std::string prefix = "Mr.",
           std::string suffix = "");
```

Rules:
- Defaults go at the tail end.
- Put each default in exactly one place: usually the prototype, never repeated in the definition. If there is no separate prototype, put it in the definition.
- Once one parameter has a default, all later ones must too.

### Overloading

```cpp
void display(int);
void display(double);
void display(std::string);
```

Rules:
- Same name, different **parameter lists**.
- **Return type is not part of the signature.**
- Be careful combining overloading with default arguments — ambiguous calls will not compile.

### Scope

| Kind | Lifetime | Visibility |
|------|----------|-----------|
| Local | Block execution | Block |
| Parameter | Function execution | Function body |
| Static local | Program | Function body only |
| Global | Program | Program (from declaration onward) |

Name resolution: **look local first, then outward**, never inward.

### The Call Stack

- Every call pushes a **frame** containing parameters, locals, and the return address.
- Every return pops that frame.
- LIFO ordering is strict.
- The stack is **finite**; deep recursion can overflow it.

### Recursion

```cpp
RetType f(args) {
    if (base_case) return base_value;  // must terminate
    return combine(f(smaller_args));   // must shrink toward base
}
```

Always write the base case first.
