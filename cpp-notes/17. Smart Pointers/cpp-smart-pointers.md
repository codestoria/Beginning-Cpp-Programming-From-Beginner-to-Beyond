# C++ Smart Pointers

## Table of Contents

1. [Why We Need Smart Pointers: The Problems with Raw Pointers](#1-why-we-need-smart-pointers-the-problems-with-raw-pointers)
2. [What Smart Pointers Actually Are](#2-what-smart-pointers-actually-are)
3. [std::unique_ptr — Exclusive Ownership](#3-stdunique_ptr--exclusive-ownership)
4. [std::shared_ptr — Shared Ownership](#4-stdshared_ptr--shared-ownership)
5. [std::weak_ptr — Breaking Reference Cycles](#5-stdweak_ptr--breaking-reference-cycles)
6. [Custom Deleters](#6-custom-deleters)
7. [Summary](#7-summary)

---

## 1. Why We Need Smart Pointers: The Problems with Raw Pointers

Raw pointers in C++ are extremely flexible. They give you direct control over heap memory: you can allocate it, pass it around, and free it whenever you want. That flexibility is powerful, but it also places the entire burden of memory management on you. In practice, manual memory management is one of the largest sources of bugs in C++ programs.

To understand why smart pointers exist, we first need to see what goes wrong with raw pointers.

### 1.1 Common bugs caused by raw pointers

1. **Uninitialized (wild) pointers**
   A pointer declared without initialization holds a garbage address. Writing to that address can crash your program or silently corrupt data.

2. **Memory leaks**
   If you allocate memory with `new` but forget to call `delete`, that memory is never returned to the system. In long-running programs, leaks accumulate and eventually cause performance degradation or crashes.

3. **Dangling pointers**
   A dangling pointer still points to memory that has already been freed. Accessing it causes undefined behavior — crashes, corrupted data, or bugs that are nearly impossible to reproduce.

4. **Lack of exception safety** (or: **Exception-unsafe behavior**)
   If an exception is thrown between a `new` and its matching `delete`, the cleanup code never runs. Even carefully written code can leak memory in this scenario.

> These pointer-related errors are among the most common and most difficult to debug in C++ programs. Smart pointers were designed specifically to prevent them by construction — that is, by making the correct behavior the default.

The key insight is this: **the problem is not pointers themselves, but manual ownership management.** If we can encode ownership rules into a type, the compiler can enforce them for us. That is exactly what smart pointers do.

---

## 2. What Smart Pointers Actually Are

### 2.1 Core definition

A **smart pointer** is a C++ template class object that wraps a raw pointer and manages or observes the lifetime of the object it points to, according to its ownership semantics.

You can think of it as a "wrapper" around a regular pointer. For **owning** smart pointers, the job is simple:

- It **owns** the heap object it points to.
- It automatically calls `delete` when the object is no longer needed.
- It follows the **RAII** principle (explained below).

Owning smart pointers are usually created as stack objects, so their destructors run deterministically when they go out of scope. That guarantee is the foundation of automatic cleanup. (Non-owning smart pointers such as `std::weak_ptr` observe an object without extending its lifetime.)

### 2.2 The RAII principle

RAII stands for **Resource Acquisition Is Initialization**. It is the foundational design pattern behind smart pointers and all modern C++ resource management.

The idea is straightforward:

1. **Acquire the resource in the constructor** — for a smart pointer, this means taking ownership of heap memory.
2. **Release the resource in the destructor** — for a smart pointer, this means calling `delete`.
3. The wrapper object is **typically a stack object**, so its lifetime is deterministic: it is destroyed automatically when it goes out of scope.

Because the destructor runs automatically — even when an exception is thrown — you never forget to clean up. This is what makes smart pointers exception-safe by design.

### 2.3 Ownership

"Ownership" means *who is responsible for destroying the object and freeing its memory*.

Smart pointers make ownership explicit in code:

- If ownership is clear, there is no confusion about who should call `delete`.
- Different smart pointer types implement different ownership models, so you can choose the one that matches your intent.

### 2.4 Standard smart pointer types

C++11 and later provide three main smart pointers, all declared in the `<memory>` header:

- `std::unique_ptr` — exclusive, single ownership
- `std::shared_ptr` — shared ownership via reference counting
- `std::weak_ptr` — non-owning observer for shared objects

> **Note:** The older `std::auto_ptr` is deprecated (and was **removed in C++17**) and should never be used in modern code. It was replaced by `std::unique_ptr` because its copy semantics were error-prone.

### 2.5 How smart pointers compare to raw pointers

- `std::unique_ptr` and `std::shared_ptr` support the dereference operator `*`
  and the member-access operator `->` just like raw pointers. They also provide
  `.get()` to obtain the underlying raw pointer.
  `std::weak_ptr` supports neither `*` nor `->`: to access the object, call
  `.lock()` first to obtain a `shared_ptr` (see Section 5).
- You **cannot** perform pointer arithmetic. This is intentional — it prevents many out-of-bounds bugs.
- You can access the underlying raw pointer with `.get()` (on `unique_ptr` and
  `shared_ptr`). Use this only when necessary, for example when calling a C library
  API that expects a raw pointer.

In short, smart pointers look and feel like raw pointers in normal use, but they remove the dangerous operations and automate cleanup.

---

## 3. std::unique_ptr — Exclusive Ownership

### 3.1 Core idea

`std::unique_ptr` is the simplest and most efficient smart pointer. It represents **exclusive ownership**: at most one non-null `unique_ptr` owns the heap object at any time.

When the `unique_ptr` goes out of scope, the heap object is immediately destroyed. There is no reference counting and no extra bookkeeping. With the default deleter, a `unique_ptr` is typically the same size as a raw pointer and adds no runtime overhead; a stateful custom deleter may add some overhead.

**When to use it:** Use `unique_ptr` by default for any heap-allocated object that has a single clear owner. This covers the vast majority of cases.

### 3.2 Key properties

- Cannot be copied or copy-assigned. The copy constructor and copy assignment operator are *deleted*.
- Can be **moved**. Ownership transfers from one `unique_ptr` to another.
- Works well in standard containers such as `std::vector`.
- With the default deleter, typically has **no runtime overhead** compared to a raw pointer.

The reason copying is disabled is simple: if two `unique_ptr` objects owned the same heap object, both would try to delete it, causing a double-free. By forbidding copies and allowing only moves, the type enforces the "exactly one owner" rule at compile time.

### 3.3 Creating a unique_ptr

#### Method 1: Using `new` (not recommended)

```cpp
#include <memory>

std::unique_ptr<int> p1{new int{100}};
```

This works, but it requires writing `new` explicitly and is less safe than the modern approach.

#### Method 2: Using `std::make_unique` (preferred, C++14+)

```cpp
auto p2 = std::make_unique<int>(200);
auto acc = std::make_unique<Account>("Moe", 5000);
```

`make_unique` creates the object and the smart pointer in a single step. It is:

- Safer (no explicit `new`, no ambiguity about ownership)
- Exception-safe
- Cleaner and harder to get wrong

> **Why is `make_unique` safer?** Before C++17, consider `f(std::unique_ptr<T>{new T}, g())`.
> The order in which `new T` and `g()` are evaluated was unspecified. If `g()` threw after `new T` had run but before the `unique_ptr` was constructed, the allocated `T` leaked. `make_unique` avoids this class of problem entirely because the allocation and the ownership transfer happen inside a single function call. (Since C++17, function argument evaluation is indeterminately sequenced, so this specific example no longer leaks; the general recommendation to prefer `make_unique` still stands.)

### 3.4 Basic usage

```cpp
#include <iostream>
#include <memory>

int main() {
    auto p = std::make_unique<int>(100);

    // Dereference
    std::cout << *p << '\n'; // prints 100

    // Modify the value
    *p = 200;

    // Check whether the pointer is non-null
    if (p) {
        std::cout << "p points to a valid int\n";
    }

    return 0;
} // p goes out of scope here → the int is automatically deleted
```

### 3.5 Useful methods

- `.get()` — returns the raw pointer inside. Use it only to interact with code that expects raw pointers.
- `.reset()` — destroys the owned object and sets the pointer to `nullptr`.

```cpp
auto p = std::make_unique<int>(100);
p.reset(); // the int is freed; p is now null
```

### 3.6 Move semantics

Since `unique_ptr` cannot be copied, you use `std::move` to transfer ownership:

```cpp
auto p1 = std::make_unique<int>(100);
auto p2 = std::move(p1); // ownership moves to p2

// p1 is now nullptr; p2 owns the integer
```

After the move, `p1` is in a valid but empty state (it holds `nullptr`). This is why moving is safe: ownership is transferred, never duplicated.

### 3.7 unique_ptr in vectors

To put a `unique_ptr` into a `std::vector`, you must move it:

```cpp
std::vector<std::unique_ptr<Account>> accounts;

auto acc = std::make_unique<Account>("James", 1000);
accounts.push_back(std::move(acc)); // acc becomes null
```

When looping over the vector, use **const references** to avoid copying (which is not allowed):

```cpp
for (const auto& acc : accounts) {
    std::cout << *acc << '\n';
}
```

> **Rule of thumb:** Use `std::unique_ptr` by default for heap-allocated objects. It is fast, simple, and makes ownership obvious.

---

## 4. std::shared_ptr — Shared Ownership

### 4.1 Core idea

Sometimes an object genuinely needs to be owned by more than one party. For example, multiple parts of a program may need to keep a resource alive, and no single part can decide when it is safe to destroy it. For these cases, C++ provides `std::shared_ptr`.

A `shared_ptr` allows **multiple pointers to share ownership of the same heap object**. The object stays alive as long as at least one `shared_ptr` points to it.

**When to use it:** Use `shared_ptr` only when shared ownership is truly required. It is more complex and more expensive than `unique_ptr`, so prefer `unique_ptr` whenever a single owner is sufficient.

### 4.2 How it works: reference counting

Every `shared_ptr` is paired with a hidden **control block** that stores a reference counter:

- When a `shared_ptr` is copy-constructed or copy-assigned from another `shared_ptr`, the counter increments by 1. (Constructing a `shared_ptr` from a raw pointer creates a *new* control block with a count of 1; moving does not change the count.)
- When a `shared_ptr` is destroyed or reset, the counter decrements by 1.
- When the counter reaches 0, the object is automatically deleted.

This mechanism is simple and effective, but it adds a small amount of overhead (the counter itself, plus atomic operations used to make reference-count updates thread-safe). Note that this only makes the **reference count** thread-safe; concurrent access to the managed object itself still requires synchronization.

### 4.3 Key properties

- Can be freely copied, assigned, and moved.
- Copying increments the reference count.
- Destroying decrements the reference count.
- Supports custom deleters (see Section 6).

### 4.4 Creating a shared_ptr

#### Method 1: Using `new` (not recommended)

```cpp
std::shared_ptr<int> p1{new int{100}};
```

#### Method 2: Using `std::make_shared` (preferred, C++11+)

```cpp
auto p2 = std::make_shared<int>(200);
auto acc = std::make_shared<Account>("Larry", 5000);
```

`make_shared` allocates the object and the control block in a single memory allocation, making it faster and more memory-efficient than constructing the pointer from a raw `new` expression.

### 4.5 Reference count in action

```cpp
#include <iostream>
#include <memory>

int main() {
    auto p1 = std::make_shared<int>(100);
    std::cout << p1.use_count() << '\n'; // 1

    {
        auto p2 = p1; // copy → count increases
        std::cout << p1.use_count() << '\n'; // 2
    } // p2 goes out of scope → count decreases

    std::cout << p1.use_count() << '\n'; // 1

    p1.reset(); // count drops to 0 → the int is deleted

    return 0;
}
```

The `.use_count()` method returns the current number of `shared_ptr` owners. It is mainly useful for debugging and for understanding the lifetime of your objects.

### 4.6 shared_ptr in vectors

Unlike `unique_ptr`, you can copy `shared_ptr` directly into a vector:

```cpp
std::vector<std::shared_ptr<Account>> accounts;

auto acc1 = std::make_shared<Account>("Larry", 1000);
accounts.push_back(acc1); // copy → reference count becomes 2
```

Both the local variable and the vector element share ownership. The object is destroyed only when *both* are gone.

---

## 5. std::weak_ptr — Breaking Reference Cycles

### 5.1 The problem: strong circular references

Consider two classes where each holds a `shared_ptr` to the other:

```cpp
class B; // forward declaration

class A {
    std::shared_ptr<B> b_ptr;
public:
    void set_b(std::shared_ptr<B> b) { b_ptr = b; }
};

class B {
    std::shared_ptr<A> a_ptr;
public:
    void set_a(std::shared_ptr<A> a) { a_ptr = a; }
};
```

Now create objects and link them:

```cpp
auto a = std::make_shared<A>();
auto b = std::make_shared<B>();
a->set_b(b);
b->set_a(a);
```

This creates a **circular reference**:

- A holds a strong reference to B.
- B holds a strong reference to A.

While the local `a` and `b` still exist, both reference counts are 2. When `a` and `b` go out of scope, each count only drops to 1, because each object is still held by the other. Neither count ever reaches 0, so the objects are never destroyed — a **memory leak**.

This is not a rare edge case — it appears naturally in graphs, trees with parent pointers, and any bidirectional data structure.

### 5.2 How weak_ptr solves this

A `std::weak_ptr` is a **non-owning observer**. It points to a `shared_ptr`-managed object but does **not** increment the **strong** reference count (it does increase the control block's weak count, which keeps the control block alive).

Because it does not participate in ownership, a `weak_ptr` does not extend the object's lifetime. It only lets you check whether the object still exists and access it safely if it does.

### 5.3 Fixing the cycle

Break the cycle by making one side a `weak_ptr`:

```cpp
class B {
    std::weak_ptr<A> a_ptr; // weak reference; does not keep A alive
public:
    void set_a(std::shared_ptr<A> a) { a_ptr = a; }
    std::shared_ptr<A> lock_a() const { return a_ptr.lock(); }
};
```

Now:

- A owns B (strong reference).
- B observes A (weak reference).

When `a` and `b` go out of scope, the reference count correctly drops to 0 and both objects are destroyed.

### 5.4 Accessing the object safely

You cannot dereference a `weak_ptr` directly. Instead, call `.lock()` to obtain a `shared_ptr`:

```cpp
std::shared_ptr<A> safe_ptr = b->lock_a();
if (safe_ptr) {
    // The object still exists; use it safely.
} else {
    // The object has already been destroyed.
}
```

The `lock()` method returns a `shared_ptr` if the object is still alive, or an empty `shared_ptr` otherwise. This prevents you from accidentally accessing a destroyed object.

### 5.5 Common use cases

- Breaking circular references in graphs, trees with parent pointers, and bidirectional data structures.
- Temporary observer pointers (such as caches or iterators) that should not extend an object's lifetime.

---

## 6. Custom Deleters

### 6.1 When you need them

By default, smart pointers use `delete` to destroy their object. Sometimes you need more than that:

- Wrapping C-style structs that have no destructor.
- Releasing special resources (file handles, locks, network connections).
- Running custom cleanup logic.

> Custom deleters are an advanced feature you will not need every day, but they are good to know about.

### 6.2 Important note

You **cannot** use `make_unique` or `make_shared` with custom deleters, because the factory functions are designed to use the default `delete`. When you need a custom deleter, construct the smart pointer directly — typically from a raw pointer obtained via `new`:

```cpp
std::unique_ptr<Test, void(*)(Test*)> p{new Test{100}, my_deleter};
```

### 6.3 Method 1: Function pointer deleter

Write a free function that takes the raw pointer and performs cleanup:

```cpp
void my_deleter(Test* ptr) {
    std::cout << "Using custom function deleter\n";
    delete ptr;
}

int main() {
    std::shared_ptr<Test> ptr{new Test{100}, my_deleter};
    return 0;
} // my_deleter is called automatically
```

### 6.4 Method 2: Lambda deleter

Define the cleanup logic inline with a lambda expression:

```cpp
int main() {
    std::shared_ptr<Test> ptr{
        new Test{1000},
        [](Test* p) {
            std::cout << "Using custom lambda deleter\n";
            delete p;
        }
    };
    return 0;
}
```

Lambdas keep the cleanup logic next to the pointer declaration, which improves readability for simple cases.

---

## 7. Summary

### 7.1 Guidelines for writing modern C++

1. **Prefer smart pointers over raw pointers** for all heap-allocated memory.
2. **Use `std::unique_ptr` by default.** It is fast, simple, and makes ownership clear.
3. **Use `std::shared_ptr` only when you truly need shared ownership.** Do not overuse it — shared ownership adds complexity and overhead.
4. **Prefer `std::make_unique` and `std::make_shared` by default.** Both are safer and cleaner than raw `new`; `make_shared` is also more efficient because it allocates the object and the control block together. Use explicit `new` only when you need a custom deleter, since the factory functions do not support custom deleters.
5. **Do not use raw pointers to express ownership.** A smart pointer should own the object; raw pointers may be used only as non-owning observers, and must not outlive the owner. Never construct a second `shared_ptr` from the same raw pointer.
6. **Use `std::weak_ptr` to break reference cycles.**
7. **Use custom deleters only for special cases.** Most of the time, the default `delete` is exactly what you need.

### 7.2 What we have achieved

With smart pointers, you can write C++ code that:

- Has no explicit `new` or `delete` in most places.
- Automatically cleans up heap memory.
- Greatly reduces memory leaks, dangling pointers, and wild pointers. (They are not fully eliminated if, for example, you keep raw pointers obtained via `.get()` beyond the owner's lifetime.)
- Is naturally exception-safe.

Smart pointers are one of the defining features of modern C++. Mastering them will make your code more robust, easier to read, and much less error-prone.
