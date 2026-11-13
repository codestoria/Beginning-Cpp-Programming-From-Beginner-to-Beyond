# C++ Characters and Strings

## Table of Contents

1. [Motivation: Why Text Matters](#1-motivation-why-text-matters)
2. [Characters: The Building Blocks](#2-characters-the-building-blocks)
3. [Inspecting and Converting Characters (`<cctype>`)](#3-inspecting-and-converting-characters-cctype)
4. [C-Style Strings: The Legacy Way](#4-c-style-strings-the-legacy-way)
5. [Working with C-Style Strings (`<cstring>`, `<cstdlib>`)](#5-working-with-c-style-strings-cstring-cstdlib)
6. [C++ Strings: The Modern Way](#6-c-strings-the-modern-way)
7. [C-Style vs. C++ Strings: Side by Side](#7-c-style-vs-c-strings-side-by-side)
8. [Which One Should You Use?](#8-which-one-should-you-use)
9. [Key Takeaways](#9-key-takeaways)

---

## 1. Motivation: Why Text Matters

Almost every useful program deals with text. A program reads a user's name, parses a command-line argument, searches a document, or prints a formatted report. To do any of this, you need to understand how C++ represents text.

C++ gives you two ways to work with text:

| Type | What it is | Status |
|---|---|---|
| **C-style string** | A `char` array terminated by a null character (`'\0'`) | Inherited from C; still common in libraries |
| **C++ string** | An object of the `std::string` class | Modern; preferred for new code |

This tutorial covers both. We start with the fundamentals — characters — then build up to C-style strings (so you understand what `std::string` replaces and why), and finish with `std::string` itself.

**Why this order?** Because C++ strings are built on top of C-style character arrays. Understanding the underlying mechanics shows you exactly what `std::string` does for you — and why modern C++ prefers it.

By the end, you will understand *why* modern C++ code uses `std::string`, and you will be able to read and write both kinds of strings confidently.

---

## 2. Characters: The Building Blocks

### 2.1 A `char` is an integer

At the language level, a **character** is a single symbol: a letter, digit, punctuation mark, or whitespace. In C++, the type `char` stores one such symbol.

But here is the key insight: **a `char` does not store the symbol itself — it stores an integer that maps to the symbol** through a character set such as **ASCII**. For example:

```cpp
char letter = 'A';   // stored as the integer 65
char digit  = '7';   // stored as the integer 55
```

This is why you can perform arithmetic on characters:

```cpp
char c = 'A';
c = c + 1;           // c now holds 'B' (integer 66)
```

The single quotes (`'A'`) tell the compiler "this is a character literal." Do not confuse `'A'` (a `char`) with `"A"` (a *string* literal, which is a `const char[2]` containing `'A'` and `'\0'`). We will return to this distinction shortly.

> **Technical note.** The C++ standard guarantees that `sizeof(char) == 1`, but does not guarantee that a byte is 8 bits. On almost all modern systems it is. This tutorial assumes 8-bit bytes and ASCII, which covers the overwhelming majority of real code.

### 2.2 Why ASCII?

ASCII assigns a number to each of 128 standard characters. The important ranges are:

| Range | Characters |
|---|---|
| 65–90 | `'A'`–`'Z'` (uppercase letters) |
| 97–122 | `'a'`–`'z'` (lowercase letters) |
| 48–57 | `'0'`–`'9'` (digits) |
| 32 | space |

Two consequences follow, and both matter later:

- **Uppercase letters have smaller ASCII values than lowercase letters.** `'A'` (65) < `'a'` (97).
- **Digits, letters, and punctuation are not contiguous with each other.** `'9'` (57) and `'A'` (65) have a gap between them.

### 2.3 Escape sequences

Some characters cannot be typed directly. C++ uses **escape sequences** — a backslash followed by a code — to represent them inside character and string literals.

| Escape | Meaning |
|---|---|
| `'\n'` | newline |
| `'\t'` | horizontal tab |
| `'\\'` | backslash |
| `'\''` | single quote |
| `'\"'` | double quote |
| `'\0'` | null character (integer 0) |

The null character `'\0'` deserves special attention: it is the **sentinel** that marks the end of a C-style string. We will see exactly why in Section 4.

---

## 3. Inspecting and Converting Characters (`<cctype>`)

### 3.1 Why we need this library

When processing text, you constantly need to ask questions about individual characters:

- Is this character a letter? A digit?
- Is it uppercase or lowercase?
- Is it whitespace?

You *could* write these checks yourself (`c >= 'a' && c <= 'z'`), but that is tedious, easy to get wrong, and not portable to non-ASCII character sets. The `<cctype>` header provides tested, efficient functions for exactly this job.

```cpp
#include <cctype>
```

### 3.2 Two categories of functions

**Testing functions** answer a yes/no question:

| Function | Returns nonzero if the character is… |
|---|---|
| `isalpha(c)` | a letter (uppercase or lowercase) |
| `isalnum(c)` | a letter or a digit |
| `isdigit(c)` | a digit (`'0'`–`'9'`) |
| `islower(c)` | a lowercase letter |
| `isupper(c)` | an uppercase letter |
| `isprint(c)` | a printable character (not a control character) |
| `ispunct(c)` | a punctuation character |
| `isspace(c)` | whitespace (space, tab, newline, etc.) |

> **Precision matters.** These functions return an `int`, not a `bool`. A nonzero result means "true"; zero means "false." In `if (isalpha(c))` the integer is implicitly converted to `bool`, so the distinction rarely matters in practice — but it is why the return type is documented as `int`.

**Conversion functions** return a transformed character:

| Function | Returns |
|---|---|
| `tolower(c)` | the lowercase equivalent of `c`, or `c` unchanged if no conversion exists |
| `toupper(c)` | the uppercase equivalent of `c`, or `c` unchanged if no conversion exists |

```cpp
char a = tolower('A');    // 'a'
char b = toupper('b');    // 'B'
char c = toupper('5');    // '5'  (unchanged — no uppercase form)
```

### 3.3 Example

```cpp
#include <iostream>
#include <cctype>

int main() {
    char c = 'A';
    if (isalpha(c)) {
        std::cout << c << " is a letter\n";
    }
    std::cout << "Lowercase: " << static_cast<char>(tolower(c)) << "\n";
}
```

Output:

```
A is a letter
Lowercase: a
```

> **Technical note.** `<cctype>` functions take an `int` argument representing a character value. Passing a plain `char` works for ASCII, but if `char` is signed and the character's byte value is above 127 (so the `char` value is negative), the result is technically undefined. For fully portable code, cast first: `isalpha(static_cast<unsigned char>(c))`. This matters for non-ASCII text; for English text it is a non-issue.

---

## 4. C-Style Strings: The Legacy Way

### 4.1 What a C-style string is

A **C-style string** is:

- a **sequence of characters** stored **contiguously** in memory,
- implemented as an **array of `char`**,
- **terminated by a null character** (`'\0'`, integer 0).

Because the string ends at the null character, C-style strings are also called **null-terminated** or **zero-terminated** strings. The null is a *sentinel*: functions that process the string have no other way to know where it stops.

### 4.2 String literals

Any text in double quotes is a **string literal**, and every string literal is a C-style string:

```cpp
"C++ is fun"
```

Two facts about string literals:

1. **They are constants.** You cannot modify the characters of a literal; doing so is undefined behavior.
2. **The compiler silently appends `'\0'`.** You write 10 visible characters; the compiler allocates 11 bytes.

```
Index:   0   1   2   3   4   5   6   7   8   9   10
Char:    C   +   +   ' ' i   s   ' ' f   u   n   '\0'
```

### 4.3 Creating C-style string variables

You create a C-style string by declaring a `char` array:

```cpp
char name1[6] = {'F', 'r', 'a', 'n', 'k', '\0'};  // explicit
char name2[6] = "Frank";                            // compiler adds '\0'
```

Important properties:

- **An array-based C-style string has a fixed size at compile time.** It cannot grow or shrink at runtime. (A `char*` can point to dynamically allocated memory, but then you must manage allocation and deallocation yourself.)
- **Extra space is zero-filled.**

```cpp
char name[8] = "Frank";   // F r a n k \0 \0 \0
```

- **Individual characters are accessed with `[]`:**

```cpp
char first = name[0];     // 'F'
```

### 4.4 The dangers of C-style strings

C-style strings are simple and fast, but they come with serious pitfalls. Understanding these is the main reason `std::string` exists.

**1. Uninitialized local arrays contain garbage.**

```cpp
char name[20];              // uninitialized
std::cout << name;          // undefined behavior — prints garbage or crashes
```

The array holds whatever bytes happen to be in memory. (Global and static arrays, by contrast, are zero-initialized.) If no null character appears nearby, printing walks off into unrelated memory.

**2. You cannot assign one C-style string to another with `=`.**

```cpp
char a[10];
char b[10] = "Frank";
a = b;                      // ❌ compiler error
```

An array name decays to a pointer to its first element in most expressions, and arrays themselves are not assignable. Copying a pointer is not a string copy. To copy the characters, you must call a function such as `strcpy` (Section 5).

**3. Buffer overflows are easy and catastrophic.**

If you copy more characters into an array than it can hold, you overwrite adjacent memory. The program may crash, silently corrupt data, or — worse — appear to work.

**4. Every operation relies on the null terminator.**

To find the length of a string, C++ has to scan character by character until it sees `'\0'`. If the null is missing (for example, because you filled the array without leaving room for it), the scan continues past the end of the array — undefined behavior.

**5. `cin >>` into a `char` array is unsafe.**

```cpp
char name[20];
std::cin >> name;           // ⚠️ overflow if input exceeds 19 characters
```

`>>` has no idea how large the array is. Always cap the read:

```cpp
#include <iomanip>
std::cin >> std::setw(20) >> name;   // reads at most 19 characters + '\0'
```

or use `cin.getline` (Section 5.6).

---

## 5. Working with C-Style Strings (`<cstring>`, `<cstdlib>`)

### 5.1 The `<cstring>` library

Because ordinary operators do not work on C-style strings, C++ provides a set of functions in `<cstring>`:

```cpp
#include <cstring>
```

Every one of these functions shares the same assumption: **the string is null-terminated.**

| Function | Purpose |
|---|---|
| `strlen(s)` | length of `s` (characters before `'\0'`) |
| `strcpy(dest, src)` | copy `src` into `dest` |
| `strcat(dest, src)` | append `src` to the end of `dest` |
| `strcmp(s1, s2)` | compare `s1` and `s2` |

### 5.2 How `strlen` works — and why it is slow

Because no length is stored with a C-style string, `strlen` must walk the array from the start, counting characters until it sees `'\0'`. For `"Frank"`:

```
F   → count 1
r   → count 2
a   → count 3
n   → count 4
k   → count 5
'\0'→ stop → return 5
```

This is why calling `strlen(s)` inside a loop condition is a performance trap: the loop becomes O(n²). Cache the length first:

```cpp
size_t len = strlen(s);        // compute once
for (size_t i = 0; i < len; ++i) { /* ... */ }
```

### 5.3 `size_t`

`strlen` does **not** return `int`. It returns **`size_t`**, an unsigned integer type defined by the standard library (the exact underlying type — `unsigned int`, `unsigned long`, etc. — depends on the platform).

Use `size_t` for lengths, indices, and anything representing a size. Sizes are never negative, so unsigned arithmetic is the correct and portable choice.

```cpp
size_t len = strlen("Frank");   // 5
```

> **Trap to know.** Because `size_t` is unsigned, subtracting can wrap around. `s.length() - 1` when `s` is empty produces a huge number, not `-1`. Guard against empty strings before subtracting.

### 5.4 `strcpy` and `strcat`

```cpp
char first[20] = "Frank";
char full[50];

strcpy(full, first);            // full = "Frank"
strcat(full, " ");              // full = "Frank "
strcat(full, "Mitropoulos");    // full = "Frank Mitropoulos"
```

Read the argument order carefully: `strcpy(dest, src)` means "copy `src` into `dest`" — the data flows from the second argument into the first. Students often find this backwards; say it out loud until it sticks.

Both functions copy the terminating `'\0'`, so the destination stays properly terminated. **You are responsible** for ensuring `dest` is large enough — the functions do not check.

> **Safer variants** like `strncpy` and `strncat` take a maximum length, but `strncpy` has its own trap: if the source is at least as long as `n`, no null terminator is written. Modern C++ code avoids all of this by using `std::string`.

### 5.5 `strcmp`

```cpp
int result = strcmp(s1, s2);
```

- Returns **0** if the strings are identical, character by character.
- Returns a **negative** value if `s1` comes before `s2` lexicographically.
- Returns a **positive** value if `s1` comes after `s2`.

The exact magnitude is unspecified — only the sign matters. `strcmp` is the standard way to compare C-style strings. The operators `==`, `<`, and `>` do **not** compare contents: `==` compares addresses, and the result of `<`/`>` on pointers to unrelated objects is unspecified — it has no lexicographic meaning.

### 5.6 Reading a full line

`cin >>` stops at the first whitespace. To read an entire line, use `cin.getline`:

```cpp
char fullName[50];
cin.getline(fullName, 50);   // reads up to 49 chars, or until Enter
```

The second argument is the buffer size. `getline` reads at most `size - 1` characters and appends `'\0'`. If the line is longer, `getline` sets the stream's failbit — you must clear it before reading again:

```cpp
cin.clear();
cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```

### 5.7 Converting strings to numbers: `<cstdlib>`

```cpp
#include <cstdlib>

int n = atoi("12");         // 12
double d = atof("3.14");    // 3.14
```

`atoi` does **not** report errors. If the string is not a number, it returns 0 — indistinguishable from a valid `"0"`. For real code, prefer `std::stoi` (C++11, declared in `<string>`), which throws `std::invalid_argument` or `std::out_of_range` on failure.

### 5.8 A complete example

```cpp
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>
using namespace std;

int main() {
    char firstName[20] = "";
    char lastName[20]  = "";
    char fullName[50]  = "";

    cout << "Enter your first name: ";
    cin >> std::setw(20) >> firstName;         // at most 19 chars + '\0'

    cout << "Enter your last name: ";
    cin >> std::setw(20) >> lastName;

    cout << "Hello " << firstName
         << ", your first name has "
         << strlen(firstName) << " characters.\n";

    // Build the full name
    strcpy(fullName, firstName);
    strcat(fullName, " ");
    strcat(fullName, lastName);
    cout << "Your full name is " << fullName << "\n";

    // Uppercase every letter
    size_t len = strlen(fullName);             // cache — avoid O(n²)
    for (size_t i = 0; i < len; ++i) {
        unsigned char uc = static_cast<unsigned char>(fullName[i]);
        if (isalpha(uc)) {
            fullName[i] = static_cast<char>(toupper(uc));
        }
    }
    cout << "Uppercase: " << fullName << "\n";
}
```

Points to notice:

- **Initialize every array.** Uninitialized C-style strings print garbage.
- **`cin >>` stops at whitespace and does not know the buffer size.** Use `std::setw(20)` to cap the read. Typing "Frank Mitropoulos" into `firstName` stores only `"Frank"`.
- **Use `size_t`** for indices over strings.
- **Cache `strlen`** — it rescans the string on every call.
- **`toupper` returns `int`**, so cast back to `char` when storing. Cast the argument to `unsigned char` first for portability.

---

## 6. C++ Strings: The Modern Way

### 6.1 Why `std::string`

C-style strings work, but as Section 4 showed, they are error-prone and awkward. Modern C++ provides **`std::string`**, a class from the C++ standard library (often loosely called the STL) that manages all the bookkeeping for you:

- **Dynamic size** — grows and shrinks automatically.
- **Always initialized** — a newly declared `std::string` is empty, never garbage.
- **Works with ordinary operators** — `=`, `+`, `==`, `<`, `[]`, `+=`.
- **Rich set of methods** — `substr`, `find`, `erase`, `length`, and many more.
- **Bounds-checked access** via `.at()`.
- **Works directly with `cin` and `cout`.**

### 6.2 Setup

```cpp
#include <string>
using namespace std;
```

`std::string` lives in the `std` namespace. Without `using namespace std;`, write `std::string` instead of plain `string`. Method calls such as `s.length()` are unaffected — only the type name needs the `std::` prefix.

> **Style note.** `using namespace std;` is fine in small programs and tutorials. In header files and large projects, avoid it — it pulls hundreds of names into scope and causes collisions.

### 6.3 Declaring and initializing

```cpp
string s1;                  // empty string (not garbage!)
string s2{"Frank"};         // from a C-style literal
string s3{s2};              // copy of s2
string s4{"Frank", 3};      // first 3 chars of "Frank" → "Fra"
string s5{s3, 0, 2};        // from s3, starting at index 0, length 2 → "Fr"
string s6(3, 'X');          // three X's → "XXX"  (note the parentheses)
```

> **Why parentheses for `s6`?** Braces `{3, 'X'}` would match `string`'s `initializer_list<char>` constructor and produce a two-character string (the character with ASCII value 3, followed by `'X'`). Parentheses `(3, 'X')` select the `(size_t count, char ch)` constructor instead — three `'X'` characters. This is a subtle but real difference.

### 6.4 Assignment copies the value

```cpp
string s1 = "C++ rocks";
string s2 = "hello";
s1 = s2;                    // s1 is now an independent copy of s2
```

After assignment, `s1` and `s2` are separate strings. Modifying one does not affect the other.

### 6.5 Concatenation with `+` and `+=`

```cpp
string part1{"C++"};
string part2{"powerful"};
string sentence = part1 + " is a " + part2 + " language";
// "C++ is a powerful language"

string name{"Frank"};
name += " James";           // "Frank James"
```

**Rule:** at least one operand of `+` must be a `std::string`.

```cpp
string bad = "Hello" + "World";              // ❌ both are const char[]
string good = string{"Hello"} + " World";    // ✅
string alsoGood = "Hello " + string{"World"};// ✅
```

Two C-style literals cannot be added — the compiler has no `operator+` that takes two `const char*` operands. As soon as one side is a `std::string`, the other is converted automatically.

### 6.6 Accessing characters

```cpp
string s{"Frank"};
char c1 = s[0];       // 'F' — no bounds check
char c2 = s.at(0);    // 'F' — bounds checked
s[0] = 'f';           // "frank"
```

- `[]` does **not** check bounds. Out-of-range access is undefined behavior.
- `.at()` **does** check and throws `std::out_of_range` — safer during development.

### 6.7 Looping through characters

```cpp
string s{"apple"};

// Classic for loop
for (size_t i = 0; i < s.length(); ++i)
    cout << s[i];

// Range-based for loop (read-only)
for (char c : s)
    cout << c;

// Range-based for loop (modify in place)
for (char& c : s)
    c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
```

Use **`size_t`**, not `int`, for the index. And be careful with the range-based loop: `for (char c : s)` copies each character; `for (char& c : s)` gives a reference so you can modify the string.

### 6.8 Comparison

`std::string` supports `==`, `!=`, `<`, `<=`, `>`, `>=`:

```cpp
string s1{"apple"};
string s2{"banana"};
string s3{"apple"};

s1 == s3;           // true  (same contents)
s1 == "apple";      // true  (literal converted to string)
s1 == s2;           // false
```

Comparison is **lexicographic, character by character**, using ASCII values. Two consequences:

- `"Apple" < "apple"` because `'A'` (65) < `'a'` (97).
- Comparison is **case-sensitive**.

You can compare a `std::string` with a C-style literal. Do **not** use these operators to compare the *contents* of two C-style strings or literals: `==` compares addresses, not text, and the result of `<`/`>` on unrelated pointers is unspecified. For C-style strings, use `strcmp` (Section 5.5).

### 6.9 Useful methods

**`substr(pos, len)`** — extract a substring:

```cpp
string s{"This is a test"};
s.substr(0, 4);     // "This"
s.substr(5, 2);     // "is"
s.substr(10, 4);    // "test"
```

If fewer than `len` characters remain, all remaining characters are returned. If `pos > s.size()`, `substr` throws `std::out_of_range`.

**`find(target, start)`** — locate a substring or character:

```cpp
string s{"This is a test"};
s.find("This");     // 0
s.find("is");       // 2
s.find("is", 4);    // 5  (search starts at index 4)
s.find('e');        // 11
s.find("xyz");      // string::npos
```

- Returns the index of the first match.
- Returns **`string::npos`** (a special constant) if not found.

Always check the result:

```cpp
size_t pos = s.find(word);
if (pos != string::npos)
    cout << "Found at " << pos << "\n";
else
    cout << "Not found\n";
```

**`rfind(...)`** — same as `find`, but searches from the end backward.

**`erase(pos, len)`** and **`clear()`**:

```cpp
string s{"This is a test"};
s.erase(0, 5);      // "is a test"
s.clear();          // ""
```

**`length()`** (or its synonym **`size()`**) — number of characters. This is fast: the length is stored inside the object, so it does not rescan.

```cpp
string s{"Frank"};
s.length();         // 5
```

**`c_str()`** — return a `const char*` pointing to the internal buffer, for passing to C APIs:

```cpp
printf("%s\n", s.c_str());
```

The pointer is valid only while the string exists and is unmodified.

### 6.10 Input with `std::string`

```cpp
string name;
cin >> name;        // stops at whitespace
```

`>>` works, but like C-style strings, it stops at the first whitespace. To read a full line:

```cpp
string fullName;
getline(cin, fullName);         // reads everything up to '\n'
```

`getline` consumes the newline but does **not** store it.

A second overload takes a delimiter:

```cpp
string s;
getline(cin, s, 'x');           // stop at 'x'
```

For input `"this is an x example"`, `s` becomes `"this is an "`, and the `x` is consumed and discarded.

> **Classic trap: mixing `>>` and `getline`.** `cin >> age` leaves the newline in the input buffer. A subsequent `getline(cin, name)` sees that newline immediately and returns an empty string. Fix it by discarding the leftover newline:
>
> ```cpp
> cin >> age;
> // requires #include <limits>
> cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
> getline(cin, name);
> ```

---

## 7. C-Style vs. C++ Strings: Side by Side

| Task | C-style string | `std::string` |
|---|---|---|
| Declare | `char s[20];` | `string s;` |
| Initialize | `char s[20] = "Frank";` | `string s{"Frank"};` |
| Default value | Garbage for uninitialized local arrays | Empty string |
| Assign | `strcpy(dest, src);` | `dest = src;` |
| Concatenate | `strcat(dest, src);` | `dest += src;` or `dest = a + b;` |
| Compare | `strcmp(a, b) == 0` | `a == b` |
| Length | `strlen(s)` | `s.length()` |
| Read a line | `cin.getline(buf, size);` | `getline(cin, s);` |
| Memory | Fixed-size array or manual dynamic allocation; null-terminated | Dynamic, automatic |
| Access character | `s[i]` | `s[i]` or `s.at(i)` |
| Safety | Manual — overflow, missing `'\0'` | Managed by the class |

---

## 8. Which One Should You Use?

**Use `std::string` for new C++ code.** It is safer, easier to read, and less error-prone. C-style strings remain relevant because:

- You will encounter them in older code and in C libraries.
- Many APIs (operating system calls, legacy libraries) accept `const char*`.
- Interoperation is easy: `std::string` constructs from a C-style literal, and `.c_str()` extracts a `const char*` when needed.

But for new code, `std::string` is the default choice. Every operation in this tutorial — building, comparing, searching, reading lines — is simpler and safer with `std::string`.

---

## 9. Key Takeaways

1. **A `char` stores an integer** that maps to a character through a character set such as ASCII. `'A'` is 65; `'a'` is 97.
2. **`<cctype>`** provides `isalpha`, `isdigit`, `isspace`, `toupper`, `tolower`, and friends for inspecting and converting single characters. They return `int`, not `bool`.
3. **C-style strings** are `char` arrays terminated by `'\0'`. They are fixed-size, unsafe, and require functions from `<cstring>`.
4. **Every C-style string function depends on the null terminator.** Always initialize arrays, always leave room for `'\0'`, and never `cin >>` into a `char` array without a width limit.
5. **`std::string`** is a class from the C++ standard library. It is dynamic, always initialized, works with ordinary operators, and offers a rich set of methods.
6. **Use `size_t`** for string lengths and indices. It is the correct portable type for non-negative sizes — and remember that unsigned subtraction can wrap around.
7. **To read a full line**, use `getline(cin, s)` for `std::string` or `cin.getline(buf, size)` for C-style strings. Beware of mixing `>>` and `getline`.
8. **Prefer `std::string`.** It is the default choice in modern C++ and the foundation for the text-processing work you will do throughout your career.

Once you are comfortable with these tools, you can handle almost any text-processing task — and you will have taken a real step into object-oriented programming, because `std::string` is one of the classes you will use most often in C++.
