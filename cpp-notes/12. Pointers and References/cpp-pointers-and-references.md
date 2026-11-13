# C++ Pointers and References

## Table of Contents

1. [Introduction: Why Pointers Matter](#1-introduction-why-pointers-matter)
2. [What Is a Pointer?](#2-what-is-a-pointer)
3. [Declaring and Initializing Pointers](#3-declaring-and-initializing-pointers)
4. [Accessing Addresses and Storing Them in Pointers](#4-accessing-addresses-and-storing-them-in-pointers)
5. [Dereferencing a Pointer](#5-dereferencing-a-pointer)
6. [Dynamic Memory Allocation](#6-dynamic-memory-allocation)
7. [The Relationship Between Arrays and Pointers](#7-the-relationship-between-arrays-and-pointers)
8. [Pointer Arithmetic](#8-pointer-arithmetic)
9. [Const and Pointers](#9-const-and-pointers)
10. [Passing Pointers to Functions](#10-passing-pointers-to-functions)
11. [Returning a Pointer from a Function](#11-returning-a-pointer-from-a-function)
12. [Common Pointer Pitfalls](#12-common-pointer-pitfalls)
13. [What Is a Reference?](#13-what-is-a-reference)
14. [L-values and R-values](#14-l-values-and-r-values)
15. [Summary: Pointers vs. References](#15-summary-pointers-vs-references)

---

## 1. Introduction: Why Pointers Matter

Pointers are one of C++'s most powerful and most misunderstood features. They give your program direct access to memory, including the ability to allocate and release memory *while the program is running* — something most high-level languages hide from you.

This control matters in domains where performance and predictability are critical: operating systems, device drivers, embedded systems, game engines, and real-time software. In these contexts, a garbage collector or virtual machine would introduce overhead and timing unpredictability that cannot be tolerated.

You may have heard that pointers are dangerous, or that languages like Java and Python "solve" the problem by hiding them. There is some truth to this: pointers demand responsibility, because you must allocate and release memory correctly. But the core idea is far simpler than its reputation suggests:

> **A pointer is just a variable that stores a memory address.**

That's it. Everything else — dereferencing, arithmetic, dynamic allocation — follows from that single definition.

This guide builds the concept from first principles. By the end, you will understand not only *how* pointers work, but *why* they behave the way they do — and when to use them instead of references.

---

## 2. What Is a Pointer?

### The Simple Definition

A pointer is a variable. Like any variable, it:

- Has a **memory address** where it is stored
- Has a **type** (e.g., `int*`, `double*`, `std::string*`)
- Holds a **value**

The distinguishing feature is that a pointer's value is a **memory address** (or a null pointer value). A pointer may refer to a valid object or function, may be null, or may have become invalid (for example, after the object it referred to has been destroyed).

### A Concrete Example

```cpp
int x = 10;
```

Here, `x` is an integer variable. It lives at some memory location (say, address `1000`), and it holds the value `10`.

Now consider:

```cpp
int* p = &x;
```

`p` is a pointer to an integer. It lives at its own memory location (say, address `2000`), and it holds the address of `x` (`1000`).

**Key insight:** `p` and `x` are two separate variables. `p` does *not* contain `10`; it contains the address where `10` lives.

### A Visual Mental Model

Think of memory as a long row of numbered mailboxes. Each mailbox has a unique number (its address) and can hold some data (its value). A pointer is a variable whose value is one of those mailbox numbers.

```text
Memory (simplified):

Address:   1000          2000
          +-------+     +-------+
          |  10   |     | 1000  |----+
          +-------+     +-------+    |
              x              p       |
                                     |
                                     +--> points to address 1000 (x)
```

When you later write `*p`, you are saying: "Go to the mailbox whose number is stored in `p`, and read (or write) what is inside."

### Why Use Pointers?

If you can just use `x` directly, why bother with a pointer? There are several important reasons:

1. **Accessing data that is not in the current function's scope.** For example, if you need a function to modify a caller's local variable, the variable's name is not visible inside the function. A pointer lets you carry its address into the function.

2. **Efficient array manipulation.** Pointers and arrays are deeply related in C++. Pointer arithmetic allows efficient traversal of contiguous memory.

3. **Dynamic memory allocation.** When you allocate memory on the heap at runtime, that memory has no variable name. The only way to access it is through a pointer.

4. **Low-level system programming.** In embedded systems, device drivers, and OS development, you sometimes need to access specific memory addresses directly. Pointers are the tool for this.

5. **Optional values.** A pointer can be `nullptr`, meaning "pointing to nothing." This provides a natural way to represent an optional value or a "not found" result.

---

## 3. Declaring and Initializing Pointers

### Declaration Syntax

Declaring a pointer is almost identical to declaring a regular variable, except you add an asterisk (`*`) before the variable name:

```cpp
int* int_ptr;          // pointer to an int
double* double_ptr;    // pointer to a double
char* char_ptr;        // pointer to a char
std::string* str_ptr;  // pointer to a std::string
```

The asterisk in a declaration is **not** a mathematical operator. It simply marks the variable as a pointer. Read declarations right-to-left: "`int_ptr` is a pointer to an `int`."

> **Style note:** You may see `int *int_ptr` (asterisk next to the name) or `int* int_ptr` (asterisk next to the type). Both are equivalent to the compiler. This guide uses the latter style, but be aware of both.

### The Critical Importance of Initialization

**Always initialize your pointers.** An uninitialized pointer contains garbage data — a random bit pattern that the compiler interprets as an address. This means the pointer could point *anywhere* in memory. Using it could corrupt data, crash your program, or (worst case) appear to work for a while before failing mysteriously.

```cpp
int* p;  // DANGER: p contains garbage; it points "anywhere"
```

There are two safe ways to initialize a pointer:

**1. Initialize to `nullptr` (pointing nowhere):**

```cpp
int* p = nullptr;  // p is a null pointer: it points to no object
```

`nullptr` is the null pointer value: it explicitly means "points to no object or function." It is a well-defined "pointing nowhere" state, very different from "pointing anywhere." You can safely check `if (p != nullptr)` before dereferencing.

> **Precision note:** the C++ standard does not guarantee that the null pointer's numeric representation is `0`, nor that address `0` is inaccessible on every platform. What *is* guaranteed is that dereferencing a null pointer is undefined behavior.

**2. Initialize to the address of an existing variable:**

```cpp
int x = 10;
int* p = &x;  // p now points to x
```

> **Best practice:** Initialize every pointer at the point of declaration — either to `nullptr` or to a valid address. This single habit eliminates an entire class of bugs.

---

## 4. Accessing Addresses and Storing Them in Pointers

### The Address-of Operator (`&`)

The address-of operator (`&`) is a unary operator that evaluates to the memory address of its operand:

```cpp
int num = 10;
std::cout << &num;  // prints something like 0x7ffee3b5a8
```

For the built-in `&` operator, the operand must be an **l-value** — something that has a persistent memory location. (We will define l-values precisely in Section 14, but for now, think of an l-value as "a named object that lives in memory.") You cannot take the address of a numeric literal or a temporary expression with the built-in `&` operator:

```cpp
&100;        // ERROR: 100 is a numeric literal (prvalue), not addressable
&(x + y);    // ERROR: (x + y) is a temporary (prvalue), not addressable
```

### Storing Addresses in Pointers

```cpp
int num = 10;
int* p = &num;  // p holds the address of num
```

Now `p` "points to" `num`. You can verify this by printing both:

```cpp
std::cout << &num;  // address of num
std::cout << p;     // same address (the value stored in p)
```

### Pointer Size vs. Pointee Size

A crucial distinction: **the size of a pointer is not the size of what it points to.**

```cpp
int* p1 = nullptr;
double* p2 = nullptr;
std::vector<std::string>* p3 = nullptr;

std::cout << sizeof(p1);  // typically 4 or 8 bytes
std::cout << sizeof(p2);  // same size as p1
std::cout << sizeof(p3);  // same size as p1
```

On most common platforms, all **object** pointers are the same size because they all store memory addresses: typically 4 bytes on a 32-bit system and 8 bytes on a 64-bit system. The C++ standard does not guarantee that every pointer type has the same size — function pointers, in particular, may differ — but for object pointers on a given platform they usually do. The size of the *pointee* (the thing pointed to) is irrelevant to the pointer's own size.

### Type Safety

C++ is strict about pointer types. A pointer declared as `int*` can only hold addresses of `int` variables:

```cpp
int score = 10;
double high_temp = 100.7;

int* score_ptr = &score;  // OK: score is an int
score_ptr = &high_temp;   // ERROR: high_temp is a double
```

Even though both addresses might be the same size, the compiler enforces type correctness. This prevents subtle bugs where you might interpret the bytes of a `double` as an `int`.

---

## 5. Dereferencing a Pointer

### Following the Pointer

To access the data a pointer points to, you **dereference** it using the asterisk (`*`) — the same symbol used in declaration, but in a different context:

```cpp
int score = 100;
int* score_ptr = &score;

std::cout << *score_ptr;  // prints 100 (the value at the address)
```

The dereference operator follows the pointer to the address it holds and gives you access to the value stored there.

### Dereferencing on the Left-Hand Side

When you dereference a pointer on the left side of an assignment, you are assigning to the memory location the pointer points to:

```cpp
*score_ptr = 200;    // changes score to 200
std::cout << score;  // prints 200
```

This is the essence of **indirect manipulation**: you change a variable's value without using its name directly.

### A Mental Model

Think of a pointer as a slip of paper with an address written on it. Dereferencing is like going to that address and interacting with whatever is there. The slip of paper itself (the pointer variable) is separate from the house at that address (the pointee).

```cpp
int high_temp = 100;
int low_temp = 37;
int* temp_ptr = &high_temp;  // temp_ptr holds address of high_temp

std::cout << *temp_ptr;      // 100
temp_ptr = &low_temp;        // now temp_ptr holds address of low_temp
std::cout << *temp_ptr;      // 37
```

Changing the pointer itself (which address it holds) is different from changing the value at that address. Both are valid operations, and understanding the distinction is essential.

> **Key takeaway:** The pointer and the pointee are independent. You can change *where* a pointer points without affecting the data at either location, and you can change the data *at* the pointee without changing the pointer.

---

## 6. Dynamic Memory Allocation

### Why Dynamic Allocation?

Often, you don't know at compile time how much memory you need. If you're modeling students in a class, how many students should you allocate for? Arrays are fixed in size once allocated. `std::vector` solves this problem internally by using dynamic allocation, but understanding how dynamic allocation works is still important — especially when working with raw pointers, legacy code, or low-level APIs.

All dynamically allocated memory comes from a region called the **heap** (or **free store**). This is separate from the **stack**, where local variables and function activation records live.

### Stack vs. Heap: A Quick Comparison

```text
+------------------+    High addresses
|      Stack       |
|  (local vars,    |
|   function       |
|   activation     |
|   records)       |
|        |         |
|        v         |
|                  |
|        ^         |
|        |         |
|      Heap        |
|  (dynamically    |
|   allocated      |
|   memory)        |
+------------------+    Low addresses
```

| Feature | Stack | Heap |
|---------|-------|------|
| Allocation | Automatic (compiler-managed) | Manual (`new` / `delete`) |
| Lifetime | Until end of scope | Until explicitly freed |
| Size | Typically small (a few MB) | Limited by available RAM |
| Speed | Very fast | Slower (allocation overhead) |
| Flexibility | Fixed at compile time | Can grow at runtime |

The stack is fast and automatic but limited in size and lifetime. The heap is slower and requires manual management but offers flexibility and a much larger capacity.

### Allocating a Single Object

Use the `new` keyword to allocate memory at runtime:

```cpp
int* int_ptr = new int;  // allocates an int on the heap
```

This does three things:

1. Allocates enough memory for an `int` on the heap
2. Returns the address of that memory
3. Stores that address in `int_ptr`

The allocated memory is **uninitialized** — its value is indeterminate. Reading it before assigning a value is undefined behavior:

```cpp
int* int_ptr = new int;  // uninitialized: do not read yet
*int_ptr = 100;          // now the heap int holds 100
std::cout << *int_ptr;   // 100
```

You can also initialize at allocation time:

```cpp
int* int_ptr = new int(100);  // allocates an int initialized to 100
```

### Allocating an Array

Use `new[]` to allocate a contiguous block of memory:

```cpp
int size;
std::cout << "How many integers? ";
std::cin >> size;

int* array_ptr = new int[size];  // allocates an array of ints
```

The address of the first element is stored in `array_ptr`. The remaining elements are contiguous in memory and can be accessed via pointer arithmetic or subscript notation.

### Deallocating Memory

When you're done with dynamically allocated memory, you must release it:

```cpp
delete int_ptr;      // for a single object
delete[] array_ptr;  // for an array (note the empty brackets)
```

**Critical rules:**

- Every `new` must have exactly one matching `delete`.
- Every `new[]` must have exactly one matching `delete[]`.
- Never `delete` memory that wasn't allocated with `new`.
- Never `delete` the same memory twice.

> **Warning:** Mixing `new` with `delete[]` or `new[]` with `delete` is undefined behavior — even if it seems to work on your compiler. Always match the forms.

### Memory Leaks

If you lose the pointer to dynamically allocated memory — for example, by letting it go out of scope or reassigning it — that memory becomes **orphaned**. It remains marked as "in use" by C++ but is unreachable. This is a **memory leak**.

```cpp
int* p = new int(100);  // allocate
p = nullptr;            // LEAK: the heap int is now unreachable
```

The heap memory still exists, but there is no way to access or free it. Over time, leaks can exhaust available memory.

### What Happens When `new` Fails

If the system cannot allocate the requested memory, `new` throws a `std::bad_alloc` exception. If you do not catch it, the program terminates. In production code, you should use exception handling to manage this gracefully:

```cpp
try {
    int* big_array = new int[1000000000];
} catch (const std::bad_alloc& e) {
    std::cerr << "Allocation failed: " << e.what() << '\n';
}
```

> **Modern C++ note:** Raw `new` and `delete` are rarely needed in modern C++. Smart pointers (`std::unique_ptr`, `std::shared_ptr`) automate memory management and eliminate most leaks and double-deletes. They are covered in advanced C++ material, but you should know they exist.

---

## 7. The Relationship Between Arrays and Pointers

### The Key Insight

In C++, an **array name evaluates to the address of its first element** in most contexts. This is why arrays and pointers are so closely related:

```cpp
int scores[] = {100, 95, 89};
int* score_ptr = scores;  // same as &scores[0]
```

Now `scores` and `score_ptr` can be used almost interchangeably:

```cpp
std::cout << scores[0];     // 100
std::cout << *score_ptr;    // 100
std::cout << score_ptr[0];  // 100
```

The key difference: `scores` is **not a modifiable variable** — you cannot change what it points to. `score_ptr` is a variable and can be reassigned.

> **Precision note:** The array name "decays" to a pointer to its first element in most expressions. Exceptions include `sizeof(scores)` (which gives the total array size) and `&scores` (which gives a pointer to the whole array). For now, the simple mental model — "array name means address of first element" — is sufficient.

### Four Equivalent Notations

C++ provides four interchangeable ways to access array elements:

| Notation | Example | Meaning |
|----------|---------|---------|
| Array subscript | `scores[i]` | Element at offset `i` from array start |
| Pointer subscript | `score_ptr[i]` | Element at offset `i` from pointer target |
| Array offset | `*(scores + i)` | Dereference address `scores + i` |
| Pointer offset | `*(score_ptr + i)` | Dereference address `score_ptr + i` |

All four produce identical results. This equivalence is fundamental to C++ and explains why pointer arithmetic works the way it does.

### Why This Matters for Dynamic Arrays

When you allocate an array on the heap:

```cpp
int* arr = new int[100];
```

You receive a pointer to the first element. To access the other elements, you *must* use pointer arithmetic or subscript notation — there is no array name to work with. Understanding the array-pointer equivalence is therefore essential for dynamic memory programming.

---

## 8. Pointer Arithmetic

### Increment and Decrement

Adding 1 to a pointer does **not** add 1 to its raw address value. Instead, it adds the size of the type it points to:

```cpp
int* p = new int[4];  // assume p holds address 1000
// p points to an int (4 bytes on most systems)

p++;  // p now holds address 1004 (next int)
p++;  // p now holds address 1008 (next int)
```

Visually:

```text
Address:  1000   1004   1008
          +------+------+------+
arr:      | a[0] | a[1] | a[2] |
          +------+------+------+
          ^      ^
          p      p+1  (address 1004, next int)
```

This is why pointer arithmetic is type-aware: `p + 1` means "the address of the next element of the type `p` points to."

### Addition and Subtraction

You can add or subtract integers from pointers:

```cpp
int* p = arr;    // points to arr[0]
int* q = p + 2;  // points to arr[2] (2 elements ahead)
int* r = p - 1;  // one element before arr[0]: undefined behavior — do not do this
```

The compiler multiplies the integer operand by the size of the pointee type:

```text
new_address = old_address + (offset * sizeof(pointee_type))
```

> **Warning:** Pointers must remain within the bounds of a valid array (or one past the end). Computing an address outside that range is undefined behavior — even if you never dereference it.

### Pointer Difference

Subtracting one pointer from another (of the same type) gives the number of **elements** between them. Both pointers must point into the **same array** (or one past its end); otherwise the behavior is undefined.

```cpp
int arr[5] = {};
int* p1 = &arr[0];
int* p2 = &arr[3];
std::cout << (p2 - p1);  // 3 (not 12, even if ints are 4 bytes)
```

This is useful for determining array length or relative positions.

### Comparison

Pointers of the same type can be compared with `==` and `!=`; this is well-defined for any two pointers, including comparison with `nullptr`. Relational comparisons (`<`, `>`, `<=`, `>=`) are only well-defined when both pointers point into the **same array** (or one past its end). Comparing pointers to unrelated objects gives an unspecified result, so do not rely on it.

```cpp
int arr[5] = {};
int* p1 = &arr[0];
int* p2 = &arr[0];
int* p3 = &arr[2];

p1 == p2;  // true (same address)
p1 == p3;  // false (different addresses)
p1 < p3;   // true (p1 points to an earlier element of the same array)
```

**Important:** Comparison checks the pointers themselves, not what they point to. Two pointers can point to different variables that happen to have equal values; the pointers will still compare as unequal.

### The `*p++` Idiom

A common pattern in C++ code:

```cpp
while (*p != sentinel) {
    std::cout << *p++ << " ";
}
```

This dereferences `p`, uses the value, and then increments `p` to point to the next element. Due to operator precedence, `*p++` is parsed as `*(p++)`. The post-increment yields the **old** value of `p`, which is then dereferenced; as a side effect, `p` is advanced to the next element by the end of the full expression.

This idiom is efficient and widely used, but it can be confusing. If clarity is more important than brevity, write it as:

```cpp
while (*p != sentinel) {
    std::cout << *p << " ";
    p++;
}
```

Both produce identical behavior.

---

## 9. Const and Pointers

The `const` qualifier gives you fine-grained control over what can be modified through a pointer. There are three distinct cases, and understanding them prevents both bugs and confusion.

### 1. Pointer to Constant

```cpp
const int* p = &x;
```

- **What is const:** The data pointed to (`*p`)
- **What can change:** The pointer itself (`p` can point elsewhere)
- **Cannot do:** `*p = 10;` (modifying the pointee)
- **Can do:** `p = &y;` (changing the pointer)

Read as: "`p` is a pointer to a `const int`."

### 2. Constant Pointer

```cpp
int* const p = &x;
```

- **What is const:** The pointer itself (`p`)
- **What can change:** The data pointed to (`*p` can be modified)
- **Cannot do:** `p = &y;` (changing the pointer)
- **Can do:** `*p = 10;` (modifying the pointee)

Read as: "`p` is a `const` pointer to an `int`."

### 3. Constant Pointer to Constant

```cpp
const int* const p = &x;
```

- **What is const:** Both the pointer and the pointee
- **Cannot do:** Either `*p = 10;` or `p = &y;`

Read as: "`p` is a `const` pointer to a `const int`."

### Why This Matters

`const` is especially useful for function parameters. When you pass a pointer to a function that shouldn't modify the data, declare the parameter as `const`:

```cpp
void display(const int* arr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    // arr[i] = 0;  // ERROR: cannot modify through const pointer
}
```

This documents your intent and lets the compiler catch accidental modifications. It also allows callers to pass pointers to `const` data without casting away constness.

> **Rule of thumb:** Make pointer parameters `const` whenever the function does not need to modify the pointee. This is one of the simplest and most effective habits in C++.

---

## 10. Passing Pointers to Functions

### Indirect Modification via Pointers

C++ can achieve pass-by-reference-like behavior in two ways: using reference parameters (covered later) and using pointer parameters. With pointer parameters, the function receives a copy of the address, but dereferencing that address gives access to the original variable:

```cpp
void double_data(int* int_ptr) {
    *int_ptr *= 2;  // doubles the value at the address
}

int main() {
    int value = 10;
    double_data(&value);  // pass address of value
    std::cout << value;   // prints 20
}
```

The function parameter `int_ptr` is a local variable containing a copy of the address. But the address points to `value` in `main`, so dereferencing it modifies `value`.

### The Swap Example

A classic example of pass-by-reference with pointers:

```cpp
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 100, y = 200;
    swap(&x, &y);
    std::cout << x << " " << y;  // prints 200 100
}
```

The pointers `a` and `b` point to `x` and `y` respectively. Dereferencing them allows the function to exchange the values stored at those addresses.

### Passing Pointers to Collections

You can pass pointers to complex objects like vectors:

```cpp
void display(const std::vector<std::string>* v) {
    for (const auto& s : *v) {  // dereference v to get the vector
        std::cout << s << " ";
    }
}

int main() {
    std::vector<std::string> stooges = {"Larry", "Moe", "Curly"};
    display(&stooges);
}
```

Note the parentheses in `(*v).size()` or `(*v).at(0)` — they are required because the dot operator has higher precedence than the dereference operator. Alternatively, use the arrow operator `v->size()`, which is shorthand for `(*v).size()`.

---

## 11. Returning a Pointer from a Function

### Returning a Pointer to Passed-in Data

It's safe to return a pointer to data that was passed into the function **as long as that data outlives the function call** — which is normally the case when the caller owns the object. The pointer itself is just copied; the pointee must still be alive when the caller uses it:

```cpp
int* largest_int(int* a, int* b) {
    return (*a > *b) ? a : b;
}

int main() {
    int x = 100, y = 200;
    int* largest = largest_int(&x, &y);
    std::cout << *largest;  // prints 200
}
```

### Returning Dynamically Allocated Memory

A very common pattern is to allocate memory inside a function and return a pointer to it:

```cpp
int* create_array(size_t size, int init_value = 0) {
    int* new_storage = new int[size];
    for (size_t i = 0; i < size; ++i) {
        *(new_storage + i) = init_value;  // pointer offset notation
    }
    return new_storage;
}

int main() {
    int* my_array = create_array(100, 20);
    // ... use my_array ...
    delete[] my_array;  // caller is responsible for freeing
}
```

The caller receives ownership of the heap memory and must eventually `delete[]` it.

> **Ownership convention:** By convention, the function that calls `new` should be documented as transferring ownership to the caller. Modern C++ uses `std::unique_ptr` to make this convention explicit and automatic.

### What NOT to Do: Returning Pointers to Local Variables

**Never return a pointer to a local (stack) variable:**

```cpp
int* dont_do_this() {
    int size = 10;
    return &size;  // LOGIC ERROR: size dies when function returns
}
```

The local variable `size` lives on the stack. When the function returns, its activation record is popped, and the memory is reclaimed. The returned pointer now points to **invalid memory** — a **dangling pointer**. Using it produces undefined behavior.

This is **not** a compile error: many compilers accept it without warning (some emit a warning), which makes it especially dangerous. The bug may not manifest immediately; it might corrupt data later when the stack is reused.

---

## 12. Common Pointer Pitfalls

Most pointer bugs fall into a small number of categories. Knowing them by name makes them easier to recognize and avoid.

### 1. Uninitialized Pointers

An uninitialized pointer contains garbage and points "anywhere." Dereferencing it is undefined behavior.

**Solution:** Always initialize pointers to `nullptr` or a valid address.

### 2. Dangling / Stray / Wild Pointers

A dangling pointer points to memory that is no longer valid — either because:

- It points to a local variable that has gone out of scope
- It points to heap memory that has already been `delete`d
- Another pointer aliased the same memory and freed it

**Solution:** Set pointers to `nullptr` after deleting their memory, and never return pointers to local variables.

### 3. Not Handling Allocation Failure

If `new` cannot allocate memory, it throws `std::bad_alloc`. If uncaught, the program terminates. In production code, use exception handling to manage allocation failures gracefully.

### 4. Memory Leaks

Losing the last pointer to dynamically allocated memory makes that memory unreachable and unreclaimable. Over time, leaks exhaust the heap.

**Solution:** Ensure every `new` has a matching `delete`, and use smart pointers (covered in advanced C++) to automate memory management.

### 5. Double Delete

Calling `delete` twice on the same pointer is undefined behavior. After deleting, set the pointer to `nullptr` to make accidental double-deletes through that pointer harmless (`delete nullptr` is a no-op). However, this only protects the pointer variable you reset; if other pointers alias the same memory, they must not be deleted again. Prefer clear ownership and smart pointers to avoid aliasing-related double deletes.

### 6. Out-of-Bounds Access

Dereferencing a pointer outside the bounds of its valid array is undefined behavior. There is no bounds checking — the pointer will happily read or write memory it does not own.

**Solution:** Track array sizes explicitly, prefer `std::vector` and `std::array`, and use iterators or range-based loops when possible.

> **A note on undefined behavior:** "Undefined behavior" means the C++ standard imposes no requirements on what the program does. It might crash, produce wrong results, appear to work, or do something completely unexpected. Never rely on undefined behavior "working" on your compiler — it can change with compiler versions, optimization levels, or even unrelated code changes.

---

## 13. What Is a Reference?

### Definition

A reference is an **alias** for an existing variable. When you use a reference, you are actually using the variable it refers to — no dereferencing syntax required.

```cpp
int num = 100;
int& ref = num;  // ref is an alias for num

std::cout << num;  // 100
std::cout << ref;  // 100 (same variable)
ref = 200;         // modifies num
std::cout << num;  // 200
```

### Key Properties

1. **Must be initialized at declaration.** A reference cannot be unbound.
   ```cpp
   int& ref;  // ERROR: references must be initialized
   ```

2. **Cannot be null.** There is no such thing as a null reference.

3. **Cannot be rebound.** Once initialized, a reference always refers to the same variable.
   ```cpp
   int a = 10, b = 20;
   int& ref = a;
   ref = b;  // assigns the VALUE of b to a; does NOT rebind ref to b
   ```

4. **No dereferencing syntax.** You use a reference exactly like the variable it aliases.

### References in Range-Based For Loops

A common and important use of references is in range-based for loops to avoid unnecessary copying:

```cpp
std::vector<std::string> stooges = {"Larry", "Moe", "Curly"};

// Without reference: makes a copy each iteration
for (auto str : stooges) {
    str = "Funny";  // modifies the copy, not the vector
}

// With reference: aliases each element
for (auto& str : stooges) {
    str = "Funny";  // modifies the actual vector element
}

// With const reference: read-only access, no copying
for (const auto& str : stooges) {
    std::cout << str << " ";  // efficient, safe
}
```

**Guidelines:**

- Use `auto&` when you need to modify elements.
- Use `const auto&` when you only need to read elements (avoids copying, prevents accidental modification).
- Use `auto` (by value) only when you specifically need a copy.

> **Why this matters:** Copying large objects (like `std::string` or `std::vector`) is expensive. A `const auto&` reference lets you read without copying and without risking accidental modification — it is one of the most common and valuable idioms in modern C++.

### References as Function Parameters

References are ideal for pass-by-reference function parameters:

```cpp
void double_data(int& n) {
    n *= 2;  // modifies the caller's variable directly
}

int main() {
    int value = 10;
    double_data(value);
    std::cout << value;  // 20
}
```

Compared to pointer parameters, references are cleaner (no `&` at the call site, no `*` in the function body) and safer (cannot be null).

---

## 14. L-values and R-values

### L-values: Addressable Objects

An **l-value** is an expression that identifies a persistent object or function occupying a memory location. You can normally take its address (bit-fields are a notable exception). Only a **modifiable l-value** — for example, a non-`const` variable — can appear on the left side of an assignment. A `const int` variable and an array name are l-values, but they are not modifiable.

```cpp
int x = 10;        // x is an l-value
std::string name;  // name is an l-value
```

L-values can appear on the left side of an assignment (hence the name).

### R-values: Temporary Values

An **r-value** is any expression that is not an l-value — typically a temporary value or literal. With the built-in `&` operator, you cannot take its address, and with built-in assignment you cannot assign to it.

```cpp
100;                    // numeric literal: an r-value (prvalue)
x + 200;                // temporary result: an r-value (prvalue)
std::string{"Frank"};   // temporary std::string: an r-value (prvalue)
f();                    // call to a function returning by value: an r-value
```

> Note: a string literal such as `"Frank"` is **not** an r-value — it is an l-value of type `const char[6]`. It is the temporary `std::string` constructed from it that is an r-value.

For the **built-in** assignment operator, the left operand must be a modifiable l-value, so an r-value cannot appear on the left (hence the name). Class types can overload `operator=` so that the **left-hand operand** may be an r-value (unless the overload is ref-qualified to accept only l-values); the built-in restriction that the left operand must be a modifiable l-value does not apply to overloaded assignment operators.

### Non-const L-value References Require L-values

**Non-const l-value references** can only bind to l-values. A **const l-value reference** can also bind to an r-value, in which case the temporary's lifetime is extended to the lifetime of the reference:

```cpp
int x = 10;
int& ref1 = x;          // OK: x is an l-value
int& ref2 = 100;        // ERROR: 100 is an r-value
const int& ref3 = 100;  // OK: const l-value reference binds to r-value
```

This is why you cannot pass a literal to a function expecting a **non-const** reference:

```cpp
void square(int& n) { n *= n; }
void print(const int& n) { std::cout << n; }

int num = 10;
square(num);  // OK
square(5);    // ERROR: 5 is an r-value
print(5);     // OK: const l-value reference
```

C++11 introduced **r-value references** (`int&&`) and **move semantics** to address this limitation, but those are advanced topics beyond the scope of this guide. The important thing for now is understanding the fundamental distinction: l-values are addressable, r-values are temporary.

> **Connection to pointers:** The address-of operator `&` requires an l-value because only l-values have a persistent address. This is why `&100` is an error — there is no stable location to point to.

---

## 15. Summary: Pointers vs. References

| Feature | Pointer | Reference |
|---------|---------|-----------|
| What it is | A variable holding an address | An alias for an existing variable |
| Initialization | Can be uninitialized (dangerous) | Must be initialized at declaration |
| Nullability | Can be `nullptr` | Cannot be null |
| Reassignment | Can point to different objects | Cannot be rebound |
| Dereferencing | Explicit (`*p`) | Implicit (use like the variable) |
| Arithmetic | Supported (`p + 1`, `p++`, etc.) | Not supported |
| Typical use | Dynamic allocation, arrays, optional parameters | Function parameters, range-based for loops |

### When to Use Pointers

- When you need to manage dynamically allocated memory
- When you need pointer arithmetic (e.g., traversing a raw array)
- When the "no object" state (`nullptr`) is meaningful
- When working with legacy code or low-level system interfaces

### When to Use References

- For function parameters that should modify the caller's variable
- For function parameters of large objects to avoid copying
- In range-based for loops to avoid copying and/or enable modification
- As a safer, cleaner alternative to pointers when nullability and reassignment are not needed

### Key Takeaways

1. **A pointer is a variable that stores an address.** Everything else follows from this definition.
2. **Always initialize pointers** — to `nullptr` or a valid address.
3. **Understand the difference between the pointer and the pointee.** Changing one does not change the other.
4. **Match `new` with `delete` and `new[]` with `delete[]`.** Never mix them.
5. **Prefer references when you can.** They are safer and cleaner than pointers when nullability and reassignment are not needed.
6. **Use `const` liberally.** It documents intent and lets the compiler catch mistakes.
7. **Beware of undefined behavior.** Dangling pointers, out-of-bounds access, and double deletes are not guaranteed to crash — but they are always bugs.
