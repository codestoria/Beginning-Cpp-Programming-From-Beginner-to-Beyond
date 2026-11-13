# C++ Variables, Constants, and Primitive Data Types

## Table of Contents

1. [Introduction: Why We Need Variables](#1-introduction-why-we-need-variables)
2. [What a Variable Really Is](#2-what-a-variable-really-is)
3. [Declaring and Initializing Variables](#3-declaring-and-initializing-variables)
4. [Naming Variables](#4-naming-variables)
5. [Scope: Local vs. Global Variables](#5-scope-local-vs-global-variables)
6. [Primitive Data Types](#6-primitive-data-types)
7. [Measuring Types: `sizeof`, `<climits>`, and `<cfloat>`](#7-measuring-types-sizeof-climits-and-cfloat)
8. [Constants](#8-constants)
9. [Putting It All Together: A Complete Example](#9-putting-it-all-together-a-complete-example)

---

## 1. Introduction: Why We Need Variables

You cannot write a useful C++ program without variables. Before your program can calculate, read input, or display results, it needs a place to **store** the data it is working with. Variables and constants are the fundamental building blocks for that storage.

This guide answers three questions:

- **What** are variables and constants — both conceptually and in code?
- **Why** do they exist, and what problems do they solve?
- **How** do we use them correctly so our programs are readable, correct, and easy to change?

We will start with the machine's point of view, build up to practical code, and finish with a complete worked example. Each section explains not just *what* to do, but *why* it matters.

---

## 2. What a Variable Really Is

### 2.1 The machine's view: memory and addresses

A typical computer has three essential parts for our purposes:

- a **CPU**, which executes instructions,
- **memory (RAM)**, which stores instructions and data,
- a **bus**, which moves data between the CPU and memory.

RAM is a contiguous block of storage organized as **memory cells**. Each cell has a unique **address** — a location number. If we had to program using raw addresses, we would write something like:

```text
move 21 to memory location 1002
```

That is exactly what very low-level languages sometimes do. It is error-prone, hard to read, and impossible to maintain. Worse, the operating system may place your program at a different address each time it runs, so hard-coded addresses are not even reliable.

### 2.2 The programmer's view: names and bindings

Most programming languages let you associate a **name** with a memory location. This association is called a **binding**. Instead of `move 21 to memory location 1002`, we write:

```cpp
age = 21;
```

Now the compiler — the program that translates your C++ source code into machine code — handles the address for us. If the program runs again and `age` lands at a different address, our code still works, because we never depended on the address in the first place.

### 2.3 The definition

> **A variable is an abstraction for a memory location.**

We use abstractions throughout programming to manage complexity. A meaningful name like `age` is far easier to read and reason about than `1002`. And because `age` is a *variable*, its contents can *vary*: when you turn 22, you can change it from `21` to `22`.

### 2.4 The two essential properties of a variable

Every variable has:

1. **A type** — what kinds of values are legal to store in it.
2. **A name** — the identifier you use to refer to it.

The type matters because the compiler must know how many bytes to reserve and how to interpret the bits. Consider:

```cpp
age = 21;   // ERROR: the compiler does not know what 'age' is
```

This fails because `age` was never *declared*. The compiler has no idea what type `age` should be, so it cannot check whether `21` is legal. Compare with:

```cpp
int age;    // declare age as an integer
age = 21;   // now legal
```

C++ uses **static typing**: type rules are enforced at **compile time**, not at run time. This is a feature, not a limitation — many errors are caught before the program ever runs.

> **Rule:** In C++, a variable must be **declared before it is used**. Its value may change as the program executes.

---

## 3. Declaring and Initializing Variables

### 3.1 Declaration syntax

The syntax for declaring a variable is simple:

```cpp
type name;
```

Three parts:

1. the **type** (what kind of data),
2. the **name** (your identifier),
3. a **semicolon** to terminate the statement.

Examples:

```cpp
#include <string>     // required for std::string

int         age;      // a whole number (declared but NOT initialized — see 3.2)
double      price;    // a floating-point number
std::string name;     // a sequence of characters
```

Notice the last one: `std::string` is **not** a built-in type. One of C++'s most powerful features is **object-oriented programming**, which lets you create your own types. You declare variables of your own types using exactly the same syntax:

```cpp
Account my_account;   // a variable of a user-defined type
Person  the_author;   // another user-defined type
```

This consistency is a great strength of C++: once you understand `int`, you understand the shape of every declaration.

### 3.2 The danger of uninitialized variables

Consider:

```cpp
int age;
std::cout << age << std::endl;
```

What will this print? **Nobody knows.** The variable was *declared* but never *initialized*. Reading an uninitialized variable is **undefined behavior (UB)**: the C++ standard imposes no requirements on what happens. In practice the memory contains whatever bits happened to be there — leftover data from another program or arbitrary garbage — so you might see an arbitrary number such as `-858993460` or `32767`, or seemingly nothing at all.

> **Uninitialized variables are one of the most common sources of programming bugs in C++.**

The good news: most compilers will warn you. A message such as *"warning: 'age' is used uninitialized in this function"* is a strong hint that something is wrong. **Do not ignore compiler warnings.** They are the compiler helping you find bugs before they happen.

### 3.3 Three ways to initialize

C++ provides three common syntactic forms for giving a variable its initial value. All three are valid; they reflect the language's evolution over time.

```cpp
int age = 21;      // 1. copy initialization
int age(21);       // 2. direct initialization
int age{21};       // 3. list initialization (C++11 and later) — RECOMMENDED
```

**1. Copy initialization** uses `=`. Despite the `=`, this is *initialization*, not assignment: the variable is being created here, and `21` is copied into it.

**2. Direct initialization** uses parentheses and initializes the variable directly. For class types this is where constructors come in; we will revisit it when we study object-oriented programming.

**3. List (braced) initialization** uses curly braces. This is the style you should prefer, for two reasons:

- It is **consistent** across all types in C++.
- It performs **narrowing checks**. If the value cannot safely fit into the variable, the compiler raises an error instead of silently corrupting your data. We will see a concrete example in [Section 6.3](#63-integer-types).

> **Recommendation:** Use list initialization (`int age{21};`) by default.

### 3.4 Initialize as close to first use as possible

A useful habit: declare each variable **just before** you need it, not all at the top of the function.

```cpp
// Less clear: everything declared up front
int room_width{0};
int room_length{0};
std::cout << "Enter the width: ";
std::cin >> room_width;
std::cout << "Enter the length: ";
std::cin >> room_length;

// More clear: declare where used
std::cout << "Enter the width: ";
int room_width{0};
std::cin >> room_width;
std::cout << "Enter the length: ";
int room_length{0};
std::cin >> room_length;
```

The second version keeps each declaration next to the logic that uses it, making the code easier to follow. It also reduces the chance that a variable is left uninitialized or accidentally reused.

---

## 4. Naming Variables

### 4.1 The rules (enforced by the compiler)

C++ variable names are simple but strict:

| Rule | Example |
|------|---------|
| May contain letters, digits, and underscores | `room_width`, `x2`, `_temp` |
| **Must not** begin with a digit | `2fast` ❌, `fast2` ✔ |
| May begin with a letter or underscore, **but avoid leading underscores** | `value`, `x2` ✔; `_temp` ⚠️ (see note) |
| **Must not** be a C++ keyword | `int` ❌, `double` ❌ |
| **Must not** redeclare a name already declared in the same scope (see [Section 5](#5-scope-local-vs-global-variables)) | — |
| **Is case-sensitive** | `Age` and `age` are different names |
| **Must not** contain spaces or special characters | `my var` ❌, `a+b` ❌, `$x` ❌ |

> **Leading underscores are reserved in some contexts.** Names containing a double underscore (`__x`), or beginning with an underscore followed by an uppercase letter (`_X`), are reserved for the implementation everywhere. Names beginning with a single underscore are reserved at global scope. In practice: **do not start your own names with an underscore.**

Invalid examples and why:

```cpp
int int;      // ❌ 'int' is a reserved keyword
int 2fast;    // ❌ begins with a digit
int my var;   // ❌ contains a space
int a+b;      // ❌ special characters
int $price;   // ❌ special characters
```

### 4.2 A subtle trap: `cout` and the standard library

`cout` obeys the naming rules, so it looks like a legal name. However, it is **already taken by the standard library**. Strictly speaking the library defines `std::cout` inside namespace `std`, not a global `cout`, so declaring your own `cout` is legal — but it is a trap, because it collides as soon as the standard name is in scope:

The two snippets below are **independent examples**; each should be read on its own.

```cpp
// Example A: without `using namespace std;`
#include <iostream>

int cout;                    // legal: this is a global int, unrelated to std::cout

void greet() {
    cout << "hi";            // ❌ error: 'cout' is an int here,
                             //    and there is no operator<< for int and const char*
}
```

```cpp
// Example B: with `using namespace std;`
#include <iostream>
using namespace std;

int cout;                    // still legal: std::cout lives in namespace std

void greet() {
    cout << "hi";            // ❌ error: reference to 'cout' is ambiguous
    std::cout << "hi";       // ✔ qualified name still works
}
```

The rule: **do not reuse names that the standard library has already taken** — `cout`, `cin`, `endl`, `string`, `vector`, and many others are effectively off-limits once you include the corresponding header. Only *unqualified* uses become ambiguous; `std::cout` always works.

### 4.3 Style and best practices (not enforced, but important)

The compiler does not care about style. Your teammates — and future you — care very much.

- **Be consistent.** Pick one convention and stick to it everywhere.
- **Follow your employer's guide.** Google, for example, publishes a widely used C++ style guide. Many companies have their own.

The most common stylistic choice is how to separate words in a name:

| Style | Example | Notes |
|-------|---------|-------|
| **CamelCase** | `roomWidth` | Capitalize each word, no underscores |
| **snake_case** | `room_width` | Separate words with underscores |

Either is fine. **Consistency is what matters.** (The course this guide accompanies uses snake_case.)

### 4.4 Use meaningful names

Avoid obscure abbreviations. Compare:

```cpp
double moe;             // ❌ what is "moe"?
double mass_of_earth;   // ✔ clear
```

`mass_of_earth` requires slightly more typing, but it makes the program dramatically more readable and maintainable.

> **Never use a variable before initializing it.** Uninitialized variables produce unpredictable results. If it matters to you what value a variable holds, initialize it — even if just to `0`.

---

## 5. Scope: Local vs. Global Variables

### 5.1 Local variables

In all the examples so far, variables were declared inside the curly braces of `main`. These are **local variables**. Their **scope** — the region of the program where the name is visible — is limited to the statements *after* their declaration, within that block.

```cpp
int main() {
    int age{18};        // local to main
    std::cout << age;   // uses this age
    return 0;
}
```

This is generally what you want. A local variable becomes usable at its declaration and goes out of scope when execution leaves its block. (Storage may be allocated earlier, and for an initialized variable its lifetime begins once initialization is complete.) They cannot interfere with code elsewhere.

### 5.2 Global variables

Variables declared **outside any function** are **global**. They can be accessed from anywhere in the program.

```cpp
int age{18};            // global

int main() {
    std::cout << age;   // finds the global age
    return 0;
}
```

Two important facts about globals:

1. **Globals are automatically initialized to zero**, unlike locals.
2. **Globals can be read and modified from anywhere**, which is a double-edged sword.

### 5.3 Why globals are risky

Because any part of the program can change a global, a bug in one function can corrupt data used by another function thousands of lines away. Imagine a large program with a global `int balance`. One function accidentally writes to it, and a completely different function later reads the wrong value. Tracing that bug can take hours.

Globals have their place — for example, truly program-wide configuration — but they should be the exception, not the rule. You will see them often in introductory examples and legacy code; that does not make them a good habit.

### 5.4 Name shadowing

You can have both a global and a local with the same name. The local one **shadows** the global within its scope:

```cpp
int age{18};            // global

int main() {
    int age{16};        // local, shadows the global
    std::cout << age;   // prints 16, not 18
    return 0;
}
```

When the compiler sees `age`, it looks **locally first**. If it finds a declaration, it uses it. Only if there is no local declaration does it look globally. Shadowing is legal but often confusing; avoid it unless you have a good reason.

---

## 6. Primitive Data Types

C++ provides several **primitive** (also called **fundamental**) data types, implemented directly by the language. Unlike many higher-level languages, the **size and precision of C++ primitives depend on your platform and compiler**. As a C++ programmer, you need to know your machine.

### 6.1 Bits and the number of representable values

Computers store data in **binary** — sequences of `0`s and `1`s. Type sizes are measured in **bits**. For integer types, the number of distinct bit patterns is:

$$
\text{distinct bit patterns} = 2^{\text{number of bits}}
$$

(For signed integer types, some of those bit patterns represent negative values; for floating-point types the relationship between bits and representable values is more complex.)

| Bits | Distinct values |
|------|-----------------|
| 8    | 256             |
| 16   | 65,536          |
| 32   | ~4.3 billion    |
| 64   | ~18.4 quintillion |

The more bits, the more values — and the more memory required. Choosing a type is always a trade-off between range and memory.

### 6.2 Character type: `char`

The `char` type represents a single character, such as `'A'` or `'x'`. It is **always 1 byte** in C++ (`sizeof(char) == 1`). On modern systems a byte is usually 8 bits, giving 256 distinct values — enough for the Latin alphabet. The number of bits per byte is implementation-defined and available as `CHAR_BIT` (from `<climits>`), but is 8 on virtually all platforms you will use.

```cpp
char middle_initial{'J'};    // single quotes for a single character
std::cout << "My middle initial is " << middle_initial;
```

> **Note:** Single quotes `'J'` create a `char`. Double quotes `"J"` create a **string**, which is a different type. Mixing them up produces compiler errors.

Some languages (Chinese, Japanese, Korean, and many others) have thousands of characters and require multiple bytes per character. C++ supports wider character types for these, and **Unicode** is the common standard for representing them. In this course we use `char`, which handles Latin characters easily.

### 6.3 Integer types

Integers represent whole numbers. C++ offers many variants, both **signed** (can be negative) and **unsigned** (non-negative only).

| Type | Typical size | Typical range (signed) |
|------|--------------|------------------------|
| `short` | 2 bytes | -32,768 to 32,767 |
| `int` | 4 bytes | about ±2.1 billion |
| `long` | 4 or 8 bytes (platform-dependent) | varies |
| `long long` | 8 bytes | about ±9.2 quintillion |

Key rules:

- **The integer types `short`, `int`, `long`, and `long long` are signed by default.** Writing `int` is the same as writing `signed int`. The signedness of plain `char` is *implementation-defined*; `char`, `signed char`, and `unsigned char` are three distinct types.
- **Use `unsigned`** when you need only non-negative values.
- **`signed` is optional; `unsigned` is required** for unsigned types.

```cpp
int           exam_score{55};     // signed by default
unsigned int  count{10};          // explicitly unsigned
unsigned short rooms{5};          // 'short' implies 'short int'
long long     population{7600000000LL};
```

#### Which should I use?

It depends on the data:

- Ages: `unsigned int` or `int`.
- Distance between planets in kilometers: `unsigned long long`.
- **For most applications, `int` is perfectly acceptable.**

But always keep the **limits** in mind, especially when doing arithmetic. A result that does not fit in the destination variable can produce surprising results, depending on the types involved:

- **Unsigned integer** arithmetic wraps modulo \(2^N\) (well-defined).
- **Signed integer** arithmetic overflow is **undefined behavior**.
- **Converting** an out-of-range value to a signed type is **implementation-defined** (in practice you often get a truncated value, e.g. a negative number).

#### Overflow in action

```cpp
short value1{30000};
short value2{1000};
short product  = value1 * value2;   // compiles (possibly with a warning): the int result
                                    // is converted back to short; out-of-range conversion
                                    // is implementation-defined (commonly truncated)
short product2{value1 * value2};    // ❌ compile error: narrowing conversion
```

The multiplication itself is fine: `short` operands are **promoted to `int`**, and `30'000 × 1'000 = 30'000'000` fits comfortably in a 32-bit `int`. The problem is the **conversion back to `short`**: `30'000'000` does not fit in 16 bits, so the result is implementation-defined (in practice it is commonly truncated to a negative number). List initialization catches this at compile time.

List initialization catches **narrowing conversions** *at compile time*:

```cpp
// On a platform where long is 32 bits (e.g. Windows):
long people_on_earth{7'600'000'000};   // ❌ error: narrowing conversion

// On a platform where long is 64 bits (e.g. Linux/macOS), this compiles fine.
```

The compiler refuses **on a 32-bit `long` platform**, because converting 7.6 billion to a 32-bit `long` is a narrowing conversion. Use `long long`, which is guaranteed to be at least 64 bits everywhere:

```cpp
long long people_on_earth{7'600'000'000};   // ✔ fits on every platform
```

> **Digit separators:** C++14 lets you write `7'600'000'000` for readability. The compiler strips the apostrophes. If your compiler rejects this, you are using a pre-C++14 standard; either upgrade or remove the separators.

Contrast list initialization with copy initialization:

```cpp
// Again, on a platform where long is 32 bits:
long people_on_earth = 7'600'000'000;   // compiles; out-of-range conversion is
                                        // implementation-defined (commonly truncated)
```

Here the compiler **allows** the conversion. Converting a value that cannot be represented to a signed type is *implementation-defined*; in practice you often get a truncated, usually negative, number such as `-989934592`.

### 6.4 Floating-point types

Floating-point types represent **real numbers approximately** — values with fractional parts, and very large or very small magnitudes: `1.2`, `0.5`, `3.14159`.

Because computers have finite storage, and many real numbers (like π) have infinitely many digits, the stored value is an **approximation**. Internally, floats are stored like scientific notation: a **mantissa** and an **exponent** (e.g., $1.234 \times 10^7$). This is handled automatically; you just need to remember these values are approximate.

| Type | Typical size | Notes |
|------|--------------|-------|
| `float` | 4 bytes | ~7 significant digits |
| `double` | 8 bytes | ~15 significant digits |
| `long double` | 8, 12, 16, or more bytes (implementation-defined) | at least as precise as `double`; on MSVC it is usually the same as `double` |

```cpp
float  car_payment{401.23f};
double pi{3.14159265358979};
long double huge{2.7e120L};   // 2.7 × 10^120
```

If you store `401.23` into an `int`, the fractional part `.23` is truncated — a common source of error:

```cpp
int dollars = 401.23;   // compiles: value becomes 401 (fraction dropped)
int dollars2{401.23};   // ❌ compile error: narrowing conversion
```

Also note that converting a floating-point value that is out of range for the integer type is **undefined behavior**. Match the type to the data.

### 6.5 Boolean type: `bool`

The Boolean type represents **true** and **false**:

```cpp
bool game_over{false};
std::cout << game_over;                        // prints 0
std::cout << std::boolalpha << game_over;      // prints false
std::cout << std::noboolalpha << game_over;    // back to 0
```

Key facts:

- In C++, **`0` is false, and any non-zero value is true.**
- `true` and `false` are keywords.
- When printed with `cout`, a `bool` prints as `0` or `1`.

Booleans are the natural type for anything with two states: *game over*, *key pressed*, *is invincible*.

---

## 7. Measuring Types: `sizeof`, `<climits>`, and `<cfloat>`

### 7.1 The `sizeof` operator

C++ provides the `sizeof` operator to find **how many bytes** a type or variable occupies on your specific machine:

```cpp
sizeof(int);       // size of the int type
sizeof(double);    // size of the double type
sizeof age;        // size of the variable 'age' (parentheses optional)
```

`sizeof` returns a value of type `std::size_t` (declared in `<cstddef>`, though you do not normally need to include it just to use `sizeof`).

Example program:

```cpp
#include <iostream>

int main() {
    std::cout << "char:        " << sizeof(char)        << " bytes\n";
    std::cout << "int:         " << sizeof(int)         << " bytes\n";
    std::cout << "unsigned:    " << sizeof(unsigned)    << " bytes\n";
    std::cout << "short:       " << sizeof(short)       << " bytes\n";
    std::cout << "long:        " << sizeof(long)        << " bytes\n";
    std::cout << "long long:   " << sizeof(long long)   << " bytes\n";
    std::cout << "float:       " << sizeof(float)       << " bytes\n";
    std::cout << "double:      " << sizeof(double)      << " bytes\n";
    std::cout << "long double: " << sizeof(long double) << " bytes\n";
    return 0;
}
```

Typical output on a 32-bit compiler on a 64-bit OS:

```text
char:        1 bytes
int:         4 bytes
unsigned:    4 bytes
short:       2 bytes
long:        4 bytes
long long:   8 bytes
float:       4 bytes
double:      8 bytes
long double: 12 bytes
```

Your numbers may differ. That is the point: **`sizeof` tells you about *your* machine.**

### 7.2 `<climits>` and `<cfloat>`

These two headers provide **predefined macro constants** describing the limits and precision of the primitive types:

- `<climits>` — information about **integral** types.
- `<cfloat>` — information about **floating-point** types.

Useful constants from `<climits>`:

| Constant | Meaning |
|----------|---------|
| `CHAR_MIN`, `CHAR_MAX` | min / max value of a `char` |
| `INT_MIN`, `INT_MAX` | min / max value of an `int` |
| `SHRT_MIN`, `SHRT_MAX` | min / max value of a `short` |
| `LONG_MIN`, `LONG_MAX` | min / max value of a `long` |
| `LLONG_MIN`, `LLONG_MAX` | min / max value of a `long long` |

Example:

```cpp
#include <iostream>
#include <climits>

int main() {
    std::cout << "char min:  " << CHAR_MIN  << "\n";
    std::cout << "char max:  " << CHAR_MAX  << "\n";
    std::cout << "int min:   " << INT_MIN   << "\n";
    std::cout << "int max:   " << INT_MAX   << "\n";
    std::cout << "short min: " << SHRT_MIN  << "\n";
    std::cout << "short max: " << SHRT_MAX  << "\n";
    return 0;
}
```

Typical output:

```text
char min:  -128
char max:  127
int min:   -2147483648
int max:   2147483647
short min: -32768
short max: 32767
```

`INT_MIN` and `INT_MAX` tell you exactly where overflow will occur — valuable when you write arithmetic.

### 7.3 Why should you care?

You may wonder why any of this matters. For simple programs, it may not. But:

- **Arrays and data structures** depend on knowing how big each element is.
- **Portability** between machines depends on knowing that type sizes vary.
- **Overflow bugs** are easier to prevent when you know the limits.

This is one of the big differences between C++ and higher-level languages like Java or Python. In those languages, you are far removed from the machine. In C++, you program at a lower level, and you are expected to understand the underlying representation. That is what gives C++ its power and performance.

---

## 8. Constants

### 8.1 What a constant is

Constants are very much like variables:

- They have **names**, following the same naming rules.
- They are usually **typed**.
- They may occupy **storage** — though the compiler is free to optimize simple `const` values away.

The one crucial difference:

> **The value of a constant cannot change once it has been declared.**

```cpp
const int age{21};
age = 22;   // ❌ error: assignment of read-only variable
```

### 8.2 Why use constants?

Suppose your program needs the number of months in a year (12) in many places. You *could* write `12` everywhere. But:

- `12` could mean many things; another programmer may not know it means "months in a year."
- If the value ever needs to change, you would have to hunt down every occurrence.

A **variable** would improve readability:

```cpp
int months_in_year = 12;
```

But a variable can be changed — even accidentally. A **constant** gives you the readability *and* the guarantee:

```cpp
const int months_in_year{12};   // cannot be changed, even by mistake
```

> **Benefit of constants:** meaningful names, guaranteed immutability, and the ability to change the value in exactly one place.

### 8.3 Literal constants

A **literal** is a value written directly in the source code:

```cpp
12          // integer literal
1.56        // floating-point literal
'J'         // character literal
"Frank"     // string literal
```

Literals are the most obvious kind of constant. They have types, determined by the literal's **form and any suffix** (e.g., `42`, `42u`, `42L`, `3.14f`).

#### Integer literal suffixes

You can add suffixes to specify the exact type:

| Suffix | Meaning | Example |
|--------|---------|---------|
| `u` or `U` | unsigned | `42u` |
| `l` or `L` | long | `42L` |
| `ul`, `lu`, etc. | unsigned long | `42UL` |
| `ll`, `LL` | long long | `42LL` |

#### Floating-point literal suffixes

| Suffix | Meaning | Example |
|--------|---------|---------|
| `f` or `F` | float | `3.14f` |
| `l` or `L` | long double | `3.14L` |
| *(none)* | double | `3.14` |

#### Character literal constants: escape codes

Inside string and character literals, certain **escape codes** represent special characters. They begin with a backslash `\`:

| Escape | Meaning |
|--------|---------|
| `\n` | newline |
| `\t` | tab |
| `\r` | carriage return |
| `\\` | backslash |
| `\'` | single quote |
| `\"` | double quote |

Example:

```cpp
std::cout << "Hello\tThere\nmy friend\n";
```

Output:

```text
Hello   There
my friend
```

Escape codes are extremely common inside string literals and output statements.

### 8.4 Declared constants: `const` (the recommended way)

The most common and most recommended way to declare a constant in C++ uses the `const` keyword:

```cpp
const double price_per_room{30.0};
const double sales_tax{0.06};
const int    estimate_expiry{30};   // days
```

Rules:

- The syntax is identical to a variable declaration, with `const` added at the front.
- **A `const` definition must be initialized** when it is declared. (A pure declaration such as `extern const int x;` is the exception, but you will rarely need it.) Otherwise the compiler reports an error about an uninitialized `const`.
- Attempting to assign to a `const` constant produces a compile error: *"assignment of read-only variable"*.

### 8.5 `#define` constants (avoid in modern code)

Older C and C++ code often uses the preprocessor directive `#define`:

```cpp
#define PI 3.1415926
```

This tells the **preprocessor** to replace every occurrence of the preprocessing token `PI` with `3.1415926` — like a find-and-replace in a word processor, but operating on tokens rather than raw text. Problems:

- The preprocessor **does not understand C++**, so it cannot type-check.
- This leads to subtle, hard-to-find errors.

You will encounter `#define` constants in legacy code, so it is worth recognizing them. **Do not use them in new C++ code.** Use `const` instead.

---

## 9. Putting It All Together: A Complete Example

Let's apply everything we have learned to a small, realistic problem.

### 9.1 The problem

> Frank runs a carpet cleaning service. He charges **$30 per room**, with a **6% sales tax**. Estimates are valid for **30 days**. Write a program that asks the user for the number of rooms and prints an estimate.

Sample interaction:

```text
Hello, welcome to Frank's Carpet Cleaning Service!
How many rooms would you like cleaned? 2

Estimate for carpet cleaning service
Number of rooms: 2
Price per room: $30
Cost: $60
Tax: $3.6
=========================================
Total estimate: $63.6
This estimate is valid for 30 days
```

(Output formatting will be improved later; for now we focus on correct logic.)

### 9.2 Step 1: Write pseudocode

Experienced programmers often start with **pseudocode** — an English-like description of the algorithm. There is no "correct" form; the goal is to organize your thoughts before writing code.

```text
prompt the user for the number of rooms
read that number
display the number of rooms
display the price per room
display the cost       = rooms × price_per_room
display the tax        = rooms × price_per_room × tax_rate
display the total      = cost + tax
display the estimate validity period
```

If you cannot solve the problem in pseudocode, you cannot solve it in C++.

### 9.3 Step 2: A first version using literal constants

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, welcome to Frank's Carpet Cleaning Service!\n";
    std::cout << "\nHow many rooms would you like cleaned? ";

    int number_of_rooms{0};
    std::cin >> number_of_rooms;

    std::cout << "\nEstimate for carpet cleaning service\n";
    std::cout << "Number of rooms: " << number_of_rooms << "\n";
    std::cout << "Price per room: $" << 30 << "\n";
    std::cout << "Cost: $" << 30 * number_of_rooms << "\n";
    std::cout << "Tax: $"  << 30 * number_of_rooms * 0.06 << "\n";
    std::cout << "=========================================\n";
    std::cout << "Total estimate: $"
              << (30 * number_of_rooms) + (30 * number_of_rooms * 0.06) << "\n";
    std::cout << "This estimate is valid for " << 30 << " days\n";

    return 0;
}
```

This works. Try it with 1, 2, and 0 rooms. Test edge cases early.

### 9.4 Step 3: The problem with literals

Suppose Frank raises his price to `$32.50`. You search for `30` and **replace all**:

```text
Price per room: $32.5
Cost: $65
Tax: $3.9
Total estimate: $68.9
This estimate is valid for 32.5 days     ← OOPS
```

The number `30` appeared both as the **price per room** and as the **estimate expiry (30 days)**. A find-and-replace cannot tell them apart. In a small file this is easy to fix; in a codebase of thousands of lines, it is a serious bug.

> **Lesson:** Hard-coded literals are fragile. Give important values **names**.

### 9.5 Step 4: Refactor using declared constants

**Refactoring** means changing code to make it better *without* changing its behavior. Let's introduce `const` constants for each meaningful value:

```cpp
#include <iostream>

int main() {
    const double price_per_room{30.0};
    const double sales_tax{0.06};
    const int    estimate_expiry{30};   // days

    std::cout << "Hello, welcome to Frank's Carpet Cleaning Service!\n";
    std::cout << "\nHow many rooms would you like cleaned? ";

    int number_of_rooms{0};
    std::cin >> number_of_rooms;

    std::cout << "\nEstimate for carpet cleaning service\n";
    std::cout << "Number of rooms: " << number_of_rooms << "\n";
    std::cout << "Price per room: $" << price_per_room << "\n";
    std::cout << "Cost: $" << price_per_room * number_of_rooms << "\n";
    std::cout << "Tax: $"  << price_per_room * number_of_rooms * sales_tax << "\n";
    std::cout << "=========================================\n";
    std::cout << "Total estimate: $"
              << (price_per_room * number_of_rooms)
               + (price_per_room * number_of_rooms * sales_tax) << "\n";
    std::cout << "This estimate is valid for " << estimate_expiry << " days\n";

    return 0;
}
```

Now changing the price requires editing **exactly one line**:

```cpp
const double price_per_room{32.50};
```

Everything else updates automatically. And because `price_per_room` is `const`, no one can accidentally change it elsewhere in the program — the compiler will stop them.

### 9.6 Step 5: What is still wrong?

The refactored code is much better, but not perfect. Look at the calculation:

```cpp
price_per_room * number_of_rooms                       // cost
price_per_room * number_of_rooms * sales_tax           // tax
price_per_room * number_of_rooms + ...                 // total
```

The expression `price_per_room * number_of_rooms` appears **four times**. That is four places where the same bug could be introduced, and four places to fix if the formula ever changes.

The solution is to compute each quantity **once**, store it in a variable, and reuse the variable. We will cover this properly when we study **assignment statements**, and later we will move such calculations into **functions**. For now, notice the duplication — learning to see it is the first step to eliminating it.

> **Principle:** Every piece of logic should exist in exactly one place. Duplication is a bug waiting to happen.
