# C++ Stream Formatting

## Table of Contents

- [Introduction](#introduction)
  - [Why Learn Stream Formatting?](#why-learn-stream-formatting)
  - [What Is a Stream?](#what-is-a-stream)
  - [The Stream Objects You Already Know](#the-stream-objects-you-already-know)
  - [Headers You Need](#headers-you-need)
  - [Two Ways to Format: Manipulators vs. Member Functions](#two-ways-to-format-manipulators-vs-member-functions)
  - [The Central Rule: Persistent vs. One-Time Settings](#the-central-rule-persistent-vs-one-time-settings)
- [1. Formatting Boolean Values](#1-formatting-boolean-values)
- [2. Formatting Integer Values](#2-formatting-integer-values)
- [3. Formatting Floating-Point Values](#3-formatting-floating-point-values)
- [4. Field Width, Alignment, and Fill Characters](#4-field-width-alignment-and-fill-characters)
- [5. Putting It All Together: A Formatted Table](#5-putting-it-all-together-a-formatted-table)
- [Best Practices](#best-practices)
- [Summary Table](#summary-table)

---

## Introduction

### Why Learn Stream Formatting?

When you first learn C++, your output usually looks plain. Numbers and text print one after another with no alignment, no control over decimal places, and no special formatting. That is fine for quick tests, but real programs—reports, tables, log files, or user interfaces—need precise control over how data appears.

Stream formatting lets you:

- Align text and numbers into clean columns
- Control how many decimal places are shown (for currency or scientific data)
- Display integers in octal or hexadecimal (decimal is the default; the standard library has no built-in binary manipulator)
- Print boolean values as `true`/`false` instead of `1`/`0`
- Fill unused space with custom characters so output is easy to read

This guide teaches the core formatting tools built into C++ streams. Every rule is explained together with the reason behind it, so you will understand not only *what* to write but *why* it behaves the way it does.

### What Is a Stream?

A **stream** is an abstraction that represents a sequence of characters (or, for binary streams, bytes) flowing between your program and an input/output device. Its key advantage is **device independence**: you use the same interface whether you are reading from the keyboard, writing to the console, reading a file, or sending data over a network.

C++ provides different stream types for different jobs:

| Stream type | Direction | Typical use |
| --- | --- | --- |
| `std::istream` | Input | Bring data *into* your program |
| `std::ostream` | Output | Send data *out of* your program |
| `std::ifstream` | Input  | Read from files |
| `std::ofstream` | Output | Write to files |
| `std::fstream`  | Input/output | Read from and write to files |
| `std::stringstream` | Input/output | Read from and write to in-memory strings |

### The Stream Objects You Already Know

- `std::cin`: standard input (connected to the keyboard by default)
- `std::cout`: standard output (connected to the console by default)
- `std::cerr`: standard error (unit-buffered, i.e. `unitbuf` is set — flushed after every insertion — for immediate error messages)
- `std::clog`: standard log (buffered, for log output)

The formatting techniques in this guide work with any output stream, including file streams such as `std::ofstream`.

### Headers You Need

```cpp
#include <iostream>   // std::cout, std::cin, and the basic manipulators
#include <iomanip>    // manipulators that take an argument, such as setw
#include <fstream>    // file streams (only if you work with files)
```

Two groups of manipulators exist, and they live in different headers:

- **Simple manipulators** (no arguments), such as `std::boolalpha`, `std::hex`, `std::fixed`, `std::left`: these are available through `<iostream>` (formally declared in `<ios>`, which `<iostream>` includes).
- **Parameterized manipulators** (take an argument), such as `std::setw(n)`, `std::setprecision(n)`, `std::setfill(c)`: these require `<iomanip>`.

### Two Ways to Format: Manipulators vs. Member Functions

There are two equivalent ways to control stream formatting.

**1. Manipulators** are special functions you insert directly into the output chain. They read naturally and are the most common style in real code:

```cpp
std::cout << std::setw(10) << number;
```

**2. Member functions** are called directly on the stream object:

```cpp
std::cout.width(10);
std::cout << number;
```

Both do the same thing. This guide focuses on manipulators because they are easier to read, but we will point out the member-function equivalents where they help explain how streams work internally.

### The Central Rule: Persistent vs. One-Time Settings

This is the most important concept in the entire guide, and the most common source of beginner bugs. Formatting settings fall into two categories:

- **Persistent settings** stay active for *all future output* until you change or reset them.
- **One-time settings** apply *only to the very next output item*, then automatically reset.

Why does the distinction exist? A persistent setting describes a *preference* ("from now on, print booleans as words"). A one-time setting describes a *slot* ("this next value should occupy a field of width 10"). The slot exists only for one value, so it makes sense that it disappears after that value is printed.

Throughout this guide, every formatter is explicitly marked as **persistent** or **one-time**. Keep these labels in mind—they explain most of the surprising behavior you will encounter.

---

## 1. Formatting Boolean Values

### Default Behavior

By default, C++ prints `bool` values as integers: inserting a `bool` into a stream yields `1` for `true` and `0` for `false`:

- `true` → `1`
- `false` → `0`

```cpp
#include <iostream>

int main() {
    bool is_equal   = (10 == 10);
    bool is_unequal = (10 == 20);

    std::cout << is_equal   << "\n";  // prints: 1
    std::cout << is_unequal << "\n";  // prints: 0
    return 0;
}
```

This is correct but not very readable for a human. When you print a condition's result, you usually want to see the words `true` and `false`.

### The `boolalpha` Manipulator

`std::boolalpha` switches the stream into "word mode": booleans print as `true` and `false`.

**Persistence**: ✅ Persistent (stays active until changed)

```cpp
#include <iostream>

int main() {
    bool is_equal   = (10 == 10);
    bool is_unequal = (10 == 20);

    std::cout << std::boolalpha;       // turn on word mode
    std::cout << is_equal   << "\n";   // prints: true
    std::cout << is_unequal << "\n";   // prints: false

    // Still active for later booleans
    std::cout << (5 > 2) << "\n";      // prints: true
    return 0;
}
```

Notice the last line: `boolalpha` remains in effect, so you do not need to repeat it. This is the persistent behavior in action.

### Toggling Back with `noboolalpha`

Use `std::noboolalpha` to switch back to the default `1`/`0` output:

```cpp
std::cout << std::noboolalpha;
std::cout << true << "\n";  // prints: 1
```

### Alternative: Member Function Syntax

The same effect can be achieved with the stream's flag API:

```cpp
std::cout.setf(std::ios::boolalpha);    // same as std::boolalpha
std::cout.unsetf(std::ios::boolalpha);  // same as std::noboolalpha
```

Note that the member function used to clear a flag is `unsetf`, not `resetiosflags`. (`resetiosflags` is a *manipulator*; see the next section.)

### Key Takeaways

- Default: `true` → `1`, `false` → `0`
- `std::boolalpha` enables `true`/`false` text output
- The setting is **persistent**
- Use `std::noboolalpha` to revert

---

## 2. Formatting Integer Values

When working with low-level systems, memory addresses, or binary data, decimal numbers are often not the most readable format. C++ lets you display integers in three bases and control several visual options.

### Number Base Conversion: `dec`, `hex`, `oct`

- `std::dec`: base 10 (decimal), the default
- `std::hex`: base 16 (hexadecimal)
- `std::oct`: base 8 (octal)

**Persistence**: ✅ Persistent

```cpp
#include <iostream>

int main() {
    int num = 255;

    std::cout << std::dec << num << "\n";  // 255 (decimal)
    std::cout << std::hex << num << "\n";  // ff  (hexadecimal)
    std::cout << std::oct << num << "\n";  // 377 (octal)
    return 0;
}
```

> 💡 **Important**: The value stored in `num` never changes. Only its *display format* changes. The integer is always the same number; the base only affects how it is rendered as text.

### Showing Base Prefixes: `showbase`

Without context, `ff` and `377` could be mistaken for decimal numbers. `std::showbase` adds the standard C++ prefixes that make the base unambiguous:

- Hexadecimal numbers get a `0x` prefix
- Octal numbers get a leading `0` prefix

**Persistence**: ✅ Persistent

```cpp
#include <iostream>

int main() {
    int num = 255;

    std::cout << std::showbase;
    std::cout << std::hex << num << "\n";  // 0xff
    std::cout << std::oct << num << "\n";  // 0377
    std::cout << std::dec << num << "\n";  // 255 (no prefix for decimal)
    return 0;
}
```

Use `std::noshowbase` to turn prefixes off again.

### Uppercase Hex Output: `uppercase`

By default, hex digits `a`–`f` and the `x` in the prefix are lowercase. `std::uppercase` switches them to capital letters.

**Persistence**: ✅ Persistent

```cpp
std::cout << std::hex << std::showbase << std::uppercase << 255 << "\n";
// prints: 0XFF
```

Use `std::nouppercase` to revert to lowercase.

### Showing Positive Signs: `showpos`

By default, positive numbers have no sign; only negative numbers get a `-`. `std::showpos` adds a `+` in front of positive values.

**Persistence**: ✅ Persistent

```cpp
#include <iostream>

int main() {
    int positive = 255;
    int negative = -255;

    std::cout << std::showpos;
    std::cout << positive << "\n";  // +255
    std::cout << negative << "\n";  // -255
    return 0;
}
```

> ℹ️ `showpos` only affects decimal integers, not hex or octal output. (For floating-point values it does apply; see [Section 3](#3-formatting-floating-point-values).)

Use `std::noshowpos` to disable it.

### Resetting to Defaults

There are two idiomatic ways to restore integer defaults.

Use `std::dec` to return to decimal—this is the simplest and most readable option:

```cpp
std::cout << std::dec;
```

Use the `std::resetiosflags` manipulator to clear specific flags (requires `<iomanip>`):

```cpp
std::cout << std::resetiosflags(std::ios::showbase);  // hide base prefixes
std::cout << std::resetiosflags(std::ios::uppercase); // lowercase hex letters
```

For the base itself, `std::dec` is clearer than clearing `std::ios::basefield` manually, and it is what experienced C++ programmers write.

### Key Takeaways

- Base selectors (`dec`, `hex`, `oct`) are persistent
- `showbase` adds `0x` / `0` prefixes for clarity
- `uppercase` capitalizes hex letters and the `X` prefix
- `showpos` adds `+` to positive decimal numbers
- Always reset formatting if later code expects the defaults

---

## 3. Formatting Floating-Point Values

Floating-point formatting is all about **precision** and **notation**. Different situations demand different rules:

- Accounting: exactly 2 decimal places, fixed-point notation
- Science: scientific notation with controlled precision
- General display: clean, readable numbers without trailing zeros

The tricky part is that the meaning of "precision" changes depending on the notation. We will untangle that below.

### Default Precision Behavior

By default:

- Precision is **6 significant digits**, counted from the most significant digit
- Rounding is applied automatically
- C++ chooses between normal decimal notation and scientific notation, whichever fits better
- Trailing zeros are not printed

```cpp
#include <iostream>

int main() {
    double num1 = 1234.5678;
    double num2 = 123456789.987654321;

    std::cout << num1 << "\n";  // 1234.57   (6 significant digits)
    std::cout << num2 << "\n";  // 1.23457e+08 (too large, uses scientific)
    return 0;
}
```

Notice that `1234.5678` is rounded to `1234.57`. Six significant digits means the stream keeps six meaningful digits and drops the rest, rounding the last one.

### Changing Precision with `setprecision`

`std::setprecision(n)` changes the precision level.

**Persistence**: ✅ Persistent
**Requires**: `<iomanip>`

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double num = 1234.5678;

    std::cout << std::setprecision(9);
    std::cout << num << "\n";  // 1234.5678 (precision = 9; trailing zeros are omitted)
    return 0;
}
```

> ⚠️ In the default mode, `setprecision` controls *total significant digits*, not digits after the decimal point. This is the most common misunderstanding about floating-point formatting.

### Fixed-Point Notation: `fixed`

`std::fixed` forces fixed-point notation (a normal decimal number). More importantly, it **changes the meaning of precision**: after `fixed`, `setprecision(n)` controls the number of digits *after the decimal point*.

**Persistence**: ✅ Persistent

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double price = 1234.5678;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << price << "\n";  // 1234.57
    return 0;
}
```

This is the standard pattern for displaying currency: two digits after the decimal point, always.

### Scientific Notation: `scientific`

`std::scientific` forces scientific notation. As with `fixed`, precision means digits *after the decimal point*.

**Persistence**: ✅ Persistent

```cpp
std::cout << std::scientific << std::setprecision(3);
std::cout << 123456789.987 << "\n";  // 1.235e+08
```

### Trailing Zeros and Decimal Points: `showpoint`

In the default (`defaultfloat`) mode, C++ drops trailing zeros and even the decimal point when there is no fractional part. `std::showpoint` forces the decimal point to appear and pads with trailing zeros up to the current precision. (In `fixed` and `scientific` modes, trailing zeros are already printed according to `precision`; there, `showpoint` mainly guarantees the decimal point is shown.)

**Persistence**: ✅ Persistent

```cpp
#include <iostream>
#include <iomanip>

int main() {
    double num = 1234.0;

    std::cout << num << "\n";             // 1234
    std::cout << std::showpoint;
    std::cout << num << "\n";             // 1234.00
    return 0;
}
```

Why `1234.00`? In the default (general) mode, precision counts significant digits. The integer part has four digits, so with the default precision of 6, two zeros are appended after the decimal point to reach six significant digits.

Use `std::noshowpoint` to revert.

### Uppercase Scientific Notation

Just as with integers, `std::uppercase` capitalizes the `e` in scientific notation:

```cpp
std::cout << std::scientific << std::uppercase << std::setprecision(5);
std::cout << 12345.67 << "\n";  // 1.23457E+04
```

### Positive Signs for Floats

`std::showpos` also works with floating-point numbers:

```cpp
std::cout << std::fixed << std::showpos << 123.45 << "\n";
// prints: +123.450000
```

(The stream's default precision is 6; in `fixed` mode, precision means digits after the decimal point, so six digits appear there. If `setprecision(n)` had been called earlier, `n` digits would appear instead.)

### Resetting to the Default Floating-Point Format

To go back to the default "general" format (neither fixed nor scientific), use the `std::defaultfloat` manipulator (C++11):

```cpp
std::cout << std::defaultfloat;
```

Alternatively, use the lower-level flag API:

```cpp
std::cout.unsetf(std::ios::fixed | std::ios::scientific);
// or, with a manipulator:
std::cout << std::resetiosflags(std::ios::floatfield);
```

Prefer `std::defaultfloat`: it is short, readable, and self-explanatory.

### Common Use Case: Currency Display

The idiomatic way to print money is:

```cpp
std::cout << std::fixed << std::setprecision(2);
std::cout << "Total: $" << total << "\n";
```

Set it once at the start of your output routine, and every subsequent floating-point value will be formatted as currency.

### Key Takeaways

- Default: 6 significant digits, automatic choice of notation
- `setprecision(n)` changes precision, but its *meaning depends on the notation*
- `fixed`: precision = digits after the decimal point
- `scientific`: exponential notation; precision = digits after the decimal point
- `showpoint`: forces the decimal point and trailing zeros
- All floating-point formatters are **persistent**
- `std::defaultfloat` restores the default notation

---

## 4. Field Width, Alignment, and Fill Characters

### The Need for Field Width

When printing tables or reports, you want columns to line up neatly. Without fixed field widths, data of different lengths appears jagged and is hard to read.

C++ gives you three tools for layout:

1. **Field width**: how many characters wide the next output slot is
2. **Justification**: whether content is left-, right-, or internally aligned inside the field
3. **Fill character**: what character fills the unused space

### Setting Field Width: `setw`

`std::setw(n)` reserves a field `n` characters wide for the next output item.

**Persistence**: ❌ **One-time only** (applies only to the very next output item)
**Requires**: `<iomanip>`

By default, content is **right-justified** inside the field.

```cpp
#include <iostream>
#include <iomanip>

int main() {
    int num = 1234;

    std::cout << std::setw(10) << num << "hello\n";
    // Output: "      1234hello"
    //          ^^^^^^^^^^
    //          10-character field (right-justified)
    return 0;
}
```

Notice that `"hello"` has no field width applied—it prints immediately after the field. **`setw` only affects one output item.** You must call it before every item you want placed in a field:

```cpp
// Correct: setw before each item
std::cout << std::setw(10) << num1
          << std::setw(10) << num2
          << std::setw(10) << name << "\n";
```

The member-function equivalent is `std::cout.width(n)`, and it is also one-time.

> 💡 **Why is `setw` one-time?** Because a width describes a single output slot, not a lasting preference. Once the value has been printed, the slot no longer exists, so the width resets to 0 (meaning "as wide as needed"). This design prevents a stray `setw` from silently affecting output far away in your program.

### Alignment: `left`, `right`, and `internal`

- `std::right`: right-justified (the default)
- `std::left`: left-justified
- `std::internal`: pads between the sign/prefix and the digits (useful for aligned numbers such as `+   42`)

**Persistence**: ✅ Persistent

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    int num = 1234;
    std::string word = "hello";

    std::cout << std::left;  // left-align from now on
    std::cout << std::setw(10) << num
              << std::setw(10) << word << "\n";
    // Output: "1234      hello     "
    return 0;
}
```

`internal` is especially handy when you want signs and base prefixes to line up at the left edge of a field while the digits line up at the right:

```cpp
std::cout << std::showpos << std::internal << std::setw(6) << 42 << "\n";
// prints: "+   42"
```

> 💡 **Good practice**: always write `std::left` or `std::right` explicitly in your code. Even though `right` is the default, explicit alignment makes your intent clear to other programmers.

### Fill Characters: `setfill`

By default, empty space in a field is filled with spaces. `std::setfill(c)` changes the fill character.

**Persistence**: ✅ Persistent
**Important**: Fill characters only appear when a field width (`setw`) is also in effect. Without `setw`, there is no empty space to fill.

```cpp
#include <iostream>
#include <iomanip>

int main() {
    int num = 1234;

    std::cout << std::setfill('-');
    std::cout << std::setw(10) << num << "\n";
    // Output: "------1234" (6 dashes + 4 digits = 10 characters)
    return 0;
}
```

You can combine `left` justification with `setfill`:

```cpp
std::cout << std::left << std::setfill('*');
std::cout << std::setw(10) << "hello" << "\n";
// Output: "hello*****"
```

### Common Pitfalls

1. **Forgetting that `setw` is one-time.** This is the #1 mistake. If you set the width once and then print five items, only the first one gets the width.
2. **Expecting `setfill` to work without `setw`.** If there is no defined field width, there is no empty space to fill.
3. **Assuming alignment resets.** `left`, `right`, and `internal` are persistent. If you switch to `left`, it stays `left` until you change it back.

### Key Takeaways

- `setw(n)`: sets field width—**one-time use only**
- `left` / `right` / `internal`: control alignment—persistent
- `setfill(c)`: sets the fill character—persistent, but only works with `setw`
- Default: right-justified, space-filled

---

## 5. Putting It All Together: A Formatted Table

Here is a complete example that combines string, floating-point, field-width, alignment, and fill formatting to produce a clean tour price report.

```cpp
#include <iostream>
#include <iomanip>
#include <string>

int main() {
    // Print a ruler for reference (columns 1-30)
    std::cout << "123456789012345678901234567890\n";

    // Table header
    std::cout << std::left  << std::setw(20) << "Destination"
              << std::right << std::setw(10) << "Price"
              << "\n";

    // Separator line
    std::cout << std::setfill('-') << std::setw(30) << "" << "\n";
    std::cout << std::setfill(' ');  // reset fill character

    // Table data
    std::cout << std::left << std::setw(20) << "Buenos Aires"
              << std::right << std::fixed << std::setprecision(2)
              << std::setw(10) << 849.99 << "\n";

    std::cout << std::left << std::setw(20) << "Rio de Janeiro"
              << std::right << std::setw(10) << 1199.50 << "\n";

    std::cout << std::left << std::setw(20) << "Santiago"
              << std::right << std::setw(10) << 725.00 << "\n";

    return 0;
}
```

**Output:**

```
123456789012345678901234567890
Destination              Price
------------------------------
Buenos Aires            849.99
Rio de Janeiro         1199.50
Santiago                725.00
```

Study how the pieces fit together:

- `std::left` and `std::right` persist, so we set them once per column change.
- `std::setw(20)` and `std::setw(10)` are repeated for every cell, because they are one-time.
- `std::fixed` and `std::setprecision(2)` are set once and persist for all prices.
- `std::setfill('-')` is turned on for the separator line and reset to `' '` immediately after.

---

## Best Practices

1. **Be explicit about alignment.** Even though `right` is the default, writing `std::right` makes your code self-documenting.
2. **Restore formatting after use.** If a function changes stream formatting, restore the previous formatting state before returning (or explicitly restore the defaults if that is the function's documented contract). Otherwise, you may break code that runs later and expects the original settings.
3. **Remember that `setw` is one-time.** Repeat `setw` before every value you want to align.
4. **Use manipulators instead of manual padding.** Never pad strings with spaces by hand. Let the stream library handle alignment.
5. **The same manipulators work on files.** Everything you learned for `std::cout` works identically for `std::ofstream` file output.
6. **Save and restore persistent formatting state when writing reusable functions.** For a robust utility, capture the stream's flags, precision, and fill character, and restore them afterwards. (Do not restore `width`: it is one-time state that resets after the next insertion anyway, and re-applying it would make a stale width affect later output.)

```cpp
auto flags     = std::cout.flags();
auto precision = std::cout.precision();
auto fill      = std::cout.fill();

// ... change formatting and produce output ...

std::cout.flags(flags);
std::cout.precision(precision);
std::cout.fill(fill);
```

---

## Summary Table

| Category | Manipulator | Purpose | Persistent? |
| --- | --- | --- | --- |
| Boolean | `boolalpha` / `noboolalpha` | Print `true`/`false` or `1`/`0` | ✅ Yes |
| Integer base | `dec` / `hex` / `oct` | Change number base | ✅ Yes |
| Integer prefix | `showbase` / `noshowbase` | Show `0x` / `0` prefix | ✅ Yes |
| Letter case | `uppercase` / `nouppercase` | Capitalize hex/scientific letters | ✅ Yes |
| Positive sign | `showpos` / `noshowpos` | Show `+` on positive numbers | ✅ Yes |
| Float precision | `setprecision(n)` | Set precision level | ✅ Yes |
| Float notation | `fixed` / `scientific` / `defaultfloat` | Fixed-point, exponential, or automatic | ✅ Yes |
| Float decimal point | `showpoint` / `noshowpoint` | Force decimal point + trailing zeros | ✅ Yes |
| Field width | `setw(n)` | Set output field width | ❌ No (one-time) |
| Alignment | `left` / `right` / `internal` | Align inside field | ✅ Yes |
| Fill character | `setfill(c)` | Fill empty field space | ✅ Yes |

You now have a complete toolkit for formatting C++ stream output. The rules are consistent and predictable—the main thing to watch is which settings are persistent and which are one-time. With practice, you will be able to produce clean, professional output for console programs and file reports alike.
