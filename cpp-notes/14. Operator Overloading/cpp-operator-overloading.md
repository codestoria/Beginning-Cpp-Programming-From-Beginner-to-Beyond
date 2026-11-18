# C++ Operator Overloading

## Table of Contents

1. [Introduction: Why Operator Overloading?](#1-introduction-why-operator-overloading)
2. [What Is Operator Overloading?](#2-what-is-operator-overloading)
3. [Which Operators Can Be Overloaded?](#3-which-operators-can-be-overloaded)
4. [Basic Rules of Operator Overloading](#4-basic-rules-of-operator-overloading)
5. [Member Functions vs. Global Functions](#5-member-functions-vs-global-functions)
6. [The Assignment Operator](#6-the-assignment-operator)
7. [Summary](#7-summary)

---

## 1. Introduction: Why Operator Overloading?

As you learn C++, you quickly discover that operators like `+`, `-`, `*`, `=`, and `==` work naturally with built-in types:

```cpp
int a = 5, b = 10;
int c = a + b;                // addition
bool same = (a == b);         // comparison
std::cout << c << std::endl;  // stream insertion
```

But what happens when you define your own class? Consider a `Number` class that can represent integers, doubles, or complex numbers:

```cpp
Number a, b, c, d;
```

Suppose you want to add `a` and `b`, then multiply the result by the quotient of `c` divided by `d`. You have two choices. You could write member functions:

```cpp
Number result = a.add(b).multiply(c.divide(d));
```

Or you could write non-member functions:

```cpp
Number result = multiply(add(a, b), divide(c, d));
```

Both are hard to read and tedious to write. Yet you have been using `+`, `*`, and `/` since elementary school. Why can't you use them here?

**The problem**: C++ does not know what `+` means for your user-defined types. In most cases, it makes no sense to add or multiply arbitrary objects. What does it mean to subtract two `Player` objects in a game, or to multiply two `Account` objects? The answer is entirely up to you — the programmer.

**The solution**: C++ lets you *overload* operators so that they work with your own classes. Your code then becomes as readable and writable as if you were working with built-in types:

```cpp
Number result = (a + b) * (c / d);
```

This is the power and purpose of operator overloading: it lets your types behave like built-in types.

> **What you will learn in this document**: How operator overloading works, which operators you can overload, the rules you must follow, how to choose between member and global functions, and — most importantly — how to implement the assignment operator correctly for classes that manage resources. We assume you are familiar with classes, constructors, and destructors.

---

## 2. What Is Operator Overloading?

C++ operators are already defined for primitive (built-in) types such as `int`, `double`, and `long`. In fact, the `+` operator is already *overloaded* by the language itself: it can add two integers, two doubles, or an integer and a double, and it behaves differently in each case.

**Operator overloading** means giving a new meaning to an operator when it is used with user-defined types. It allows your types to behave and feel similar to the built-in types, making code more readable and writable.

Key points to understand:

- **Operators are functions in disguise.** Overloading an operator is simply defining a function with a special name: the keyword `operator` followed by the operator symbol. For example, overloading `+` defines a function named `operator+`:

  ```cpp
  // A member function version: a + b becomes a.operator+(b)
  Number Number::operator+(const Number& rhs) const {
      // ... compute and return the sum ...
  }

  // A global function version: a + b becomes operator+(a, b)
  Number operator+(const Number& lhs, const Number& rhs) {
      // ... compute and return the sum ...
  }
  ```

- **Behind the scenes, it is a function call.** When you write `a + b`, the compiler translates this into a function call such as `a.operator+(b)` or `operator+(a, b)`. This is **syntactic sugar** — it looks like an operator, but it is actually a method or function call.

- **The compiler usually does not provide meaningful overloaded operators for your class.** If you want an operator to have class-specific meaning, you normally have to define that overload yourself. Among the overloadable operators, there is one important exception: the **assignment operator** (`=`). The compiler implicitly declares a copy assignment operator (and, under specific conditions, a move assignment operator) for your class. However, these implicit versions may not do what you want — for example, they may be defined as **deleted** if a member is `const`, a reference, or itself not assignable, so not every object is actually assignable. In addition, user-declared special member functions can suppress or affect the implicit declaration of the move assignment operator. All other overloadable operators that you want to give class-specific meaning must be explicitly defined by the programmer.

> **Key idea**: Operator overloading does not change how operators work with built-in types. It only lets you define what an operator means for *your* types.

---

## 3. Which Operators Can Be Overloaded?

C++ allows you to overload most of its operators. However, a few operators **cannot** be overloaded:

| Operator | Name | Why It Cannot Be Overloaded |
|----------|------|----------------------------|
| `::` | Scope resolution operator | It names a scope, not a value |
| `?:` | Conditional (ternary) operator | It has special evaluation semantics |
| `.*` | Pointer-to-member access operator | It accesses a member through a pointer-to-member |
| `.` | Dot (member access) operator | It is fundamental to the language |
| `sizeof` | `sizeof` operator | It is evaluated at compile time |
| `typeid`, `alignof`, `noexcept` | Type / alignment / `noexcept` operators | They are language operators with special semantics |
| `static_cast`, `dynamic_cast`, `const_cast`, `reinterpret_cast` | Named casts | They have special semantics and cannot be overloaded |

> Note: `.*` cannot be overloaded, but `->*` (the pointer-to-member access operator applied to an object pointer) **can** be overloaded. Do not confuse the two.

**Important principle**: Just because an operator *can* be overloaded does not mean you *should* overload it. Only overload an operator if it makes sense for your class and makes your code more usable, more readable, and more writable. If overloading an operator would confuse users of your class, don't do it.

For example, it makes sense to overload `+` for a `Number` class — users intuitively know what addition means. It is much less clear what `==` should mean for a `Player` class: are you comparing names, health, XP, or all attributes? The meaning must be obvious to the users of your class.

---

## 4. Basic Rules of Operator Overloading

When overloading operators, the following rules must be followed:

1. **Precedence and associativity cannot be changed.** C++ programmers are already familiar with how operators bind; overloading does not alter that. For example, `*` still binds tighter than `+`.

2. **Arity cannot be changed (for ordinary operators).** If an operator is binary (applied to two arguments), it stays binary; if it is unary, it stays unary. You cannot turn a binary operator into a unary one or vice versa. The special operators `operator()` and `operator[]` are exceptions: `operator()` accepts any number of parameters, and since C++23 `operator[]` may also take more than one.

3. **You cannot change how operators work with built-in types.** For example, you cannot redefine what `+` does for two `int`s. At least one operand must be a user-defined type.

4. **You cannot create new operators.** You can only overload operators that already exist in C++. You cannot invent a new symbol like `**` for exponentiation (though you could overload `^` or write a function instead).

5. **Some operators must be implemented as member functions.** The assignment operator (`=`) and the arrow operator (`->`) are required by the language to be **non-static** member functions. The subscript operator (`[]`) and the function call operator (`()`) are also required to be member functions; before C++23 they had to be non-static, and since C++23 they may also be declared `static` member functions. Others, like `+` or `==`, can be implemented as either member or global functions. The reason is partly historical and partly semantic: operators that fundamentally modify the left-hand object or access its internals are required to be members so that the left-hand operand is normally the object itself.

---

## 5. Member Functions vs. Global Functions

C++ allows you to overload operators using either **member functions** or **global (non-member) functions**. Each has its advantages:

- **Member functions** have direct access to the private members of the class and are naturally associated with the left-hand operand (the object on which the method is called). They are required for operators like `=` and `[]`.

- **Global functions** are useful when the left-hand operand is not an object of your class — for example, when overloading `<<` for stream output, where the left-hand side is `std::ostream`, not your class. They may need to be declared as `friend` functions if they need access to private data.

The choice depends on the situation. A common guideline is:

- If the operator modifies the left-hand operand (e.g., `=`, `+=`, `++`), make it a member function.
- If the operator creates a new value from its operands (e.g., `+`, `==`), it can be either. A member function only allows implicit conversions on the right-hand operand; a global function allows implicit conversions on both operands, which often makes it the better choice when symmetry matters.

**A concrete example.** Suppose you want to overload `<<` so that you can print a `Number` object:

```cpp
Number n(42);
std::cout << n << std::endl;  // we want this to work
```

The left-hand operand is `std::cout`, which is an `std::ostream`, not a `Number`. Therefore, this operator cannot be a member of `Number` — it must be a global function:

```cpp
std::ostream& operator<<(std::ostream& os, const Number& n) {
    os << n.value;  // may need friend access
    return os;
}
```

In the next section, we focus on the assignment operator, which **must** be a **non-static** member function.

---

## 6. The Assignment Operator

> **Running example**: To make the discussion concrete, we will use a simple `MyString` class that wraps a dynamically allocated C-style string. A minimal version looks like this:
>
> ```cpp
> #include <cstring>
> #include <iostream>
> #include <utility>   // for std::move
>
> class MyString {
> public:
>     // Constructor: creates a MyString from a C-style string.
>     // Precondition: s must not be nullptr (pass "" for an empty string).
>     MyString(const char* s = "") {
>         str = new char[std::strlen(s) + 1];
>         std::strcpy(str, s);
>     }
>
>     // Copy constructor
>     MyString(const MyString& rhs) {
>         str = new char[std::strlen(rhs.str) + 1];
>         std::strcpy(str, rhs.str);
>     }
>
>     // Move constructor
>     MyString(MyString&& rhs) noexcept {
>         str = rhs.str;
>         rhs.str = nullptr;
>     }
>
>     // Destructor
>     ~MyString() {
>         delete[] str;
>     }
>
>     // The assignment operators we will implement below
>     MyString& operator=(const MyString& rhs);
>     MyString& operator=(MyString&& rhs) noexcept;
>
> private:
>     char* str;
> };
> ```
>
> The class owns a heap-allocated character array, so it must manage that resource carefully. This is exactly the situation where overloading the assignment operator matters.
>
> **A note on moved-from objects.** After a move, the source object's `str` is `nullptr`. A moved-from `MyString` can be safely destroyed (the destructor calls `delete[] nullptr`, which does nothing) or reassigned, but it must **not** be used as the source of a copy — copying it would call `std::strlen(nullptr)`, which is undefined behavior. This is a common convention: moved-from objects are left in a "valid but unspecified" state that supports only destruction and reassignment. If you need a moved-from object to remain fully usable, you must handle the `nullptr` case explicitly in the copy operations.

### 6.1 Initialization vs. Assignment

Before we overload the assignment operator, we must clearly distinguish between **initialization** and **assignment**:

- **Initialization** happens when a new object is created. It is performed by constructors. For example:

  ```cpp
  MyString s2 = s1;  // initialization (copy constructor is called)
  ```

  Even though this looks like an assignment, it is actually initialization, because `s2` has not been created yet. The statement is equivalent to `MyString s2(s1);` and invokes the **copy constructor**.

- **Assignment** happens when an already-existing object is given a new value. For example:

  ```cpp
  MyString s2;  // s2 is created (default constructor)
  s2 = s1;      // assignment (operator= is called)
  ```

  Here `s2` already exists, so the assignment operator is invoked.

> **Key rule**: An assignment occurs when an object has already been initialized and you want to assign another object to it. Initialization creates a new object; assignment modifies an existing one.

### 6.2 The Implicitly-Declared Copy Assignment Operator

C++ provides a default way of assigning one object to another. If you do not provide your own copy assignment operator, the compiler implicitly declares one. The implicit copy assignment operator performs **member-wise assignment**: each member of the right-hand side object is assigned to the corresponding member of the left-hand side object, using that member's own assignment operator. For a class-type member this calls its `operator=`; for a raw-pointer member it copies only the pointer value — a **shallow copy** — not the pointed-to data.

**Caveats.** The implicit version is not always available. It may be defined as **deleted** if a member is `const`, a reference, or itself not assignable. Declaring a **move constructor or move assignment operator** also causes the implicit copy assignment operator to be defined as `deleted`. Conversely, declaring a destructor, copy constructor, or copy assignment operator suppresses the implicit declaration of the **move** assignment operator (and the move constructor). (Declaring a destructor or a copy constructor does not, by itself, suppress the implicit copy assignment operator in the usual case, though in C++11–C++17 such a declaration made it deprecated; that deprecation rule was removed in C++20.)

If your class does not contain raw pointers, the implicitly-declared copy assignment operator is usually fine. However, if your class has a raw pointer member (as our `MyString` class does), shallow copying causes problems:

```
After shallow copy:
s1: [str] ---> "Hello\0"   (on heap)
s2: [str] ---> "Hello\0"   (same memory! both point to it)

When s2 is destroyed:
s2: [str] ---> (freed)
s1: [str] ---> (dangling pointer!)

When s1 is later destroyed:
s1: [str] ---> (freed again — double-free error!)
```

Two objects end up pointing to the same dynamically allocated memory. When one object is destroyed, the other is left with a dangling pointer; when both are destroyed, you get a double-free error.

Therefore, for classes that manage resources (like raw pointers), you must overload the assignment operator to perform a **deep copy**: allocate new memory and copy the pointed-to data, so each object owns its own resources independently.

### 6.3 Overloading the Copy Assignment Operator

#### 6.3.1 The Prototype

The copy assignment operator must be overloaded as a member function. Its general form is:

```cpp
Type& operator=(const Type& rhs);
```

For our `MyString` class:

```cpp
class MyString {
public:
    MyString& operator=(const MyString& rhs);
    // ...
};
```

Why this signature?

- **Returns `Type&` (a reference to the class type)**: We return by reference to avoid unnecessary copies and to support **chained assignment**, such as `a = b = c`. The expression `b = c` returns a reference to `b`, which is then assigned to `a`. If we returned by value, each assignment would create an extra temporary object.

- **Parameter is `const Type&`**: The right-hand side object is passed by reference to avoid a copy, and it is `const` because we do not intend to modify it — it is the source of the assignment, not the target.

- **Member function**: The assignment operator must be a member function. The left-hand side object is the object on which the method is called (referred to by the `this` pointer), and the right-hand side object is the parameter.

#### 6.3.2 The Implementation Pattern

Here is a robust implementation for deep-copy assignment:

```cpp
MyString& MyString::operator=(const MyString& rhs) {
    std::cout << "Copy assignment" << std::endl;

    // 1. Check for self-assignment
    if (this == &rhs) {
        return *this;
    }

    // 2. Allocate new space and copy the data into it
    //    (do this before releasing the old buffer, so that a failed
    //     allocation leaves *this unchanged)
    char* newStr = new char[std::strlen(rhs.str) + 1];
    std::strcpy(newStr, rhs.str);

    // 3. Deallocate the left-hand side's existing resource
    delete[] str;

    // 4. Take ownership of the new buffer
    str = newStr;

    // 5. Return the left-hand side object (for chaining)
    return *this;
}
```

Let's walk through each step:

1. **Check for self-assignment.** If someone writes `a = a`, we should detect it and do nothing. This avoids unnecessary allocation and copying. More importantly, it protects implementations that deallocate the old resource before copying from `rhs`: without the check, such an implementation would free the very resource it then tries to copy from. **The implementation above** avoids that specific danger by allocating and copying **before** deleting the old buffer, so `a = a` would not crash even without the check — but the check is still recommended because it avoids wasted work. We check by comparing the address of the current object (`this`) with the address of the right-hand side object (`&rhs`).

2. **Allocate new space and copy the data.** We allocate a new buffer and copy `rhs.str` into it *before* touching the old resource. If `new` throws `std::bad_alloc`, `*this` is left unchanged — no leak, no dangling pointer. Assuming `std::strlen`/`std::strcpy` do not throw and `delete[]` does not throw, this ordering gives the whole copy assignment the **strong exception safety** guarantee. (An alternative is the copy-and-swap idiom: build a temporary `MyString` from `rhs`, then `swap` it with `*this`.)

3. **Deallocate the left-hand side's existing resource.** Now that the new buffer is ready, it is safe to `delete[]` the old character array. The left-hand side object may already own some memory (even an empty string occupies memory). If we simply overwrote the pointer, that memory would be orphaned and leaked.

4. **Take ownership of the new buffer.** We assign `newStr` to `str`, so the object now owns the freshly copied data.

5. **Return `*this`.** We return a reference to the left-hand side object to support chained assignments like `a = b = c`.

This pattern is an **idiom** in C++ — a standard way of implementing deep-copy assignment. If your class has multiple raw pointers, remember to deep-copy each one.

> **Related idiom: the Rule of Three / Rule of Five.** If a class needs a custom destructor, a custom copy constructor, or a custom copy assignment operator, it almost certainly needs all three (the **Rule of Three**). With C++11 and move semantics, the same logic extends to the move constructor and move assignment operator (the **Rule of Five**). The reason is that these five special member functions all deal with resource management, and handling one correctly usually requires handling all of them consistently. This is why our `MyString` class defines a destructor, a copy constructor, a move constructor, and both assignment operators — they form a package deal.

#### 6.3.3 A Note on `s = "Hello"` (Assignment from a C-style string)

Consider this statement:

```cpp
MyString s;
s = "Hello";
```

Here, `s` is an existing object, so this is assignment. But the right-hand side is a C-style string literal (`const char*`), not a `MyString` object. How does this work?

Overload resolution considers all viable `operator=` candidates. If there is an assignment operator that accepts a `const char*`, it can be selected directly. Otherwise, `"Hello"` can be converted into a temporary `MyString` using a **converting constructor** (a constructor that takes a `const char*`), and then an assignment operator is called to assign that temporary object to `s`. Because the temporary is an rvalue, the **move assignment operator** is selected if it is available; otherwise the **copy assignment operator** is used. Finally, the temporary object is destroyed.

This is why the `MyString` class needs a constructor that accepts a C-style string — it enables implicit conversion from string literals. If you want to prevent such implicit conversions, you can mark the constructor `explicit`.

### 6.4 Overloading the Move Assignment Operator

#### 6.4.1 Motivation: Lvalues and Rvalues

The copy assignment operator we just wrote performs a deep copy: it allocates new memory and copies every character. This is necessary when the right-hand side object is a named variable that will continue to exist after the assignment — we cannot steal its resources because it may be used again later.

To talk about this precisely, C++ distinguishes two kinds of expressions:

- **Lvalue** (locator value): an expression that refers to a persistent object — roughly, something whose address you can take. (Strictly, there are lvalues you cannot take the address of, such as bit-fields; "roughly" is precise enough here.) Named variables are lvalues, and they persist beyond the expression in which they appear.
- **Rvalue**: an expression that is treated as movable-from — either a temporary that does not refer to a persistent object, or an expression such as `std::move(x)` that names a persistent object but is explicitly marked as movable-from. Temporaries such as `MyString("Frank")`, and the result of expressions like `x + y`, are rvalues; they are typically destroyed at the end of the full expression. (Strictly, C++11 splits rvalues into *prvalues* and *xvalues*; for this discussion, "rvalue" is precise enough.)

Consider:

```cpp
MyString s1;
s1 = MyString("Frank");
```

The right-hand side creates a temporary `MyString` object (an rvalue). It has no name and will be destroyed at the end of the statement. There is no need to deep-copy its data — we can simply **steal** its pointer, avoiding an allocation and a copy. This is much more efficient.

Move semantics allow us to do exactly that.

#### 6.4.2 When Is Move Assignment Used?

The move assignment operator is selected when the right-hand side is an **rvalue expression** — typically a temporary object or an object explicitly passed through `std::move`. The parameter type `Type&&` is an rvalue reference, but a named `Type&&` variable is itself an lvalue. Examples:

```cpp
s1 = MyString("Frank");  // temporary object (an rvalue)
s2 = std::move(s3);      // explicit move
```

`std::move` is declared in `<utility>`; remember to `#include <utility>` when you use it. Note that `std::move` itself does **not** move anything — it is just a cast that produces an xvalue (an rvalue), so that an overload taking `Type&&` can be selected.

If you do not provide a move assignment operator (and no move assignment is implicitly declared), overload resolution falls back to the copy assignment operator when it is available. The program still works, but it is less efficient.

#### 6.4.3 The Prototype

```cpp
Type& operator=(Type&& rhs);
```

Key differences from copy assignment:

- **Double ampersand (`&&`)** in the parameter list indicates an rvalue reference.
- **No `const`** on the parameter, because we will modify the right-hand side object (we will null out its pointer).
- **`noexcept`** is strongly recommended when the move operation cannot throw. For this `MyString`, move assignment only transfers ownership of a resource; it does not allocate memory and does not throw. Marking move operations `noexcept` allows standard containers and generic code to use moves more aggressively when they need a strong exception guarantee; in particular, `std::vector` reallocation decides whether to move or copy elements based on whether the element's **move constructor** is `noexcept`. For a general type, whether a move operation should be `noexcept` depends on its actual implementation.

For `MyString`:

```cpp
MyString& operator=(MyString&& rhs) noexcept;
```

#### 6.4.4 The Implementation

```cpp
MyString& MyString::operator=(MyString&& rhs) noexcept {
    // Note: no debug output here. This function is marked noexcept, and
    // iostream operations can throw when the stream's exception mask is set;
    // an exception escaping a noexcept function calls std::terminate.

    // 1. Check for self-assignment
    if (this == &rhs) {
        return *this;
    }

    // 2. Deallocate the left-hand side's existing resource
    delete[] str;

    // 3. Steal the pointer from the right-hand side
    str = rhs.str;

    // 4. Null out the right-hand side pointer
    rhs.str = nullptr;

    // 5. Return the left-hand side object
    return *this;
}
```

Step by step:

1. **Check for self-assignment.** As with copy assignment, we must avoid self-assignment. (This is rare with rvalues, but `s = std::move(s)` does reach this branch.)

2. **Deallocate the left-hand side's existing resource.** The left-hand side object may already own memory. We delete it so it doesn't leak when we overwrite the pointer.

3. **Steal the pointer.** Instead of allocating new memory and copying, we simply copy the pointer value from `rhs` to `this->str`. Now both point to the same memory — but only temporarily.

4. **Null out the right-hand side pointer.** This step is **critical**. The source object will eventually be destroyed — often it is a temporary, but it may also be a named object passed through `std::move`. If we don't null out its pointer, its destructor will `delete[]` the memory we just stole, leaving the left-hand side object with a dangling pointer. By setting `rhs.str = nullptr`, we ensure the destructor has nothing to delete.

5. **Return `*this`.** As with copy assignment, we return the left-hand side object by reference.

#### 6.4.5 Conceptual Diagram

Consider `s1 = MyString("Hola")`:

**Before assignment:**

```
s1:        [str] ---> "Hello\0" (on heap)

temporary: [str] ---> "Hola\0"  (on heap)
```

**After `delete[] s1.str`:**

```
s1:        [str] ---> (dangling)

temporary: [str] ---> "Hola\0"
```

**After stealing the pointer:**

```
s1:        [str] ---> "Hola\0"   (same memory as the temporary used to own)

temporary: [str] ---> "Hola\0"   (still points to that memory; ownership
                                  has been transferred, but the pointer is
                                  not yet null)
```

**After nulling out the temporary's pointer:**

```
s1:        [str] ---> "Hola\0"

temporary: [str] ---> nullptr
```

When the temporary is destroyed, its destructor calls `delete[] nullptr`, which is safe and does nothing. The memory now belongs solely to `s1`.

#### 6.4.6 Why Move Assignment Is More Efficient

Copy assignment requires:
- Allocating new memory
- Copying every character from the source
- Deallocating the old memory

Move assignment requires:
- Deallocating the old memory
- Copying a single pointer value
- Setting a pointer to null

No allocation, no character-by-character copy. For large strings or expensive-to-copy objects, this is a significant performance win.

---

## 7. Summary

- **Operator overloading** lets you define how operators work with your own classes, making your code as readable and writable as code that uses built-in types.

- **Operators are functions in disguise.** The `operator` keyword followed by the symbol defines the function name. Writing `a + b` is syntactic sugar for a function call such as `a.operator+(b)` or `operator+(a, b)`.

- **Most operators can be overloaded**, but some (like `.`, `::`, `?:`, `.*`, `sizeof`, `typeid`, and the named casts) cannot. Some must be member functions: `=` and `->` must be **non-static** members; `[]` and `()` must be members — they had to be non-static before C++23, and since C++23 they may also be declared `static` member functions. Only overload an operator when it makes sense for your class.

- **Initialization uses constructors; assignment uses `operator=`.** Know the difference: `MyString s2 = s1;` is initialization (copy constructor), while `s2 = s1;` after `s2` already exists is assignment.

- **The compiler implicitly declares a copy assignment operator** that performs **member-wise assignment**. For class-type members, this calls their `operator=`; for raw-pointer members, it copies only the pointer — a **shallow copy**. For classes with raw pointers, this is dangerous — you must overload `operator=` to perform a **deep copy**. Note that the implicitly declared operator may be `deleted` (e.g., if a member is `const`, a reference, or not assignable).

- **Copy assignment** (`operator=(const Type&)`) is used for lvalues. It performs a deep copy: check for self-assignment, allocate and copy first, deallocate the old resource, take ownership, return `*this`. Allocating before deallocating gives strong exception safety.

- **Move assignment** (`operator=(Type&&)`) is used for rvalue arguments — typically temporaries, or named objects explicitly passed through `std::move`. It steals the resource: check for self-assignment, deallocate, steal pointer, null out source, return `*this`. Mark it `noexcept` when it cannot throw; this helps containers and generic code use move operations safely and efficiently. (For `std::vector` reallocation, the relevant property is whether the **move constructor** is `noexcept`.)

- **Check for self-assignment** in `operator=`. For copy assignment, the check avoids unnecessary allocation and copying, and it is essential for implementations that delete the old resource before copying from `rhs` (the implementation in §6.3.2 allocates first, so it would survive self-assignment even without the check, but the check still saves work). For the move assignment implemented in §6.4.4, the check is **required**: without it, `s = std::move(s)` would release `s`'s resource and then set `s.str` to null, losing its original value. The check makes the operation a safe no-op.

- **Move semantics are more efficient** than copy semantics because they avoid allocation and copying.

- **Moved-from objects** are left in a "valid but unspecified" state. For our `MyString`, `str` is `nullptr`, so the object can be safely destroyed or reassigned — but it must not be used as the **source** of a copy, because copying a `nullptr` `str` would call `std::strlen(nullptr)`, which is undefined behavior.

- **The Rule of Three / Rule of Five**: If you need a custom destructor, copy constructor, or copy assignment operator, you almost certainly need all three (and with move semantics, all five). Resource management is a package deal.

By mastering operator overloading, you make your classes first-class citizens of the C++ language — types that behave as naturally as `int` and `double`. Start with the assignment operator, and you will build a solid foundation for overloading other operators like `+`, `==`, `<<`, and more in the rest of this section.
