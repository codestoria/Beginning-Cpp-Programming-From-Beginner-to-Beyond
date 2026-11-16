# C++ OOP: Copy and Move Semantics

## Table of Contents

1. [Why Do We Need Copy and Move Semantics?](#1-why-do-we-need-copy-and-move-semantics)
2. [The Copy Constructor](#2-the-copy-constructor)
3. [Shallow Copying and Its Problems](#3-shallow-copying-and-its-problems)
4. [Deep Copying](#4-deep-copying)
5. [The Copy Assignment Operator](#5-the-copy-assignment-operator)
6. [The Rule of Three](#6-the-rule-of-three)
7. [lvalues and rvalues](#7-lvalues-and-rvalues)
8. [The Move Constructor and Move Semantics](#8-the-move-constructor-and-move-semantics)
9. [The Move Assignment Operator](#9-the-move-assignment-operator)
10. [The Rule of Five and the Rule of Zero](#10-the-rule-of-five-and-the-rule-of-zero)
11. [default and delete](#11-default-and-delete)
12. [Copy Elision and Return Value Optimization](#12-copy-elision-and-return-value-optimization)
13. [Summary Cheat Sheet](#13-summary-cheat-sheet)

---

## 1. Why Do We Need Copy and Move Semantics?

### 1.1 Objects are values

In C++, objects are **values**, not references. When you write:

```cpp
Player p2 = p1;
```

the language creates a **second object** that looks like `p1`. Changing `p2`'s own members afterwards does **not** affect `p1`. (If a member is a shallow-copied raw pointer, the two objects still share the pointed-to resource — see §3.) This is called **value semantics**, and it is a deliberate design choice: it makes the lifetime and ownership of data predictable, and it lets the compiler reason about your program without a garbage collector.

Contrast this with languages such as Java or Python, where `p2 = p1` makes both names refer to the **same** object. C++ chose value semantics because it gives programmers fine-grained control over performance and resource lifetime — but that control comes with responsibility: **you must tell the compiler how to copy and move your objects.**

### 1.2 The special member functions

The functions that define an object's lifecycle are called **special member functions**. There are six of them:

| Situation | Function used |
|---|---|
| Creating a new object from an existing one | **Copy constructor** |
| Assigning to an already-existing object | **Copy assignment operator** |
| Creating a new object from a temporary | **Move constructor** (C++11) |
| Assigning from a temporary | **Move assignment operator** (C++11) |
| Cleaning up when the object dies | **Destructor** |
| Creating an object with no source | **Default constructor** |

If you do not write these functions yourself, the compiler **generates them automatically** (with some exceptions we will see later). The generated versions perform a **member-wise** operation: every member of the source is copied (or moved) into the corresponding member of the destination.

### 1.3 The two problems this guide solves

**Problem 1 — incorrect default copying.** For many classes the compiler-generated copy is exactly right. But for a class that **owns a resource** (such as heap memory, a file handle, or a network socket), the default copy is **wrong**: it duplicates the *handle* but not the *resource*, leading to double-free, dangling pointers, and silent data corruption. Section 3 shows exactly how this happens.

**Problem 2 — wasteful copying.** Even when a copy is correct, it can be expensive. If the source object is a **temporary** that is about to be destroyed, duplicating its data is pure waste — we could simply *steal* its resources instead. **Move semantics** (introduced in C++11) make this possible, and they can turn an O(n) copy into an O(1) pointer transfer.

### 1.4 A note on RAII

Throughout this guide we will refer to **RAII** (*Resource Acquisition Is Initialization*). An RAII type is one that acquires a resource in its constructor and releases it in its destructor, so that the resource's lifetime is tied to the object's lifetime. `std::string`, `std::vector`, and `std::unique_ptr` are all RAII types. They already implement correct move and destroy behavior, and the appropriate copy behavior (`std::unique_ptr` is move-only, so its copy operations are deleted), which is why the modern guideline (§10) is to **build your classes out of RAII members** whenever possible.

---

## 2. The Copy Constructor

### 2.1 When is the copy constructor called?

The compiler uses the copy constructor whenever a **new object** must be created from an **existing lvalue**:

- Passing an **lvalue** to a function **by value** (passing an rvalue can use the move constructor instead — see §8).
- Returning an object from a function **by value**, if neither copy elision (§12) nor the move constructor (§8) applies.
- Writing an explicit copy: `Player p2{p1};` or `Player p2 = p1;`.

Note that some of these copies can be **eliminated entirely** by the compiler (**copy elision**, §12). For now, reason as if the copy constructor is always called; we will refine this picture later.

### 2.2 The compiler-generated copy constructor

If you do not write a copy constructor, the compiler generates one. It performs a **member-wise copy**: every member of the source is copied into the corresponding member of the destination. For class-type members (`std::string`, `std::vector`, …), *their* copy constructors are invoked in turn.

For many classes — especially those built only from RAII types — this generated copy constructor is **exactly what you want**, and you should leave it alone. The Rule of Zero (§10) turns this observation into a design guideline.

### 2.3 The signature — and *why* it looks the way it does

```cpp
ClassName(const ClassName& source);
```

Three parts deserve explanation:

1. **It is a constructor** — its name matches the class and it has no return type.
2. **It takes a reference (`&`).** If the parameter were passed **by value**, passing an object to the constructor would itself require copying that object — and copying it is exactly what the constructor is supposed to do, so you would need the constructor before you could call it. A reference avoids this: it is just an alias for the existing object, so no copy is needed to bind the parameter. (In fact, the C++ standard *requires* the first parameter of a copy constructor to be a reference; the by-value version would not even qualify as a copy constructor.)
3. **It takes a `const` reference.** Copying must not modify the source. The `const` enforces that, and it also allows us to copy from `const` objects — which is essential, since most objects you want to copy from are `const` in practice.

### 2.4 Implementing a copy constructor

The straightforward form initializes each member directly in the **member initializer list**:

```cpp
class Player {
public:
    Player(const Player& source)
        : name{source.name}, health{source.health}, xp{source.xp} {
        // Optional: any extra initialization logic
    }
private:
    std::string name;
    int health;
    int xp;
};
```

You can also **delegate** to another constructor to avoid repeating the initialization logic:

```cpp
// assumes a Player(std::string, int, int) constructor exists
Player::Player(const Player& source)
    : Player{source.name, source.health, source.xp} {
}
```

Delegation is preferable when the class has several constructors, because it keeps the initialization logic in one place.

### 2.5 Best practices

- Prefer a **`const` reference** (recommended, not required by the language): it prevents modifying the source and allows copying from `const` objects.
- If your class holds a **raw pointer** to owned memory, you **must** write a copy constructor that performs a *deep copy* (see §4).
- Prefer STL containers and smart pointers over raw pointers — they already handle copying correctly.

---

## 3. Shallow Copying and Its Problems

### 3.1 What a shallow copy does

A **shallow copy** copies the *pointer value*, not the data the pointer points to. After a shallow copy, both objects refer to the **same heap memory**. This is exactly what the compiler-generated copy constructor does for a raw-pointer member.

```cpp
class Shallow {
private:
    int* data;
public:
    Shallow(int d) : data{new int{d}} {}   // allocate
    ~Shallow()     { delete data; }        // free

    Shallow(const Shallow& source)
        : data{source.data} {              // shallow: copies the pointer only
    }
};
```

### 3.2 Why is this dangerous?

Consider passing such an object **by value**:

```cpp
void displayShallow(Shallow s) {          // s is a copy of the caller's object
    std::cout << *s.data << '\n';
}                                         // s destroyed here — destructor runs

int main() {
    Shallow obj{100};
    displayShallow(obj);                  // copy created
    std::cout << *obj.data << '\n';       // undefined behavior!
}
```

Trace what happens step by step:

1. `obj` is created; `obj.data` points to a heap integer containing `100`.
2. `displayShallow(obj)` creates a copy `s`; `s.data` points to the **same** heap integer.
3. `displayShallow` returns; `s`'s destructor runs `delete s.data`, freeing the heap integer.
4. Back in `main`, `obj.data` still points to that now-freed memory — it is a **dangling pointer**.
5. Reading `*obj.data` is **undefined behavior**. The program might crash, print garbage, or silently corrupt data.
6. When `obj` is later destroyed, its destructor calls `delete obj.data` on the already-freed pointer — a **double free**, which frequently crashes.

**The root cause:** two objects believe they **own** the same resource, and each will try to free it. This is the central idea behind all the problems in this guide: **ownership must be unambiguous.**

> **Rule of thumb.** Shallow copying is only safe when the class **does not own** the memory it points to (for example, it is a non-owning observer). For owning raw pointers, you need a **deep copy**.

---

## 4. Deep Copying

### 4.1 What a deep copy does

A **deep copy** allocates **new memory** for the copy and copies the **pointed-to data** into it. After a deep copy, each object owns its own independent resource.

```cpp
class Deep {
private:
    int* data;
public:
    Deep(int d) : data{new int{d}} {}
    ~Deep()     { delete data; }

    Deep(const Deep& source)
        : data{new int{*source.data}} {}   // new allocation + copied value
};
```

Or, using delegation:

```cpp
Deep::Deep(const Deep& source)
    : Deep{*source.data} {                  // reuse the int constructor
}
```

Now the memory layout is:

```
obj.data  → heap[A] = 100
copy.data → heap[B] = 100     ← independent allocation
```

Destroying the copy frees `B` only; `obj` remains perfectly valid.

### 4.2 When is a deep copy required?

When your class:

- Owns a raw pointer to dynamically allocated memory **and**
- Is responsible for releasing that memory in its destructor.

Otherwise, you will hit the double-free and dangling-pointer problems above.

### 4.3 Best-practice summary

- Prefer **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) — they handle ownership correctly.
- Prefer **STL containers** (`std::vector`, `std::string`) — they have proper copy constructors.
- If you **must** use raw pointers, always write a deep-copying copy constructor — and, in nearly every case, also a copy assignment operator and a destructor (the **Rule of Three**, §6).

---

## 5. The Copy Assignment Operator

### 5.1 How it differs from the copy constructor

The **copy assignment operator** is called when you assign to an **already-existing** object:

```cpp
a = b;    // both a and b already exist
```

This is different from the copy constructor, which **creates a brand-new object**. The distinction matters because the destination object may already **own a resource** that must be released before the new resource is installed.

### 5.2 Signature

```cpp
ClassName& operator=(const ClassName& source);
```

### 5.3 Why each part matters

- **Return type `ClassName&`** — return a reference to `*this` so that chained assignments like `a = b = c;` work.
- **`const ClassName&` parameter** — must not modify the source; also allows assigning from `const` objects.
- **Must handle self-assignment** — `a = a;` must not destroy its own data.

### 5.4 A correct implementation

```cpp
class Deep {
private:
    int* data;
public:
    Deep(int d) : data{new int{d}} {}
    ~Deep()     { delete data; }

    Deep(const Deep& source) : Deep{*source.data} {}

    Deep& operator=(const Deep& source) {
        if (this == &source) {                  // 1. self-assignment check
            return *this;
        }
        int* new_data = new int{*source.data};  // 2. allocate & copy FIRST
        delete data;                            // 3. THEN free old memory
        data = new_data;                        // 4. install new pointer
        return *this;                           // 5. support chaining
    }
};
```

**Allocating before deleting** makes the operator **exception-safe**: if `new` throws, the original object is untouched.

**Self-assignment check.** In this particular implementation, self-assignment would actually be safe even without the check (because we allocate before deleting). But the check avoids a pointless allocation and makes the intent explicit. Crucially, if you ever rewrite the operator as *delete-then-copy*, forgetting the check would destroy the very data you are about to read — so it is safer to check habitually.

### 5.5 What if you don't write one?

The compiler will generate a **member-wise copy assignment operator**. For raw-pointer members, that leads to exactly the same shallow-copy problems we saw with the copy constructor.

---

## 6. The Rule of Three

> **Rule of Three.** If a class needs **any one** of the following three special member functions, it almost certainly needs **all three**:
>
> 1. **Destructor** — to release resources.
> 2. **Copy constructor** — to perform a deep copy.
> 3. **Copy assignment operator** — to perform a deep copy on assignment.

**Why?** All three deal with **resource management**. If a class owns a resource (for example, a raw pointer), the three must cooperate. Missing one of them usually means a bug waiting to happen: leaks, double-free, or dangling pointers.

```cpp
// Rule of Three satisfied
class ResourceOwner {
public:
    ResourceOwner();                                     // constructor
    ~ResourceOwner();                                    // destructor
    ResourceOwner(const ResourceOwner& other);           // copy constructor
    ResourceOwner& operator=(const ResourceOwner& rhs);  // copy assignment
};
```

If you find yourself writing one of the three, **stop and ask: do I need the other two?** Most of the time, the answer is yes.

---

## 7. lvalues and rvalues

To understand **move** semantics, we first need the concept of *value categories*.

### 7.1 lvalues

An **lvalue** (*locator value*) is an expression that names a **stable, addressable** object. You can take its address with `&`.

```cpp
int x = 100;    // x is an lvalue
int* p = &x;    // OK: x has an address
```

**Intuition:** an lvalue is something **with a name** that persists beyond the current expression.

### 7.2 rvalues

An **rvalue** (*right value*) is a **temporary value** with no stable address — for example, the result of an arithmetic expression or an unnamed temporary object.

```cpp
int y = 100 + 200;         // 300 is an rvalue
// int* p = &(100 + 200);  // ERROR: cannot take the address of a temporary
```

**Intuition:** an rvalue is something **temporary** that will disappear soon.

### 7.3 The precise picture

The C++ standard splits expressions into three *basic* categories (and two *combined* categories built from them). You do not need to memorize the taxonomy, but knowing the names helps when reading error messages and documentation:

- **prvalue** ("pure rvalue") — a temporary with no identity, such as `Move{10}` or `100 + 200`.
- **xvalue** ("expiring value") — an object with an identity whose resources may be reused (moved from), such as the result of `std::move(obj)`. An xvalue is an rvalue that names a real object; it is **not** necessarily destroyed right away.
- **lvalue** — an object with a stable identity, such as a named variable.

The combined categories are **glvalue** (= lvalue or xvalue: an expression that names an object) and **rvalue** (= prvalue or xvalue: an expression whose resources may be reused). Both prvalues and xvalues are **rvalues**; the distinction matters only when we talk about `std::move` (§8.7) and guaranteed copy elision (§12).

### 7.4 Why this distinction matters

In move semantics, rvalues are the temporaries the compiler creates — for example `Move{10}` in `vec.push_back(Move{10});`. Because such a temporary is **about to be destroyed anyway**, it is **safe to steal its resources** instead of copying them. This is the key insight behind move operations.

---

## 8. The Move Constructor and Move Semantics

### 8.1 The problem: unnecessary deep copies

A deep copy is **expensive**: it allocates memory and duplicates data. But in many cases, the **source object is a temporary** that will be destroyed immediately after the copy. All that work is wasted.

```cpp
std::vector<Move> vec;
vec.push_back(Move{10});     // a temporary Move is created
```

Here, `Move{10}` is created and then destroyed. If the class has no move constructor **but has a usable copy constructor**, the temporary is **copied** into the vector — pure overhead, since its data could simply be **transferred**. With a move constructor available, the temporary is **moved** instead of copied, which is exactly the overhead move semantics eliminate.

### 8.2 Rvalue references

C++11 introduced **rvalue references**, written with `&&`:

```cpp
int&& rref = 200;   // rref is an rvalue reference bound to the temporary 200
rref = 300;         // legal: modifies the temporary
```

An rvalue reference can **bind to a temporary**, extending its lifetime, and — crucially — you are allowed to **modify** that temporary.

You cannot bind an rvalue reference to an lvalue:

```cpp
int x = 100;
int&& rref = x;    // ERROR: x is an L-value
```

This asymmetry is what lets the compiler distinguish "this argument is a temporary I can steal from" from "this argument is a real object I must not modify".

### 8.3 The move constructor

The move constructor takes an **rvalue reference** and **steals** the source's resources instead of copying them.

**Signature:**

```cpp
ClassName(ClassName&& source);   // no const, && instead of &
```

The parameter is **not `const`** because the move constructor must be able to modify the source — specifically, to null out its pointer so that the source's destructor does nothing.

**Differences from the copy constructor:**

| Copy constructor | Move constructor |
|---|---|
| `const ClassName&` | `ClassName&&` (non-const) |
| Copies data | Transfers ownership |
| Source is left **unchanged** | Source is left in a *moved-from* (typically empty) state |
| Can be slow (allocation + copy) | Extremely fast (pointer transfer + null-out) |

**Implementation:**

```cpp
class Move {
private:
    int* data;
public:
    Move(int d) : data{new int{d}} {}
    ~Move()     { delete data; }

    // Copy constructor (deep copy)
    Move(const Move& source) : Move{*source.data} {}

    // Move constructor
    Move(Move&& source) noexcept
        : data{source.data} {          // 1. steal the pointer
        source.data = nullptr;         // 2. null out the source
    }

    // ... copy assignment and move assignment omitted here
    //     see the complete Rule-of-Five version in §9.2
};
```

### 8.4 What the move constructor actually does

1. **Take ownership** of the source's pointer: `data = source.data`.
2. **Null out the source's pointer**: `source.data = nullptr`.

Step 2 is essential. When the source is destroyed shortly afterward, its destructor runs `delete data` — but on `nullptr`, which is **safe and does nothing**. Without the null-out, the source would `delete` a pointer we now own, causing a **double free**.

**No allocation, no copy** — just a pointer transfer and a null assignment. This is why moves are essentially free.

### 8.5 Why `noexcept`?

The `noexcept` specifier tells the compiler that the move constructor **will not throw**. This enables an important optimization:

- When a `std::vector` needs to grow, it must move (or copy) its elements to new storage.
- If your move constructor is `noexcept`, `vector` **will prefer to move**.
- If your move constructor could throw *and* a copy is available, `vector` falls back to copying, to preserve its strong exception-safety guarantee. (If a move throws halfway through, the original vector would be in a partially-moved state; a copy failure leaves the original intact.)

**Always mark move operations `noexcept` unless you have a compelling reason not to.**

### 8.6 When is the move constructor called?

The rule is simple: **the compiler calls the move constructor when the argument is an rvalue**; otherwise, it calls the copy constructor.

```cpp
Move a{10};
Move b{a};               // lvalue     → copy constructor
Move c = Move{20};       // prvalue    → C++17 guaranteed elision:
                         //              Move(int) directly initializes c; no copy/move ctor
Move d{std::move(a)};    // xvalue     → move constructor
```

### 8.7 What `std::move` actually does

`std::move(x)` **does not move anything**. It is nothing more than a **cast to an rvalue (specifically, an xvalue)**. It tells the compiler: *"Treat `x` as a temporary — I don't need its value anymore."*

```cpp
Move d{std::move(a)};   // now a is in a moved-from state
```

> **Important.** `std::move(a)` itself does **not** change `a`. Only **after `a` has actually been moved from** — e.g. after `Move d{std::move(a)};` has executed — must you stop relying on `a`'s **value**. You may still **destroy** it or **assign to it**, but reading it may be meaningless (in our simplified `Move` example, `a.data` is now `nullptr`).

### 8.8 A note on moved-from objects

In the simplified `Move` above, the copy constructor would dereference `source.data`. If the source was already moved-from (with `data == nullptr`), copying it would **dereference a null pointer** — undefined behavior. This is why it is critical that:

- For a custom type that does **not** document a moved-from state, do not read or dereference its resources — only destroy it or assign to it.
- Well-designed types (like `std::vector`, `std::string`) leave the moved-from object in a **valid but unspecified** state: you *can* destroy it, assign to it, and call operations that have no preconditions, but you must not assume it still holds its original value.

---

## 9. The Move Assignment Operator

### 9.1 Purpose

The move assignment operator is to *assignment* what the move constructor is to *construction*: it steals resources instead of copying them.

**Signature:**

```cpp
ClassName& operator=(ClassName&& source) noexcept;
```

### 9.2 Implementation

```cpp
class Move {
private:
    int* data;
public:
    Move(int d) : data{new int{d}} {}
    ~Move()     { delete data; }

    Move(const Move& source) : Move{*source.data} {}
    Move(Move&& source) noexcept : data{source.data} {
        source.data = nullptr;
    }

    // Copy assignment
    Move& operator=(const Move& source) {
        if (this == &source) {          // 1. self-assignment check
            return *this;
        }
        int* new_data = new int{*source.data};  // 2. allocate & copy first
        delete data;                            // 3. then free old memory
        data = new_data;                        // 4. install new pointer
        return *this;                           // 5. support chaining
    }

    // Move assignment
    Move& operator=(Move&& source) noexcept {
        if (this == &source) {          // 1. self-assignment check
            return *this;
        }
        delete data;                    // 2. free the existing resource
        data = source.data;             // 3. steal the source's pointer
        source.data = nullptr;          // 4. leave the source empty
        return *this;                   // 5. support chaining
    }
};
```

**Key steps:** self-assignment check → free current resource → steal the source's resource → leave the source safe and empty → return `*this`.

### 9.3 Why delete *before* steal?

Notice one subtle difference from the copy assignment operator: here we **delete first**, then steal. This is **intentional** — the move assignment operator is declared `noexcept`, meaning it is *designed* not to throw; if it did throw, `std::terminate` would be called. It is therefore written so that it cannot fail halfway. No allocation happens here, and the pointer assignments cannot throw. In this example the resource is an `int`, so `delete` cannot throw either; more generally, resource release in a destructor is expected not to throw. If a `noexcept` move assignment does throw, `std::terminate` is called, so move operations must not rely on throwing. That is why the "allocate-first" reordering we used for the copy assignment is unnecessary here.

---

## 10. The Rule of Five and the Rule of Zero

### 10.1 The Rule of Five

The **Rule of Five** extends the Rule of Three with the two move operations:

1. Destructor
2. Copy constructor
3. Copy assignment operator
4. **Move constructor**
5. **Move assignment operator**

If you define **any one** of them for a **resource-owning** class, you should **consider defining all five**.

This is not just a style preference — it follows from the C++ standard’s rules for implicitly declared special members: if you declare a destructor, copy constructor, or copy assignment operator, the **implicit declaration of the move constructor and move assignment operator is suppressed**. Overload resolution may then fall back to copying for rvalues, which may be slow or outright wrong. Conversely, declaring a move constructor or move assignment operator causes the **implicitly declared** copy operations to be **defined as deleted**. Being explicit about all five eliminates both surprises.

### 10.2 The Rule of Zero

> **Rule of Zero.** If your class does not directly manage a resource, do not write *any* of the five special member functions. Let the compiler generate them.

This is the **preferred modern style**:

- Use `std::string`, `std::vector`, `std::unique_ptr`, `std::shared_ptr`, etc., as members.
- These types already implement correct copy / move / destroy behavior.
- Your class then gets correct behavior **for free**.

```cpp
// Rule of Zero: no special members needed
class Player {
private:
    std::string name;                 // manages its own memory
    std::vector<int> inventory;       // manages its own memory
    std::unique_ptr<Weapon> weapon;   // manages its own memory
public:
    Player(std::string n) : name{std::move(n)} {}
};
```

The compiler-generated destructor, move constructor, and move assignment operator all behave correctly. Because `std::unique_ptr` is **move-only**, the compiler-generated copy constructor and copy assignment are **implicitly deleted** — `Player` becomes a **move-only type**. If you need `Player` to be copyable, either use `std::shared_ptr` for `weapon`, or implement a deep copy and follow the Rule of Five.

> **Guideline.** Prefer the **Rule of Zero**. Only fall back to the Rule of Five (or Three) when you genuinely need to manage a raw resource yourself.

---

## 11. default and delete

C++ lets you explicitly control which special member functions the compiler generates.

### 11.1 `= default`

You can ask the compiler to generate the **default implementation**:

```cpp
class Player {
public:
    Player()                          = default;   // default constructor
    Player(const Player&)             = default;   // copy constructor
    Player& operator=(const Player&)  = default;   // copy assignment
    Player(Player&&)                  = default;   // move constructor
    Player& operator=(Player&&)       = default;   // move assignment
    ~Player()                         = default;   // destructor
private:
    std::string name;
    int health = 0;
};
```

This is useful when you want to be **explicit** about which special members exist, without writing their bodies yourself.

**Important subtlety.** Merely declaring a destructor, copy constructor, or copy assignment operator **suppresses** the implicit move operations. So if you want moves to be generated, declare them with `= default` as shown above — or omit the destructor and copy operations entirely, and let the compiler generate whatever special members it needs.

### 11.2 `= delete`

You can **forbid** a special member function entirely:

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&)            = delete;  // no copying
    NonCopyable& operator=(const NonCopyable&) = delete;  // no copy assignment
};
```

Attempting to copy a `NonCopyable` object produces a **compile-time error**. This is the idiomatic way to express *"this class must not be copied"* — much clearer than the pre-C++11 technique of declaring a private copy constructor without a definition, which often failed at **link time** instead of producing a clear deleted-function compile error.

**Note.** Because we declared the copy operations, the compiler does **not** implicitly declare the move operations, so `NonCopyable` is also **not movable**. If you want a move-only type, declare the move operations explicitly:

```cpp
class MoveOnly {
public:
    MoveOnly() = default;
    MoveOnly(const MoveOnly&)            = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&&)                 = default;
    MoveOnly& operator=(MoveOnly&&)      = default;
};
```

---

## 12. Copy Elision and Return Value Optimization

Modern compilers are aggressive about **eliminating unnecessary copies**. When you step through code in a debugger, you may see the copy (or move) constructor **not being called**, even though the source code suggests it should be. This is **copy elision**.

The most common form is **Return Value Optimization (RVO)**:

```cpp
Move createMove() {
    Move temp{100};
    return temp;      // NRVO: the compiler may construct the result directly
}

Move m = createMove();   // C++17: the returned prvalue initializes m directly
```

There are two related cases:

### 12.1 Prvalue elision

A temporary prvalue **directly initializes** an object:

```cpp
Move m1 = Move{100};       // guaranteed prvalue elision
Move m2 = createMove();    // the returned prvalue initializes m2
```

Since **C++17**, this form of elision is **guaranteed by the standard**. No **copy or move** constructor is called, even if it has side effects. (Other constructors, such as the one that builds the prvalue in the first place, still run.)

**Careful.** The guarantee covers the prvalue → `m` initialization. What happens *inside* `createMove()` — namely whether `return temp;` triggers NRVO — is still an **optional** optimization (see §12.2). If NRVO does not happen, `return temp;` may call the move constructor.

### 12.2 Named RVO (NRVO)

A **named local variable** is returned:

```cpp
Move temp{100};
return temp;
```

This is still an **optional optimization**, **not guaranteed** by C++17. If the compiler does **not** elide the copy, the **move constructor** is used (or the copy constructor if no move is available).

### 12.3 Why you should be aware of this

Elision is **legal and beneficial** — it makes your code faster without any semantic change. Don't be confused if your debugger shows **fewer constructor calls than you expect**; that is the compiler doing its job.

> **Practical advice.** Write `return local_variable;` rather than `return std::move(local_variable);`. The former enables NRVO (which is free), whereas the latter prevents it and forces an actual move.

---

## 13. Summary Cheat Sheet

| Concept | Key point |
|---|---|
| **Copy constructor** | `const ClassName&`; creates a new object from an existing one; compiler-generated version is member-wise — shallow only for owning raw-pointer members |
| **Shallow copy** | Copies pointers, not pointed-to data; causes double-free and dangling pointers |
| **Deep copy** | Allocates new memory and copies the data; required for owning raw pointers |
| **Copy assignment** | `operator=(const ClassName&)`; handle self-assignment; allocate before deleting; return `*this` |
| **Rule of Three** | If you need a destructor, copy constructor, or copy assignment, you likely need all three |
| **lvalue vs rvalue** | An lvalue has a stable address; an rvalue is a temporary |
| **Rvalue reference** | `T&&`; binds to temporaries; enables move semantics |
| **Move constructor** | `ClassName&&`; steals resources, nulls out the source; mark `noexcept` |
| **Move assignment** | `ClassName& operator=(ClassName&&) noexcept`; same idea as the move constructor, but for assignment |
| **`std::move`** | A cast to an rvalue — it does not move anything by itself |
| **Rule of Five** | Destructor + copy ctor + copy assign + move ctor + move assign |
| **Rule of Zero** | Prefer members that manage their own resources; write no special members |
| **`= default` / `= delete`** | Explicitly request or forbid compiler-generated special members |
| **Copy elision / RVO** | Compiler may eliminate copies; guaranteed in many cases since C++17 |

---

## Final Thoughts

The rules in this guide all follow from one central question: **who owns this resource?**

- If your class **does not own** any resources — it only uses RAII types as members — you do not need to write anything. That is the **Rule of Zero**, and it is the style you should reach for first.
- If your class **owns** a resource, you must manage all of its lifecycle operations together. That is the **Rule of Three** (pre-C++11) or the **Rule of Five** (C++11 and later). A missing operation usually means a lurking bug.
- Understanding **value categories** (lvalues vs rvalues) is what allows you to write efficient move operations. **`std::move`** is only a cast; the real work happens in *your* move constructors and move assignment operators.

With these principles in mind, you can read any modern C++ class — from your own code to the standard library — and immediately see whether its resource handling is correct.
