# C++ Lambda Expressions

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. Motivation: Why C++ Needed Lambda Expressions](#2-motivation-why-c-needed-lambda-expressions)
- [3. Structure and Syntax of a Lambda Expression](#3-structure-and-syntax-of-a-lambda-expression)
- [4. Stateless Lambda Expressions](#4-stateless-lambda-expressions)
- [5. Stateful Lambda Expressions and Closures](#5-stateful-lambda-expressions-and-closures)
- [6. Lambdas with STL Algorithms](#6-lambdas-with-stl-algorithms)
- [7. Quick Reference Summary](#7-quick-reference-summary)

---

## 1. Introduction

Introduced in **C++11** and expanded in **C++14**, **C++17**, and **C++20**, **lambda expressions** provide a concise way to define small, inline callable behavior directly where it is needed. Rather than writing a separate named function or a full class definition far away from the call site, you can express short operations right at the point of use.

This tutorial is self-contained and written for beginners who want to understand both **how** to use lambda expressions and **why** they work the way they do. Each section builds on the previous one:

1. **Motivation and Function Objects:** Why C++ introduced lambdas, how callable objects with `operator()` work, and why they matter for understanding lambdas.
2. **Structure and Syntax:** Every part of a lambda expression, when elements are optional, and why `auto` is required when storing a lambda in a variable.
3. **Stateless Lambda Expressions:** Lambdas with an empty capture list `[]`, parameter passing modes, generic `auto` parameters, predicates, and returning lambdas from functions.
4. **Stateful Lambda Expressions and Closures:** How the compiler generates a closure class, how capture modes and `mutable` behave, how default/mixed captures work, and how to capture `this` safely.
5. **Lambdas with STL Algorithms:** How unary/binary predicates and operations connect with Standard Template Library (STL) algorithms in practical code.
6. **Quick Reference Summary:** A concise lookup table of core concepts and rules.

---

## 2. Motivation: Why C++ Needed Lambda Expressions

### Life Before C++11: Functions and Function Objects

Many C++ functions — especially algorithms in the Standard Template Library (STL) — accept custom behavior as an argument. For example, a sorting algorithm needs to know *how* to compare two elements, and a transformation algorithm needs to know *what* operation to apply to each element.

Before C++11, developers primarily passed custom behavior using two mechanisms:

* **Function pointers:** Pointers that store the address of a regular function and invoke it when called. They are simple and familiar, but a plain function pointer cannot carry its own stored state (such as a configurable threshold or multiplier) unless you rely on global or `static` variables.
* **Function objects (often called *functors*):** Instances of a `class` or `struct` that overload the **function call operator**, `operator()`.

Overloading `operator()` allows an object to be called using parentheses `()` just like a function. If `square` is an object of a type that defines `int operator()(int x)`, then writing `square(4)` is shorthand for calling `square.operator()(4)`. Because a function object is a class instance, it can store state in data members while still acting like a callable function.

### Why Function Objects Alone Were Not Enough for Small Tasks

Function objects remain useful in C++, but writing a separate class or struct for every small, localized operation creates friction in everyday code:

1. **Separation from the call site:** Helper classes and small functions are often placed elsewhere in a file or header, forcing the reader to jump back and forth to understand what a single algorithm call actually does.
2. **Boilerplate overhead:** Defining a class or struct with a constructor, private member variables, and `operator()` for minor variations (such as multiplying by `10`, `30`, or `100`) adds repetitive code that obscures the main logic.
3. **Inlining and optimization:** Keeping small callable logic right at the call site makes it straightforward for the compiler to inline the call and optimize the surrounding code.

### Example: Function Object vs. Lambda Expression

To see the difference in practice, consider multiplying every element in an `std::vector<int>` by `100` using `std::transform`:

```cpp
#include <algorithm>
#include <iostream>
#include <vector>

class Multiplier {
private:
    int factor{};

public:
    explicit Multiplier(int value) : factor{value} {}

    int operator()(int n) const {
        return n * factor;
    }
};

int main() {
    std::vector<int> values{1, 2, 3, 4};

    // 1. Using a named function object
    Multiplier mult_by_100{100};
    std::transform(values.begin(), values.end(), values.begin(), mult_by_100);

    for (int value : values) {
        std::cout << value << " ";
    }
    std::cout << "\n";  // Output: 100 200 300 400

    // 2. Resetting the vector and performing the same operation inline with a lambda
    values = {1, 2, 3, 4};
    std::transform(values.begin(), values.end(), values.begin(), [](int n) {
        return n * 100;
    });

    for (int value : values) {
        std::cout << value << " ";
    }
    std::cout << "\n";  // Output: 100 200 300 400
    return 0;
}
```

### Choosing Between a Lambda and a Function Object

Lambdas complement function objects rather than replacing them completely:

- **Prefer a lambda expression** when the behavior is short — typically one or a few statements — and is needed only at a specific call site.
- **Prefer a named function object (or a regular named function)** when the logic is substantial, needs a clear domain name, or is reused across many parts of a program.

Understanding function objects pays off directly because **the C++ compiler implements every lambda expression by generating an unnamed class (called a closure type) with an overloaded `operator()`**. Lambdas are a cleaner way to write callable objects without manually authoring the boilerplate class yourself.

---

## 3. Structure and Syntax of a Lambda Expression

Every lambda expression follows a predictable grammatical structure:

```cpp
[ capture-list ] ( parameter-list ) specifiers -> return-type {
    // function body
}
```

Each component serves a distinct role:

1. **Capture List `[ ... ]` (Required):** Starts the lambda expression and controls whether and how variables from the enclosing scope are captured. An empty capture list `[]` captures nothing from the surrounding scope.
2. **Parameter List `( ... )` (Optional when empty):** Specifies the arguments passed when the lambda is called, similar to a function's parameter list. If the lambda takes no parameters and has no specifiers or explicit return type, you may omit `()` (for example, `[]{ std::cout << "Hello\n"; }`).
3. **Specifiers (Optional):** Keywords placed after the parameter list that modify how the lambda behaves — most notably `mutable` (covered in Section 5), `constexpr`, or `noexcept` (C++17+).
4. **Return Type `-> Type` (Optional):** Explicitly declares the return type. When omitted, the compiler deduces the return type from `return` statements (or deduces `void` if there is no `return` statement). Providing an explicit return type is helpful when you want to enforce a specific return type or when deduction is unclear.
5. **Body `{ ... }` (Required):** Contains the statements executed whenever the lambda is invoked.

### Immediately Invoked Lambdas vs. Stored Lambdas

You can call a lambda immediately by appending argument parentheses `()` right after its closing brace `}`, or you can store the lambda object in a variable and call it later:

```cpp
#include <iostream>

int main() {
    // Immediately invoked lambda with no parameters
    []() {
        std::cout << "Hello from an immediate lambda\n";
    }();

    // Immediately invoked lambda with parameters
    [](int x, int y) {
        std::cout << "Sum: " << (x + y) << "\n";
    }(100, 200);

    // Storing a lambda in a named variable using auto
    auto add = [](int x, int y) {
        return x + y;
    };

    std::cout << "Result: " << add(10, 20) << "\n";  // Output: Result: 30
    return 0;
}
```

#### Why `auto` Is Required When Storing a Lambda

Each lambda expression in a program has its **own unique, unnamed class type** generated by the compiler at compile time. Because no programmer can write that compiler-generated type name explicitly, you use `auto` to let the compiler deduce the exact type of the lambda object.

---

## 4. Stateless Lambda Expressions

A **stateless lambda expression** has an **empty capture list `[]`**. Because it captures no local variables from the surrounding function or block, its behavior depends only on:

* Entities already reachable by normal C++ scope rules (such as global variables, named functions, or types), and
* Arguments explicitly passed into its **parameter list** at each call.

### Passing Parameters: By Value, By Reference, and By Pointer

Lambda parameters follow the same argument-passing rules as regular functions:

* **By value (`T param`):** Copies the argument into the parameter. Modifying `param` inside the lambda changes only that local copy, leaving the caller's variable untouched. For class types, the copy constructor runs.
* **By reference (`T& param` or `const T& param`):** Binds the parameter directly to the caller's object without copying it. A non-const reference (`T&`) allows the lambda to modify the caller's variable or object; a `const T&` avoids copying while preventing modification through the reference.
* **By pointer (`T* param`):** Receives the memory address of an object (typically passed with `&obj`) and dereferences it (`*param`) to read or modify the pointee.

```cpp
#include <iostream>
#include <string>

class Person {
private:
    std::string name{};
    int age{};

public:
    Person(std::string n, int a) : name{std::move(n)}, age{a} {}

    void set_name(const std::string& n) { name = n; }
    void set_age(int a) { age = a; }
    const std::string& get_name() const { return name; }
    int get_age() const { return age; }
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.get_name() << " (" << p.get_age() << ")";
    return os;
}

int main() {
    int num1{100};
    int num2{100};

    // x is a reference (modifies caller's num1); y is a copy (leaves caller's num2 unchanged)
    auto modify_demo = [](int& x, int y) {
        x = 1000;
        y = 2000;
    };

    modify_demo(num1, num2);
    std::cout << "num1: " << num1 << ", num2: " << num2 << "\n";
    // Output: num1: 1000, num2: 100

    // Read-only access to a class object without copying
    Person member{"Larry", 18};
    auto display_person = [](const Person& p) {
        std::cout << p << "\n";
    };
    display_person(member);  // Output: Larry (18)

    // Modifying a class object in place through a non-const reference
    auto update_person = [](Person& p) {
        p.set_name("Frank");
        p.set_age(25);
    };
    update_person(member);
    std::cout << "Updated member: " << member << "\n";
    // Output: Updated member: Frank (25)

    return 0;
}
```

### Generic Lambdas with `auto` Parameters (C++14+)

Introduced in **C++14**, using `auto` in a lambda's parameter list creates a **generic lambda**. Rather than requiring a fixed type upfront, `auto` tells the compiler to deduce each parameter's type from the arguments supplied at each call site (conceptually equivalent to a templated `operator()`).

```cpp
#include <iostream>

int main() {
    auto print_pair = [](const auto& first, const auto& second) {
        std::cout << first << " and " << second << "\n";
    };

    print_pair(10, 20);            // two int values
    print_pair(100.5, 200);        // double and int values
    print_pair("Larry", "Curly");  // two string literals

    return 0;
}
```

Keep in mind that any expression inside the lambda body (such as `<<`) must be valid for the deduced types; if an operation is unsupported for a given type combination, the compiler rejects the call during compilation.

### Predicates and Returning Lambdas from Functions

A **predicate** is a callable (such as a function, function object, or lambda) that accepts one or more arguments and returns a `bool` (`true` or `false`). **Predicate lambdas** are especially useful because they separate *the condition being tested* from *the control flow that iterates or searches* over data.

A function can also **return a lambda** — most cleanly by using `auto` as the function's return type (C++14+) so the caller receives the exact closure type without type-erasure overhead:

```cpp
#include <iostream>
#include <vector>

template <typename Predicate>
void filter_vector(const std::vector<int>& values, Predicate pred) {
    std::cout << "[ ";
    for (int value : values) {
        if (pred(value)) {
            std::cout << value << " ";
        }
    }
    std::cout << "]\n";
}

auto make_greeter() {
    return []() {
        std::cout << "Hello from a returned lambda!\n";
    };
}

int main() {
    std::vector<int> numbers{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    // Different predicate lambdas customize filtering while filter_vector handles the loop
    filter_vector(numbers, [](int x) { return x > 50; });
    filter_vector(numbers, [](int x) { return x <= 30; });
    filter_vector(numbers, [](int x) { return x >= 30 && x <= 60; });

    auto greeter = make_greeter();
    greeter();

    return 0;
}
```

---

## 5. Stateful Lambda Expressions and Closures

### What Happens Under the Hood?

A **stateful lambda expression** has a **non-empty capture list**.
It captures automatic (local) variables from its enclosing scope so the lambda body can use them without passing them as arguments on every call.

When you write a stateful lambda, the compiler performs two key steps:

1. It generates an unnamed class — the **closure type** — and adds a **data member** for each captured local variable.
2. When the lambda expression is evaluated (for example, when initializing a variable with `auto fn = [...] { ... };`), the compiler constructs a **closure object** and initializes those data members from the enclosing variables at that moment.

Calling the lambda variable simply calls `operator()` on that closure object.
Because capture initialization happens when the closure object is **created**, subsequent calls to the lambda reuse its stored members rather than re-reading the outer variables' initial values.

> **Scope Rule for Captures:** Capture lists apply to **local (automatic) variables** in the enclosing scope. Global variables and `static` variables are already accessible via normal scope rules and should not be placed in a capture list.

### Capture by Value, Capture by Reference, and `mutable`

There are two fundamental ways to capture a local variable `x`:

1. **Capture by value (`[x]`):** Copies `x` into a data member of the closure object. By default, the closure's `operator()` is `const`, which prevents modifying captured copies inside the lambda body. Adding the **`mutable`** keyword removes `const` from `operator()`, allowing the lambda to modify its own internal copy:
   ```cpp
   auto increment_copy = [x]() mutable {
       x += 100;  // Modifies only the lambda's internal copy of x
   };
   ```
   Even with `mutable`, modifying a by-value capture changes only the closure's internal copy — the caller's original variable `x` remains unchanged.

2. **Capture by reference (`[&x]`):** Stores a reference to the caller's variable `x` inside the closure. Modifying `x` inside the lambda modifies the caller's variable `x` directly. Because the reference itself is not being rebound, modifying the referenced object does not require `mutable`. However, you must ensure the referenced variable **outlives** the lambda; otherwise, calling the lambda later produces a dangling reference and undefined behavior.

### Default Captures and Mixed Captures

When a lambda uses several local variables, a **default capture** at the start of the capture list can save repetition:

* `[=]`: Default to **capture by value** for local variables used inside the lambda body.
* `[&]`: Default to **capture by reference** for local variables used inside the lambda body.

You can combine a default capture with explicit exceptions by placing the default capture **first** and specifying the **opposite** capture mode for the named variables:

* `[=, &y]`: Capture referenced local variables by value by default, except capture `y` by reference.
* `[&, x, z]`: Capture referenced local variables by reference by default, except capture `x` and `z` by value.

Compilers capture only the local variables actually referenced in the lambda body and may warn if you name a variable in the capture list that the body never uses.

```cpp
#include <iostream>

int main() {
    int x{100};
    int y{200};
    int z{300};

    // Default by value (with mutable so copies can change), except y by reference
    auto mixed_by_val = [=, &y]() mutable {
        x += 100;  // modifies internal copy of x
        y += 100;  // modifies outer y
        z += 100;  // modifies internal copy of z
        std::cout << "Inside [=, &y]  -> x: " << x << ", y: " << y << ", z: " << z << "\n";
    };

    mixed_by_val();
    std::cout << "Outside [=, &y] -> x: " << x << ", y: " << y << ", z: " << z << "\n";

    // Reset outer variables and demonstrate default by reference with explicit value captures
    x = 100;
    y = 200;
    z = 300;

    auto mixed_by_ref = [&, x, z]() mutable {
        x += 100;  // modifies internal copy of x
        y += 100;  // modifies outer y
        z += 100;  // modifies internal copy of z
        std::cout << "Inside [&, x, z]  -> x: " << x << ", y: " << y << ", z: " << z << "\n";
    };

    mixed_by_ref();
    std::cout << "Outside [&, x, z] -> x: " << x << ", y: " << y << ", z: " << z << "\n";

    return 0;
}
```

### Capturing `this` Inside Member Functions

Inside a non-static member function of a class, data members and member functions are accessed through the implicit `this` pointer. To access class members from a lambda inside a member function, capture `this` (`[this]`).

* **Best practice:** Write **`[this]`** explicitly rather than relying on `[=]` to capture member access. In C++20, implicitly capturing `this` via `[=]` is deprecated because `[=]` suggests copying values, whereas capturing `this` actually captures a pointer to the enclosing object. Being explicit makes the lifetime dependency on the object clear, and you must ensure the object outlives the lambda — otherwise calling it later causes undefined behavior due to a dangling pointer (C++17 also supports `[*this]` to copy the enclosing object by value when needed).

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

class Person {
private:
    std::string name{};
    int age{};

public:
    Person(std::string n, int a) : name{std::move(n)}, age{a} {}
    const std::string& get_name() const { return name; }
    int get_age() const { return age; }
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.get_name() << " (" << p.get_age() << ")";
    return os;
}

class People {
private:
    std::vector<Person> people{};
    int max_people{10};

public:
    explicit People(int max = 10) : max_people{max} {}

    void add(const std::string& name, int age) {
        people.emplace_back(name, age);
    }

    void set_max_people(int max) {
        max_people = max;
    }

    std::vector<Person> get_people(int min_age) const {
        std::vector<Person> result{};
        int matched_count{0};

        // Captures 'this' to read max_people, 'matched_count' by reference to tally matches,
        // and 'min_age' by value
        std::copy_if(
            people.begin(),
            people.end(),
            std::back_inserter(result),
            [this, &matched_count, min_age](const Person& p) {
                if (p.get_age() > min_age && ++matched_count <= max_people) {
                    return true;
                }
                return false;
            }
        );

        return result;
    }
};

int main() {
    People group{};
    group.add("Larry", 18);
    group.add("Curly", 25);
    group.add("Moe", 30);
    group.add("Frank", 42);
    group.add("James", 55);

    std::vector<Person> adults = group.get_people(17);
    std::cout << "Up to default max (10) over age 17:\n";
    for (const auto& p : adults) {
        std::cout << "  " << p << "\n";
    }

    group.set_max_people(3);
    std::vector<Person> limited_adults = group.get_people(17);
    std::cout << "Up to max (3) over age 17:\n";
    for (const auto& p : limited_adults) {
        std::cout << "  " << p << "\n";
    }

    return 0;
}
```

---

## 6. Lambdas with STL Algorithms

One of the primary reasons lambdas are so valuable in C++ is that **STL algorithms** (`<algorithm>` and `<numeric>`) are designed to accept callables for custom comparisons, conditions, and element operations. When reading algorithm specifications, two distinctions come up constantly:

* **Predicate vs. Operation:** A **predicate** returns `bool` to test a condition (such as whether an element matches a rule or whether one element precedes another). An **operation** performs an action or computes a transformed value for an element.
* **Unary vs. Binary:** A **unary** callable takes **1** argument (typically a single element); a **binary** callable takes **2** arguments (typically two elements being compared or combined).

### Common STL Patterns with Lambdas

1. **`std::for_each` (Unary Operation):** Applies a callable to each element in `[first, last)`. It does not alter the container structure itself, though a non-const reference parameter allows modifying elements in place.
2. **`std::is_permutation` (Binary Predicate):** Checks whether two sequences contain the same elements (possibly in a different order), using a binary predicate to determine when two elements count as equivalent (note that it can take up to $O(N^2)$ time on large inputs).
3. **`std::transform` (Unary Operation):** Applies a callable to each element in an input range and writes the returned values to an output range.
4. **`std::remove_if` + `erase` ("Erase-Remove Idiom" with Unary Predicate):** `std::remove_if` moves elements that *do not* satisfy the predicate toward the beginning of the range and returns an iterator marking the new logical end; calling `.erase(new_end, container.end())` actually erases the leftover elements from the container.
5. **`std::sort` (Binary Predicate):** Reorders elements in place using a strict weak ordering binary predicate that returns `true` when the first argument should appear before the second.
6. **`std::all_of` and `std::none_of` (Unary Predicates):** `std::all_of` returns `true` only if *every* element in the range satisfies the predicate; `std::none_of` returns `true` only if *no* element satisfies the predicate. (In the example below, we also use `std::iota` from `<numeric>` to fill a vector with consecutive ascending integers.)
7. **Nested Predicates Inside a Member Function:** Combines `std::all_of` and `std::none_of` with `[this]` inside `PasswordValidator::is_valid` to verify that no character in a password matches any restricted symbol.

### Complete Example: STL Algorithms in Action

```cpp
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

struct Point {
    int x{};
    int y{};
};

class Person {
private:
    std::string name{};
    int age{};

public:
    Person(std::string n, int a) : name{std::move(n)}, age{a} {}
    const std::string& get_name() const { return name; }
    int get_age() const { return age; }
};

std::ostream& operator<<(std::ostream& os, const Person& p) {
    os << p.get_name() << " (" << p.get_age() << ")";
    return os;
}

class PasswordValidator {
private:
    std::vector<char> restricted_symbols{'!', '$', '+'};

public:
    bool is_valid(const std::string& password) const {
        // Every character in password must match NONE of the restricted symbols
        return std::all_of(
            password.begin(),
            password.end(),
            [this](char ch) {
                return std::none_of(
                    restricted_symbols.begin(),
                    restricted_symbols.end(),
                    [ch](char forbidden) {
                        return ch == forbidden;
                    }
                );
            }
        );
    }
};

int main() {
    std::cout << std::boolalpha;

    // 1. std::for_each (unary operation)
    std::vector<int> nums{10, 20, 30, 40, 50};
    std::cout << "for_each: ";
    std::for_each(nums.begin(), nums.end(), [](int n) {
        std::cout << n << " ";
    });
    std::cout << "\n";

    // 2. std::is_permutation (binary predicate)
    Point p1{1, 2}, p2{3, 4}, p3{5, 6}, p4{7, 8};
    std::vector<Point> group_a{p1, p2, p3};
    std::vector<Point> group_b{p2, p3, p1};
    std::vector<Point> group_c{p1, p2, p4};

    auto same_point = [](const Point& lhs, const Point& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    };

    std::cout << "group_a and group_b same set of points? "
              << std::is_permutation(group_a.begin(), group_a.end(), group_b.begin(), same_point) << "\n";
    std::cout << "group_a and group_c same set of points? "
              << std::is_permutation(group_a.begin(), group_a.end(), group_c.begin(), same_point) << "\n";

    // 3. std::transform (unary operation with a by-value capture)
    std::vector<int> scores{93, 88, 75, 68, 65};
    int bonus_points{5};
    std::transform(scores.begin(), scores.end(), scores.begin(), [bonus_points](int s) {
        return s + bonus_points;
    });
    std::cout << "Adjusted scores: ";
    for (int s : scores) {
        std::cout << s << " ";
    }
    std::cout << "\n";

    // 4. Erase-remove idiom with std::remove_if (unary predicate)
    std::vector<int> values{1, 2, 3, 4, 5};
    auto new_end = std::remove_if(values.begin(), values.end(), [](int n) {
        return n % 2 == 0;  // true means "remove this even number"
    });
    values.erase(new_end, values.end());
    std::cout << "Remaining odd values: ";
    for (int v : values) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // 5. std::sort (binary predicates)
    std::vector<Person> members{
        {"Larry", 18},
        {"Moe", 30},
        {"Curly", 25}
    };

    std::sort(members.begin(), members.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.get_name() < rhs.get_name();  // ascending alphabetical order
    });
    std::cout << "Sorted by name (ascending): ";
    std::for_each(members.begin(), members.end(), [](const Person& p) {
        std::cout << p << " ";
    });
    std::cout << "\n";

    std::sort(members.begin(), members.end(), [](const Person& lhs, const Person& rhs) {
        return lhs.get_age() > rhs.get_age();  // descending age order
    });
    std::cout << "Sorted by age (descending): ";
    std::for_each(members.begin(), members.end(), [](const Person& p) {
        std::cout << p << " ";
    });
    std::cout << "\n";

    // 6. std::iota and std::all_of (unary predicate)
    std::vector<int> sequence(10);          // 10 elements initialized to 0
    std::iota(sequence.begin(), sequence.end(), 1);  // fills with 1, 2, ..., 10

    int min_allowed{1};
    int max_allowed{10};
    bool all_in_range = std::all_of(sequence.begin(), sequence.end(), [min_allowed, max_allowed](int n) {
        return n >= min_allowed && n <= max_allowed;
    });
    std::cout << "All elements in [1, 10]? " << all_in_range << "\n";

    // 7. Nested std::all_of and std::none_of inside a member function
    PasswordValidator validator{};
    std::cout << "Password 'Hollywood1$' valid? " << validator.is_valid("Hollywood1$") << "\n";
    std::cout << "Password 'Hollywood1' valid?  " << validator.is_valid("Hollywood1") << "\n";

    return 0;
}
```

---

## 7. Quick Reference Summary

| Concept | Key Takeaway |
| :--- | :--- |
| **Why Lambdas Exist** | Let you define concise, inline callable behavior right at the call site, avoiding boilerplate classes for short, localized tasks. |
| **Under the Hood** | The compiler generates a unique closure class with an overloaded `operator()`; captured local variables become members initialized when the closure object is created. |
| **Syntax** | `[captures](params) specifiers -> ret_type { body }` (parameters, specifiers, and return type are optional in common cases). |
| **Stateless (`[]`)** | Captures no local variables; relies on parameters and normal scope rules. |
| **Stateful (`[...]` non-empty)** | Captures local variables by value (`[x]`, `[=]`), by reference (`[&x]`, `[&]`), or captures `this` (`[this]`). |
| **`mutable`** | Allows modifying by-value captured copies inside the lambda's own closure state, without affecting the caller's original variables. |
| **Generic Lambdas (`auto`)** | `auto` parameters (C++14+) let the compiler deduce parameter types from the arguments passed at each call. |
| **Predicates & STL** | Predicates return `bool` and customize algorithms such as `std::sort`, `std::remove_if`, `std::all_of`, `std::none_of`, and `std::is_permutation`. |
