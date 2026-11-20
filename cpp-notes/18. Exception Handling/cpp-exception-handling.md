# C++ Exception Handling

## Table of Contents

1. [Why Exception Handling Matters](#1-why-exception-handling-matters)
2. [Core Concepts and Basic Syntax](#2-core-concepts-and-basic-syntax)
3. [Throwing Exceptions from Functions](#3-throwing-exceptions-from-functions)
4. [Handling Multiple Exception Types](#4-handling-multiple-exception-types)
5. [Stack Unwinding](#5-stack-unwinding)
6. [User-Defined Exception Classes](#6-user-defined-exception-classes)
7. [Exceptions in C++ Classes](#7-exceptions-in-c-classes)
8. [The C++ Standard Exception Hierarchy](#8-the-c-standard-exception-hierarchy)
9. [Best Practices and Final Thoughts](#9-best-practices-and-final-thoughts)

---

## 1. Why Exception Handling Matters

Every non-trivial program encounters errors: division by zero, missing files, exhausted memory, invalid user input. The naive approach—scattering `if-else` checks and return-code logic throughout your code—creates three major problems:

1. **Muddled logic**: Error-checking code gets tangled with normal business logic, making the code harder to read and maintain.
2. **Ambiguous return values**: A function returning `0` or `-1` could signal either a valid result or an error. The caller cannot tell the difference without extra documentation or conventions.
3. **Error propagation overhead**: Errors detected deep in the call stack must be manually passed up through every intermediate function, adding boilerplate at every layer.

Exception handling solves these problems by **separating error detection from error handling**. The low-level code that discovers an error simply *throws* an exception; the higher-level code that knows how to recover *catches* and handles it. No intermediate functions need to know about the error at all.

In short: exceptions let each function focus on its own job. A function that detects an error reports it once and moves on; a function that knows how to recover deals with it. Everything in between stays clean.

---

## 2. Core Concepts and Basic Syntax

### 2.1 What Is an Exception?

An exception is an object (or primitive value) that signals an error or abnormal condition has occurred. The exception object typically carries information about what went wrong.

C++ exceptions are designed for **synchronous errors**—errors that happen within the normal flow of execution, such as division by zero or out-of-bounds array access. They are not intended for asynchronous events like hardware interrupts or network signals, which follow different mechanisms.

### 2.2 Common Causes of Exceptions

Exceptional situations arise from many sources:

- **Insufficient resources**: Running out of heap memory, filling all available storage.
- **Missing resources**: Attempting to open a file that does not exist.
- **Invalid operations**: Division by zero, index out of range, numeric overflow/underflow, illegal function arguments.

### 2.3 The Three Keywords: `throw`, `try`, `catch`

C++ exception handling revolves around three keywords:

- `throw`: Used at the point where an error is detected. It creates an exception object and immediately aborts the normal control flow.
- `try`: A block wrapping code that *might* throw an exception. It marks the region where exceptions are to be caught.
- `catch`: A handler block that follows a `try` block. It defines how to handle an exception of a specific type. It only runs when a matching exception is thrown.

The relationship is simple: code inside `try` may `throw`; if it does, control jumps to a matching `catch`.

### 2.4 Basic Example: Division by Zero

We will start with the classic divide-by-zero problem to demonstrate the mechanics.

#### The Problem with Unchecked Division

The behavior of division by zero depends on the data type:

- **Integer division by zero**: Causes undefined behavior; on most systems the program crashes immediately.
- **Floating-point division by zero**: On implementations that support IEC 60559/IEEE 754, it typically produces `inf`, `-inf`, or `nan` (and may raise floating-point exception flags). However, the C++ standard does not guarantee this result on every platform, so you should not rely on floating-point division by zero having portable, well-defined behavior. Even when it produces `inf` or `nan` instead of crashing, those values silently corrupt downstream calculations, so the result is still unacceptable.

Neither outcome is acceptable. We must prevent the division from happening at all.

#### Traditional Fix with `if-else`

The straightforward approach uses a conditional check:

```cpp
#include <iostream>

int main() {
    int miles = 130;
    int gallons = 0;

    if (gallons != 0) {
        double mpg = static_cast<double>(miles) / gallons;
        std::cout << "Miles per gallon: " << mpg << '\n';
    } else {
        std::cerr << "Error: Cannot divide by zero\n";
    }

    std::cout << "Program continues\n";
    return 0;
}
```

This works for simple cases, but it does not scale well when the calculation lives inside a reusable function. The function would have to return a sentinel value, and every caller would have to remember to check it.

#### Fix with Exception Handling

Here is the same logic rewritten with `try`/`throw`/`catch`:

```cpp
#include <iostream>

int main() {
    int miles = 130;
    int gallons = 0;

    try {
        if (gallons == 0) {
            throw 0; // signal error by throwing an integer
        }
        double mpg = static_cast<double>(miles) / gallons;
        std::cout << "Miles per gallon: " << mpg << '\n';
    }
    catch (const int&) {
        std::cerr << "Error: Cannot divide by zero\n";
    }

    std::cout << "Program continues\n";
    return 0;
}
```

#### Execution Flow Explained

- **Normal path (no exception)**: If `gallons` is non-zero, all code inside the `try` block runs normally. The `catch` block is skipped entirely, and execution continues after it.
- **Error path (exception thrown)**:
  1. When `throw 0` executes, all remaining code inside the `try` block is skipped—the division never happens.
  2. The C++ runtime searches for a `catch` handler whose parameter type matches the type of the thrown value.
  3. When a match is found, the `catch` body runs.
  4. After the `catch` block finishes, execution continues normally after the catch chain. The program does not crash.

> **Note**: This trivial example is for syntax demonstration only. For simple inline checks, an `if-else` is usually cleaner. The real value of exceptions emerges when errors are detected inside functions and handled elsewhere.

---

## 3. Throwing Exceptions from Functions

### 3.1 Why Throw Exceptions from Functions?

Consider a function that computes an average:

```cpp
double calculate_average(int sum, int count) {
    // If count == 0, this is floating-point division by zero:
    // it typically yields inf/nan and does NOT throw a C++ exception.
    return static_cast<double>(sum) / count;
}
```

What should this function return when `count` is zero?

- Return `0`? Zero could be a perfectly valid average.
- Return `-1`? Same problem—negative averages can be valid.
- Return a `bool` for success and write the result into a reference parameter? That makes the function awkward to use and breaks natural call chaining.

Exceptions solve this cleanly: **the function keeps its natural return type, and errors are reported through a separate channel**. The function does not need to decide how to recover; it just signals the problem and lets the caller decide.

### 3.2 Example: Function That Throws

Here we rewrite the miles-per-gallon calculation as a function that throws on error:

```cpp
#include <iostream>

double calculate_mpg(int miles, int gallons) {
    if (gallons == 0) {
        throw 0;
    }
    return static_cast<double>(miles) / gallons;
}

int main() {
    int miles = 120;
    int gallons = 0;

    try {
        double mpg = calculate_mpg(miles, gallons);
        std::cout << "Miles per gallon: " << mpg << '\n';
    }
    catch (const int&) {
        std::cerr << "Error: Tried to divide by zero\n";
    }

    std::cout << "Bye\n";
    return 0;
}
```

The function `calculate_mpg` has a single, clear return type (`double`). The error is reported separately, so callers are never forced to guess whether a return value is "real" or an error code.

### 3.3 Uncaught Exceptions

If a function throws an exception and no matching `catch` handler exists anywhere on the call stack, the C++ runtime calls `std::terminate()`, and the program aborts immediately. This is important: exceptions are not silently ignored.

> **Key idea**: An exception travels upward through the function call stack until it finds a handler or reaches the top of `main`. This upward search process is part of **stack unwinding**, and it is the subject of Section 5.

---

## 4. Handling Multiple Exception Types

### 4.1 Why Multiple Exception Types?

A single function can fail for multiple reasons. Using a different exception type for each failure mode lets callers handle each error appropriately—or deliberately handle only some of them.

For `calculate_mpg`, there are at least two distinct error conditions:

1. `gallons` equals zero → division by zero.
2. `miles` or `gallons` is negative → invalid input.

We can throw a different type for each case.

### 4.2 Multiple Catch Blocks

You can chain multiple `catch` blocks after one `try` block, each handling one exception type:

```cpp
#include <iostream>
#include <string>

double calculate_mpg(int miles, int gallons) {
    if (gallons == 0) {
        throw 0;
    }
    if (miles < 0 || gallons < 0) {
        throw std::string{"Negative value error"};
    }
    return static_cast<double>(miles) / gallons;
}

int main() {
    int miles = -10;
    int gallons = 5;

    try {
        double mpg = calculate_mpg(miles, gallons);
        std::cout << "Miles per gallon: " << mpg << '\n';
    }
    catch (const int&) {
        std::cerr << "Error: Cannot divide by zero\n";
    }
    catch (const std::string& ex) {
        std::cerr << "Error: " << ex << '\n';
    }

    std::cout << "Bye\n";
    return 0;
}
```

> **Note**: Throwing `int` or `std::string` here is only for demonstrating multiple exception types. In real code, prefer custom exception classes or standard exception types, as discussed in Section 6.

The first matching handler wins. The order of `catch` blocks matters: more specific handlers should come before more general ones.

### 4.3 Catch-All Handler

To catch any exception whatsoever as a fallback, use `catch(...)`:

```cpp
catch (...) {
    std::cerr << "Error: An unknown exception occurred\n";
}
```

Rules for catch-all:

- It must appear **last** in the sequence of catch blocks.
- You cannot access the exception object inside it; you only know that *some* exception was thrown.
- It is useful as a safety net to prevent program termination from unexpected exception types.

---

## 5. Stack Unwinding

### 5.1 What Is Stack Unwinding?

When an exception is thrown, the C++ runtime walks up the function call stack looking for a matching `catch` handler. As it leaves each function, it destroys all local objects with automatic storage duration in that function (calling their destructors) and pops the function off the stack. This process is called **stack unwinding**.

The phrase describes exactly what happens: the stack of active function calls is "unwound" one frame at a time until a handler is found.

### 5.2 Step-by-Step Mechanism

Suppose the call chain is `main` → `func_a` → `func_b` → `func_c`, and `func_c` throws an `int` exception:

1. `func_c` throws the exception. Since `func_c` has no matching handler, it begins to exit: all local objects are destroyed, and `func_c` is popped from the stack.
2. Control returns to `func_b`. If the call to `func_c` was inside a `try` block with a matching `catch`, the handler runs. If not, `func_b` is also destroyed and popped.
3. The process repeats for `func_a` and then `main`.
4. If no handler is found by the time `main` is reached, `std::terminate()` is called.

### 5.3 Concrete Demonstration

```cpp
#include <iostream>

void func_c() {
    std::cout << "Starting func_c\n";
    throw 100;
    std::cout << "Ending func_c\n"; // NEVER executes
}

void func_b() {
    std::cout << "Starting func_b\n";
    func_c();
    std::cout << "Ending func_b\n"; // NEVER executes if not caught here
}

void func_a() {
    std::cout << "Starting func_a\n";
    func_b();
    std::cout << "Ending func_a\n"; // NEVER executes if not caught here
}

int main() {
    std::cout << "Starting main\n";
    try {
        func_a();
    }
    catch (const int&) {
        std::cout << "Caught error in main\n";
    }
    std::cout << "Finishing main\n";
    return 0;
}
```

**Output**:

```
Starting main
Starting func_a
Starting func_b
Starting func_c
Caught error in main
Finishing main
```

None of the "Ending" lines print, because the exception interrupts normal function return.

If we move the handler into `func_b`:

```cpp
void func_b() {
    std::cout << "Starting func_b\n";
    try {
        func_c();
    }
    catch (const int&) {
        std::cout << "Caught error in func_b\n";
    }
    std::cout << "Ending func_b\n"; // now this runs
}
```

**Output**:

```
Starting main
Starting func_a
Starting func_b
Starting func_c
Caught error in func_b
Ending func_b
Ending func_a
Finishing main
```

Once caught, normal execution resumes inside the function that handled the exception. This is the key behavioral difference: the exception stops propagating as soon as a matching handler is found.

### 5.4 Critical Implication: Resource Safety

Stack unwinding automatically calls destructors for automatic local objects. However, if you use raw `new`/`delete` to manage memory, the `delete` statement may be skipped by an exception, causing a **memory leak**.

Consider this problematic pattern:

```cpp
void risky() {
    int* data = new int[1000];
    might_throw();          // if this throws, delete[] is skipped
    delete[] data;
}
```

**The fix**: Use **RAII** (Resource Acquisition Is Initialization) classes such as `std::unique_ptr`, `std::vector`, and `std::fstream`. Their destructors always release resources, even when exceptions occur.

```cpp
void safe() {
    std::vector<int> data(1000);
    might_throw();          // vector's destructor always runs
}
```

> **Rule of thumb**: Always use RAII types to manage resources. This is the foundation of all exception-safe code.

---

## 6. User-Defined Exception Classes

### 6.1 Why Create Custom Exception Types?

Throwing integers or strings works, but it has major weaknesses:

- **Poor semantics**: `throw 0` tells you nothing about *what kind* of error occurred.
- **Type collisions**: Unrelated errors might accidentally use the same primitive type and get caught by the wrong handler.
- **Limited information**: Primitive types cannot carry structured error details.

**Industry best practice**: Throw objects, not primitive types. Custom exception classes make error types explicit and can carry as much context as needed.

### 6.2 Defining Simple Exception Classes

We define one class per error category:

```cpp
class DivideByZeroException {};
class NegativeValueException {};
```

Even empty classes are useful: the *type name itself* fully describes the error. The class name becomes the error message.

### 6.3 Throwing and Catching Custom Exceptions

```cpp
#include <iostream>

class DivideByZeroException {};
class NegativeValueException {};

double calculate_mpg(int miles, int gallons) {
    if (gallons == 0) {
        throw DivideByZeroException{}; // throw by value
    }
    if (miles < 0 || gallons < 0) {
        throw NegativeValueException{}; // throw by value
    }
    return static_cast<double>(miles) / gallons;
}

int main() {
    int miles = 120;
    int gallons = 0;

    try {
        double mpg = calculate_mpg(miles, gallons);
        std::cout << "Miles per gallon: " << mpg << '\n';
    }
    catch (const DivideByZeroException&) {
        std::cerr << "Error: You cannot divide by zero\n";
    }
    catch (const NegativeValueException&) {
        std::cerr << "Error: One of your parameters is negative\n";
    }

    std::cout << "Bye\n";
    return 0;
}
```

### 6.4 Guideline: Throw by Value, Catch by Reference

- **Throw by value**: The `throw` expression initializes an exception object in storage managed by the runtime; that object survives stack unwinding. Copy elision may apply, so a copy is not guaranteed to occur. The original object's lifetime is irrelevant to the handler.
- **Catch by `const` reference**: Avoids unnecessary copies, prevents object slicing in inheritance hierarchies, and respects the fact that you usually do not modify the exception.

This pair of rules is called the *canonical pattern* for C++ exception handling. Follow it consistently.

---

## 7. Exceptions in C++ Classes

Exceptions work with class member functions, constructors, and destructors—but each has very different rules.

### 7.1 Ordinary Member Functions

Regular member functions follow exactly the same rules as free functions: throw when you detect an error, let the caller handle it.

### 7.2 Constructors

Constructors are where exceptions shine:

- Constructors have no return value, so error codes are not an option.
- If construction cannot complete successfully, throwing an exception is the cleanest way to signal failure. No partially-constructed object is left behind.

**Example: Account class constructor**

```cpp
#include <iostream>
#include <memory>

class IllegalBalanceException {};

class Account {
private:
    double balance;
public:
    explicit Account(double balance) : balance{balance} {
        if (balance < 0) {
            throw IllegalBalanceException{};
        }
    }
};

class CheckingAccount : public Account {
public:
    using Account::Account;
};

int main() {
    try {
        auto moes_account = std::make_unique<CheckingAccount>(-100.0);
        std::cout << "Account created successfully\n";
    }
    catch (const IllegalBalanceException&) {
        std::cerr << "Error: Could not create account - negative balance\n";
    }

    std::cout << "Program completed successfully\n";
    return 0;
}
```

When a constructor throws, the base-class and member subobjects that were already fully constructed are destroyed in reverse order, and the `new` expression automatically releases the allocated storage. Note that the destructor of the object whose constructor threw is **not** called, because that object was never fully constructed. In the example above, the `Account` constructor body throws before `CheckingAccount` or `Account` is complete, so neither destructor runs. Also note that the smart pointer is not yet constructed at this point—`std::make_unique` has not returned—so it does not perform the cleanup; the language itself guarantees it. This is exactly why RAII types are essential in exception-aware code: once a smart pointer *does* exist, its destructor will always run during stack unwinding.

### 7.3 Destructors

> **Non-negotiable rule**: **Never let exceptions escape a destructor.**

Reasons:

- Since C++11, destructors have an implicit exception specification. It is usually `noexcept(true)`, but it becomes `noexcept(false)` if any base-class or non-static data member destructor is potentially throwing. A destructor can also be explicitly declared `noexcept(false)`. Even when the specification permits throwing, letting an exception escape a destructor is almost always a design error.
- If a destructor throws during stack unwinding while another exception is already active, the program immediately calls `std::terminate()`. C++ cannot handle two simultaneous exceptions.

All cleanup work in a destructor must be done silently. If a cleanup operation can fail, handle it inside the destructor and never let the exception propagate out.

---

## 8. The C++ Standard Exception Hierarchy

### 8.1 The `std::exception` Base Class

The C++ standard library defines a built-in inheritance tree of exception types, most of which are derived from `std::exception` (declared in `<exception>`). The header `<stdexcept>` provides commonly used derived types such as `std::logic_error` and `std::runtime_error`; `std::bad_alloc` is declared in `<new>`.

The base class provides one key virtual method:

```cpp
virtual const char* what() const noexcept;
```

It returns a human-readable null-terminated string describing the error.

Common standard derived classes include:

- `std::logic_error`: Errors that could, in principle, be detected before runtime (e.g., `std::out_of_range`, `std::invalid_argument`).
- `std::runtime_error`: Errors that can only be detected during execution (e.g., `std::overflow_error`, `std::range_error`).
- `std::bad_alloc`: Thrown by `operator new` / `operator new[]` when memory allocation fails.

### 8.2 Benefits of Inheriting from `std::exception`

Deriving your custom exceptions from `std::exception` gives you two advantages:

1. **Interoperability**: Your exceptions can be caught by generic `catch (const std::exception&)` handlers, just like standard library exceptions.
2. **Polymorphic error messages**: You can use the `what()` interface uniformly for all exceptions.

### 8.3 Example: Custom Exception with `std::exception`

```cpp
#include <iostream>
#include <exception>
#include <memory>

class IllegalBalanceException : public std::exception {
public:
    IllegalBalanceException() = default;
    ~IllegalBalanceException() override = default;

    const char* what() const noexcept override {
        return "Illegal balance exception: balance cannot be negative";
    }
};

class Account {
private:
    double balance;
public:
    explicit Account(double balance) : balance{balance} {
        if (balance < 0) {
            throw IllegalBalanceException{};
        }
    }
};

int main() {
    try {
        auto moes_account = std::make_unique<Account>(-100.0);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
    }
    return 0;
}
```

The `noexcept` qualifier on `what()` is required: it promises that the function itself will not throw. This is part of the `std::exception` contract.

---

## 9. Best Practices and Final Thoughts

### 9.1 Best Practices

1. **Throw objects, not primitives**. Use custom classes or standard exception types for clear semantics and type safety.
2. **Throw by value, catch by `const` reference**. This is the canonical, safe pattern in C++.
3. **Never throw from destructors**. It is the fastest way to crash during stack unwinding.
4. **Manage all resources with RAII**. Smart pointers, containers, and RAII wrappers eliminate resource leaks in the presence of exceptions.
5. **Reserve exceptions for exceptional situations**. Do not use them for ordinary control flow.

### 9.2 The Exception Debate

There is no universal consensus on how heavily to use exceptions in C++. Three major schools of thought exist:

- **Minimalist**: Use exceptions only for fatal, unrecoverable errors.
- **No-exceptions**: Avoid exceptions entirely, use error codes everywhere. Common in games, embedded systems, and other latency-sensitive domains.
- **Liberal**: Use exceptions for most error conditions.

Which approach is right depends on your project domain, performance constraints, and team conventions. The goal of this guide is to give you the foundational knowledge to work effectively in any codebase.

### 9.3 Key Takeaways

- Exceptions separate **detection** (throw) from **handling** (catch), keeping code clean.
- The runtime searches for a matching handler by **unwinding the stack**, calling destructors along the way.
- **RAII** is the foundation of exception safety—use it everywhere.
- **Custom exception classes** communicate error meaning through type names and structured data.
- **Never throw from destructors**; handle failures internally.
- **Inherit from `std::exception`** for interoperability and a uniform `what()` interface.

With these principles in hand, you can write C++ code that handles errors gracefully, safely, and readably.
