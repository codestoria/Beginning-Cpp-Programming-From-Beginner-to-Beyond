# C++ OOP Advanced: Object Lifecycle, Copy/Move Semantics, and Advanced Class Features

## Table of Contents

- [Introduction](#introduction)
- [Part 1: Object Lifecycle — Constructors and Destructors](#part-1-object-lifecycle--constructors-and-destructors)
  - [1.1 What Is a Constructor?](#11-what-is-a-constructor)
  - [1.2 The Default Constructor](#12-the-default-constructor)
  - [1.3 Overloading Constructors](#13-overloading-constructors)
  - [1.4 Constructor Initialization Lists](#14-constructor-initialization-lists)
  - [1.5 Delegating Constructors](#15-delegating-constructors)
  - [1.6 Constructor Parameters and Default Values](#16-constructor-parameters-and-default-values)
  - [1.7 Destructors](#17-destructors)
  - [1.8 Part 1 Summary](#18-part-1-summary)
- [Part 2: Copy and Move Semantics](#part-2-copy-and-move-semantics)
  - [2.1 The Copy Constructor](#21-the-copy-constructor)
  - [2.2 Shallow Copying and Its Problems](#22-shallow-copying-and-its-problems)
  - [2.3 Deep Copying](#23-deep-copying)
  - [2.4 Copy Assignment Operator](#24-copy-assignment-operator)
  - [2.5 The Rule of Three](#25-the-rule-of-three)
  - [2.6 L-values and R-values](#26-l-values-and-r-values)
  - [2.7 Move Constructors and Move Semantics](#27-move-constructors-and-move-semantics)
  - [2.8 Move Assignment Operator](#28-move-assignment-operator)
  - [2.9 The Rule of Five and the Rule of Zero](#29-the-rule-of-five-and-the-rule-of-zero)
  - [2.10 `= default` and `= delete`](#210--default-and--delete)
  - [2.11 Copy Elision and Return Value Optimization (RVO)](#211-copy-elision-and-return-value-optimization-rvo)
  - [2.12 Part 2 Summary](#212-part-2-summary)
- [Part 3: Advanced Class Features](#part-3-advanced-class-features)
  - [3.1 Friends](#31-friends)
  - [3.2 The `this` Pointer](#32-the-this-pointer)
  - [3.3 `const` Correctness in Classes](#33-const-correctness-in-classes)
  - [3.4 Static Class Members](#34-static-class-members)
- [Part 4: Structs vs Classes](#part-4-structs-vs-classes)
- [Summary](#summary)

---

## Introduction

When you first learn C++ classes, it is natural to focus on member variables and basic methods. But writing a class that merely compiles is very different from writing one that is correct, efficient, and maintainable. Real C++ programs demand that you understand **how objects are created, copied, moved, and destroyed**, and how to control access to class internals.

This tutorial covers the essential class-related features that separate beginner code from production-quality code:

- Constructors and destructors — the object lifecycle
- Constructor initialization lists, delegating constructors, and default parameters
- Copy constructors; shallow vs. deep copying
- Move constructors and move semantics
- Friend functions and friend classes
- The `this` pointer
- `const` correctness in classes
- Static class members
- The difference between `struct` and `class`

**Prerequisites:** You should already be comfortable with basic C++ syntax — variables, functions, `if`/`for`, and the idea of a class with member variables and member functions. You do *not* need prior experience with copy/move semantics or the more advanced features covered here; each concept is introduced from first principles.

**How to read this tutorial:** Every section follows the same pattern. First we explain **why** a feature exists — what problem it solves. Then we show **what** it is and **how** to use it. Finally, we point out common pitfalls. Do not skip the "why" parts; understanding the motivation is what lets you apply these features correctly in your own code.

---

## Part 1: Object Lifecycle — Constructors and Destructors

### 1.1 What Is a Constructor?

When the compiler creates an object, it first allocates raw memory. At that moment, the bytes in that memory are essentially random — they have no meaning for your class. A **constructor** is a special member function whose job is to turn that raw memory into a **valid, initialized object**.

Consider what happens without a constructor:

```cpp
class Player {
public:
    std::string name;
    int health;
    int xp;
};

int main() {
    Player p;          // memory allocated, but health and xp are indeterminate!
    std::cout << p.health;  // undefined behavior: reading an uninitialized int
}
```

The `std::string` member is fine — it has its own constructor that runs automatically. But `health` and `xp` are built-in types with no constructor, so they are left with whatever bits happened to be in memory. Reading them is **undefined behavior**.

A constructor fixes this by giving you a place to establish a known state:

```cpp
class Player {
private:
    std::string name;
    int health;
    int xp;
public:
    Player() {
        name = "None";
        health = 0;
        xp = 0;
    }
};
```

Key properties of constructors:

- They have the **same name as the class**.
- They have **no return type** (not even `void`).
- They can be **overloaded** — you can define several with different parameter lists.
- They are invoked **automatically** by the compiler during object creation. You never call a constructor directly.

> **Why "no return type"?** A constructor does not produce a value that you can use; its only effect is to initialize the object being created. The language enforces this by forbidding a return type, which prevents confusion with ordinary functions.

---

### 1.2 The Default Constructor

A **default constructor** is a constructor that can be called with **no arguments**. It is also called the **no-args constructor**.

#### Compiler-generated default constructor

If you define **no constructors at all**, the compiler implicitly declares a default constructor (provided it can be generated — for example, all members must be default-constructible). This generated constructor does almost nothing: it default-constructs class-type members (like `std::string`) and leaves built-in members (`int`, `double`, pointers) with **indeterminate values**.

```cpp
class Account {
private:
    std::string name;   // default-constructed to empty string
    double balance;     // indeterminate!
    // No constructors defined.
};

int main() {
    Account frank;               // OK: compiler-generated default constructor
    Account* jim = new Account;  // Also OK
    // frank.balance holds garbage — do not read it!
}
```

> **Pitfall:** The compiler-generated default constructor does **not** initialize built-in members. Reading such a member is undefined behavior. This is one of the most common sources of subtle bugs in C++.

#### Writing your own default constructor

The safe approach is to write your own no-args constructor so that every object starts in a known state:

```cpp
class Account {
private:
    std::string name;
    double balance;
public:
    Account() {
        name = "None";
        balance = 0.0;
    }
};
```

#### The critical rule: defining any constructor removes the implicit default

Once you define **any** constructor — even one that takes arguments — the compiler **stops generating** the default constructor. If your code still needs a no-args constructor, you must write it yourself.

```cpp
class Account {
private:
    std::string name;
    double balance;
public:
    Account(std::string name_val, double balance_val) {
        name = name_val;
        balance = balance_val;
    }
    // No no-args constructor defined!
};

int main() {
    Account frank;                // ERROR: no default constructor available
    Account bill("Bill", 100.0);  // OK
}
```

This rule is intentional. Once you have provided a way to construct your object, the compiler assumes a default-constructed object may not make sense for your class (for example, an `Account` with no owner and no balance might be meaningless). If you *do* want a default constructor in addition to your parameterized one, write it explicitly.

---

### 1.3 Overloading Constructors

A class can have as many constructors as needed, as long as each has a **unique signature** (a unique parameter list). The compiler selects the correct one based on the arguments you provide at the point of object creation.

```cpp
class Player {
private:
    std::string name;
    int health;
    int xp;
public:
    Player() { name = "None"; health = 0; xp = 0; }
    Player(std::string name_val) { name = name_val; health = 0; xp = 0; }
    Player(std::string name_val, int health_val, int xp_val) {
        name = name_val;
        health = health_val;
        xp = xp_val;
    }
};

int main() {
    Player empty;                                  // calls no-args
    Player hero("Hero");                           // calls one-string
    Player frank("Frank", 100, 4);                 // calls three-args
    Player* enemy = new Player("Enemy", 1000, 0);  // heap allocation
}
```

If the compiler cannot decide which constructor to call — for example, if two constructors could both accept the given arguments — it produces an **ambiguity error** rather than guessing. We will see an example in [1.6](#16-constructor-parameters-and-default-values).

---

### 1.4 Constructor Initialization Lists

So far we have initialized members by **assigning** values inside the constructor body:

```cpp
Player::Player() {
    name = "None";   // assignment, not initialization
    health = 0;
    xp = 0;
}
```

This works, but it is **not true initialization**. By the time the constructor body runs, every member has already been constructed. So `name = "None"` is not "creating `name` with the value `"None"`" — it is "creating `name` as an empty string, then replacing its contents with `"None"`".

For a `std::string`, this means:

1. The default constructor runs, allocating a small internal buffer (or none).
2. The assignment operator runs, possibly allocating a new buffer and copying `"None"` into it.

For complex types, this can be wasteful: you may allocate memory, then immediately free it and allocate again.

#### The initialization-list syntax

C++ lets you **initialize** members directly, before the constructor body runs, using a **constructor initialization list**:

```cpp
Player::Player()
    : name{"None"}, health{0}, xp{0} {
    // constructor body (can be empty)
}
```

The syntax:

1. After the parameter list, write a **colon** (`:`).
2. Follow it with a comma-separated list of `member{value}` or `member(value)` initializers.
3. Then write the constructor body (which may be empty).

Now `name` is initialized **directly** to `"None"` — the empty string is never created, so there is no wasted allocation.

#### Declaration order matters, not initialization-list order

Members are initialized in the order they are **declared in the class**, not the order they appear in the initialization list. This is a common source of confusion:

```cpp
class Example {
private:
    int a;
    int b;
public:
    Example() : b{2}, a{1} {}  // a is initialized first (declared first), then b
};
```

The compiler may warn you if the initialization-list order differs from the declaration order. Best practice: **keep the initialization list in the same order as the member declarations**.

#### Initializing with parameters

The same technique works for parameterized constructors:

```cpp
Player::Player(std::string name_val, int health_val, int xp_val)
    : name{name_val}, health{health_val}, xp{xp_val} {
}
```

#### When initialization lists are mandatory

For some members, assignment inside the constructor body is **impossible**:

- **`const` members** — they cannot be assigned after construction.
- **Reference members** — they must be bound at construction and cannot be rebound.
- **Members of types without a default constructor** — they must be given arguments at construction.

```cpp
class Config {
private:
    const int max_players;   // must be initialized in the list
    std::string& log_file;   // reference member: must be initialized in the list
public:
    Config(int max, std::string& file)
        : max_players{max}, log_file{file} {   // required
    }
};
```

For built-in types, the difference is small: the member is default-initialized (leaving it indeterminate) and then assigned, so the main benefit of an initialization list is clarity and avoiding an indeterminate intermediate value. For class-type members, the difference can be significant. **Rule of thumb: always use initialization lists unless you have a specific reason not to.**

---

### 1.5 Delegating Constructors

When you overload constructors, you often end up duplicating initialization logic:

```cpp
Player::Player()
    : name{"None"}, health{0}, xp{0} {}

Player::Player(std::string name_val)
    : name{name_val}, health{0}, xp{0} {}

Player::Player(std::string name_val, int health_val, int xp_val)
    : name{name_val}, health{health_val}, xp{xp_val} {}
```

The three initialization lists are nearly identical. Duplicated code is a bug magnet: when you change the initialization logic, you might forget to update every constructor.

C++ solves this with **delegating constructors**: one constructor can call another constructor of the same class from its initialization list.

```cpp
// The "master" constructor does all the work.
Player::Player(std::string name_val, int health_val, int xp_val)
    : name{name_val}, health{health_val}, xp{xp_val} {
}

// Delegating constructors: each calls the master.
Player::Player() : Player{"None", 0, 0} {
}

Player::Player(std::string name_val) : Player{name_val, 0, 0} {
}
```

Rules for delegation:

1. After the parameter list, write `:` followed by the **class name** and the arguments to the target constructor.
2. You **cannot** also initialize members in the same initialization list — delegation replaces member initialization.
3. Delegation **only works in the initialization list**, not in the constructor body.
4. If constructor A delegates to constructor B, **both bodies run**: B's body first, then A's.

This eliminates duplication. Change the master constructor, and all delegating constructors follow automatically.

---

### 1.6 Constructor Parameters and Default Values

Just like ordinary functions, constructors can have **default parameter values**. This can dramatically reduce the number of overloaded constructors you need.

```cpp
class Player {
private:
    std::string name;
    int health;
    int xp;
public:
    Player(std::string name_val = "None",
           int health_val = 0,
           int xp_val = 0)
        : name{name_val}, health{health_val}, xp{xp_val} {
    }
};
```

Now a single constructor handles every call:

```cpp
Player empty;                          // name="None", health=0, xp=0
Player hero{"Hero"};                   // name="Hero",  health=0, xp=0
Player frank{"Frank", 100};            // name="Frank", health=100, xp=0
Player villain{"Villain", 100, 55};    // all three supplied
```

This one constructor replaces the no-args, one-arg, two-arg, and three-arg constructors from earlier sections.

#### Ambiguity warning

If you combine default parameters with other constructors, be careful not to create **ambiguous overloads**:

```cpp
class Player {
public:
    Player(std::string name_val = "None", int health_val = 0, int xp_val = 0);
    Player();  // AMBIGUOUS with the above when called with no args!
};
```

For `Player p;`, the compiler cannot decide which constructor to call — both are viable — so it emits an error. Default parameters are convenient when they make sense, but sometimes you want to force the caller to provide certain values explicitly. Use them judiciously.

---

### 1.7 Destructors

A **destructor** is a special member function called automatically when an object is destroyed. It is the counterpart to the constructor: the constructor acquires resources and establishes state, the destructor releases resources and cleans up.

Key properties:

- Same name as the class, preceded by a **tilde** (`~`).
- **No return type** and **no parameters**.
- **Cannot be overloaded** — exactly one destructor per class.
- Called automatically when:
  - A local (stack) object goes out of scope.
  - `delete` is called on a pointer to a dynamically allocated object.
  - An object containing a member object is destroyed; the member object's destructor runs as part of the containing object's destruction.

```cpp
class Player {
public:
    Player() { /* constructor */ }
    ~Player() {
        // Cleanup code here
        std::cout << "Destructor called for " << name << std::endl;
    }
private:
    std::string name;
    int health;
    int xp;
};
```

Why can't destructors be overloaded? Because **the compiler decides when to call the destructor** — you do not supply arguments. If there were multiple destructors, the compiler would have no way to choose one.

#### What goes in a destructor?

The destructor is the right place to:

- Release dynamically allocated memory (`delete` / `delete[]`).
- Close files or network connections.
- Release other resources (locks, handles, etc.).

If you don't write a destructor, the compiler generates an empty one. For classes with no resources to release, that is perfectly fine.

#### Order of destruction

When a block of code ends, local objects are destroyed in **reverse order** of their creation (LIFO — last in, first out). This mirrors the stack discipline:

```cpp
{
    Player frank;    // constructed first
    Player hero;     // constructed second
    Player villain;  // constructed third
}   // villain destroyed first, then hero, then frank
```

Understanding this order matters when objects depend on each other. For example, if `hero` holds a reference to `frank`, you need to know that `frank` outlives `hero` — which is guaranteed by the reverse-order rule.

---

### 1.8 Part 1 Summary

| Concept | Key Point |
|---|---|
| Constructor | Same name as class, no return type, can be overloaded; turns raw memory into a valid object |
| Default constructor | Generated only if no constructors are defined; write your own to avoid indeterminate values |
| Initialization list | True initialization, more efficient than assignment; required for `const`, reference, and non-default-constructible members |
| Declaration order | Members are initialized in declaration order, not initialization-list order |
| Delegating constructor | One constructor calls another to eliminate duplicated initialization logic |
| Default parameters | Reduce overload count; watch for ambiguity with other constructors |
| Destructor | One per class, no parameters; releases resources; called in reverse order of construction |

---

## Part 2: Copy and Move Semantics

### 2.1 The Copy Constructor

C++ may create copies of objects in several situations:

1. **Pass-by-value** to a function or method.
2. **Return-by-value** from a function or method.
3. **Explicit copy**: `Player p2{p1};` or `Player p2 = p1;`.

In these cases, C++ uses a **copy constructor** — or, when the source is a temporary, a **move constructor** — to create the new object. Note that compilers may eliminate the copy/move entirely through copy elision; since C++17, some of these elisions are guaranteed (see [2.11](#211-copy-elision-and-return-value-optimization-rvo)).

#### Compiler-generated copy constructor

If you don't define a copy constructor, the compiler generates one that performs a **member-wise copy**: each member of the source object is copied to the corresponding member of the target object. For class-type members, their copy constructors are invoked.

For many classes — especially those built only from `std::string`, `std::vector`, and other RAII types — this generated copy constructor is exactly what you want.

#### The copy constructor signature

A copy constructor has the form:

```cpp
ClassName(const ClassName& source);
```

- Same name as the class (it is a constructor).
- Takes a **single parameter**: a **const reference** to the same class type.

Why **reference** and **const**?

- **Reference**: if the parameter were passed by value, the copy constructor would need to make a copy to pass the argument — which would call the copy constructor — which would make a copy… infinite recursion. A reference avoids the copy.
- **`const`**: copying should not modify the source. The `const` qualifier enforces this and allows copying from `const` objects.

#### Implementing a copy constructor

```cpp
class Player {
public:
    Player(const Player& source)
        : name{source.name}, health{source.health}, xp{source.xp} {
        // Optional: additional initialization logic
    }
private:
    std::string name;
    int health;
    int xp;
};
```

You can also **delegate** to another constructor:

```cpp
Player::Player(const Player& source)
    : Player{source.name, source.health, source.xp} {
}
```

#### Best practices for copy constructors

- If your class manages a **raw pointer**, you **must** write a copy constructor that performs a **deep copy** (see [2.3](#23-deep-copying)).
- Use a `const` reference parameter.
- Prefer STL containers and smart pointers over raw pointers — they handle copying correctly for you.
- If possible, avoid raw pointers as data members entirely.

---

### 2.2 Shallow Copying and Its Problems

A **shallow copy** copies the **pointer value**, not the data it points to. Both the original and the copy end up pointing to the **same** heap-allocated memory.

This is the default behavior of the compiler-generated copy constructor for pointer members.

```cpp
class Shallow {
private:
    int* data;
public:
    Shallow(int d) {
        data = new int{d};   // allocate and store d
    }
    ~Shallow() {
        delete data;         // free the memory
    }
    Shallow(const Shallow& source)
        : data{source.data} {  // shallow copy: copies the pointer
    }
};
```

#### Why is this dangerous?

Consider what happens when an object is passed by value:

```cpp
void displayShallow(Shallow s) {  // s is a copy of the caller's object
    std::cout << *s.data << std::endl;
}   // s goes out of scope — its destructor runs

int main() {
    Shallow obj{100};
    displayShallow(obj);  // copy made; the copy's destructor deletes the data
    // obj.data now points to freed memory!
    std::cout << *obj.data << std::endl;  // undefined behavior!
}
```

The sequence of events:

1. `obj` is created; `obj.data` points to a heap integer containing `100`.
2. `displayShallow(obj)` makes a copy `s`; `s.data` points to the **same** heap integer.
3. When `displayShallow` returns, `s` is destroyed; `s.~Shallow()` calls `delete s.data`, freeing the heap integer.
4. Back in `main`, `obj.data` still points to that now-freed memory.
5. Accessing `*obj.data` is **undefined behavior** — the program might crash, produce garbage, or silently corrupt data.
6. Eventually, `obj`'s destructor also calls `delete obj.data` — a **double-free**, which often crashes the program.

The fundamental problem: **two objects think they own the same resource**, and each will try to free it.

Shallow copying is only safe when the class does **not** own the memory it points to (for example, it is a non-owning observer). For owning raw pointers, you need a **deep copy**.

---

### 2.3 Deep Copying

A **deep copy** allocates **new memory** for the copy and **copies the pointed-to data** into it. Each object owns its own independent copy of the resource.

```cpp
class Deep {
private:
    int* data;
public:
    Deep(int d) {
        data = new int{d};
    }
    ~Deep() {
        delete data;
    }
    Deep(const Deep& source)
        : data{new int{*source.data}} {  // deep copy: new allocation + copied data
    }
};
```

Or using delegation:

```cpp
Deep::Deep(const Deep& source)
    : Deep{*source.data} {  // delegate to the int constructor
}
```

Now each object has its own heap integer:

- `obj.data` → heap location A containing `100`.
- `copy.data` → heap location B containing `100`.

Destroying the copy frees location B only; `obj` remains valid.

#### When to use deep copy

You **must** use deep copy when your class:

- Owns a raw pointer to dynamically allocated memory.
- Is responsible for releasing that memory in its destructor.

Otherwise you get the double-free and dangling-pointer problems described above.

#### Best-practice summary

- Prefer **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) — they handle ownership correctly.
- Prefer **STL containers** (`std::vector`, `std::string`) — they have proper copy constructors.
- If you must use raw pointers, **always** write a deep-copying copy constructor (and typically also a copy assignment operator and a destructor — the "Rule of Three", see [2.5](#25-the-rule-of-three)).

---

### 2.4 Copy Assignment Operator

The **copy assignment operator** is called when you assign one existing object to another: `a = b;`. It is distinct from the copy constructor, which creates a **new** object.

Signature:

```cpp
ClassName& operator=(const ClassName& source);
```

Key points:

- Returns a reference to the current object (`*this`) to support chaining: `a = b = c;`.
- Takes a `const` reference parameter.
- Must handle **self-assignment** (`a = a;`) safely.

Example:

```cpp
class Deep {
private:
    int* data;
public:
    Deep(int d) : data{new int{d}} {}
    ~Deep() { delete data; }

    Deep(const Deep& source) : Deep{*source.data} {}

    // Copy assignment
    Deep& operator=(const Deep& source) {
        if (this == &source) {        // self-assignment check
            return *this;
        }
        int* new_data = new int{*source.data};  // allocate and copy first
        delete data;                            // then free the old resource
        data = new_data;
        return *this;
    }
};
```

Allocating before deleting makes the assignment exception-safe: if `new` throws, the original object is left unchanged.

The self-assignment check avoids a pointless allocation and makes the intent explicit. In this particular implementation (allocate-then-delete), self-assignment would also be safe without the check — but if you ever rewrite the operator to delete first and then copy, forgetting the check would free the very data you are about to read from.

> **Note:** If you don't write a copy assignment operator, the compiler generates one that performs member-wise copy assignment. For pointer members, this leads to the same shallow-copy problems as the compiler-generated copy constructor.

---

### 2.5 The Rule of Three

The **Rule of Three** states: if a class needs any one of the following three special member functions, it almost certainly needs **all three**:

1. **Destructor** — to release resources.
2. **Copy constructor** — to perform a deep copy.
3. **Copy assignment operator** — to perform a deep copy on assignment.

Why? Because each of them deals with resource management. If your class owns a resource (for example, a raw pointer), all three must cooperate to avoid leaks, double frees, and dangling pointers. Writing one but not the others usually means you have a bug waiting to happen.

```cpp
// All three are present — Rule of Three satisfied
class ResourceOwner {
public:
    ResourceOwner();                                     // constructor
    ~ResourceOwner();                                    // destructor
    ResourceOwner(const ResourceOwner& other);           // copy constructor
    ResourceOwner& operator=(const ResourceOwner& rhs);  // copy assignment
};
```

If you find yourself writing one of the three, stop and ask: *do I need the other two as well?* Most of the time, the answer is yes.

---

### 2.6 L-values and R-values

To understand move semantics, you need a basic grasp of value categories.

- An **lvalue** (locator value) is an expression that refers to a **named, addressable** object. It has a stable location in memory, and you can take its address with `&`.
  ```cpp
  int x = 100;  // x is an L-value
  int* p = &x;  // OK: x has an address
  ```
- An **rvalue** (read value) is a **temporary** value that has no stable address — for example, the result of an expression, or an unnamed temporary object.
  ```cpp
  int y = 100 + 200;  // 300 is an R-value (temporary)
  // int* p = &(100 + 200);  // ERROR: cannot take the address of a temporary
  ```

A useful intuition:

- **L-value** = something with a name that persists beyond the current expression.
- **R-value** = something temporary that will disappear soon.

In the context of move semantics, **R-values are the temporaries** created by the compiler — for example, `Move{10}` in the `push_back` call below. Because such a temporary is about to be destroyed, it is safe to "steal" its resources instead of copying them.

---

### 2.7 Move Constructors and Move Semantics

#### The problem: unnecessary deep copies

Copy constructors that perform deep copies are **computationally expensive**: they allocate memory and copy data. In many cases, the source object is a **temporary** that will be destroyed immediately after the copy — so all that work is wasted.

Consider:

```cpp
std::vector<Move> vec;
vec.push_back(Move{10});  // temporary Move object created
```

The temporary `Move{10}` is created, then **copied** into the vector, then destroyed. The deep copy of the temporary is pure overhead — the temporary's data could simply be **transferred** to the vector's element instead.

#### R-value references

C++11 introduced **R-value references**, written with `&&`:

```cpp
int&& rref = 200;  // rref is an R-value reference bound to the temporary 200
rref = 300;         // legal: modifies the temporary
```

An R-value reference can bind to a temporary, extending its lifetime and allowing you to modify it.

You cannot bind an R-value reference to an L-value:

```cpp
int x = 100;
int&& rref = x;  // ERROR: x is an L-value
```

#### The move constructor

The **move constructor** takes an **R-value reference** parameter and **steals** the source object's resources instead of copying them.

Signature:

```cpp
ClassName(ClassName&& source);  // note: no const, && instead of &
```

Differences from the copy constructor:

- Parameter is **non-const** — the move constructor modifies the source (to null out its pointers).
- Parameter is an **R-value reference** (`&&`).

Implementation:

```cpp
class Move {
private:
    int* data;
public:
    Move(int d) : data{new int{d}} {}

    ~Move() {
        delete data;
    }

    // Copy constructor (deep copy)
    Move(const Move& source) : Move{*source.data} {
    }

    // Move constructor
    Move(Move&& source) noexcept
        : data{source.data} {       // steal the pointer
        source.data = nullptr;      // null out the source
    }
};
```

What the move constructor does:

1. **Takes ownership** of the source's pointer (`data = source.data`).
2. **Nulls out** the source's pointer (`source.data = nullptr`), so that when the source is destroyed, its destructor does nothing (deleting `nullptr` is safe).
3. No memory allocation, no data copying — **extremely efficient**.

> The `noexcept` specifier tells the compiler the move constructor won't throw. This enables important optimizations: for example, when a `std::vector` reallocates, it will **prefer to move** elements rather than copy them — but only if the move constructor is `noexcept` (or the copy constructor is unavailable). If the move constructor can throw and a copy is possible, `vector` falls back to copying to preserve its strong exception-safety guarantee.

#### When is the move constructor called?

The compiler calls the move constructor when the argument is an **R-value** (a temporary). When the argument is an **L-value** (a named object), the copy constructor is called.

```cpp
Move a{10};
Move b{a};              // L-value: copy constructor
Move c{Move{20}};       // prvalue: guaranteed copy elision in C++17; no move constructor call
Move d{std::move(a)};   // std::move casts a to an rvalue (xvalue): move constructor
```

`std::move` does not move anything by itself — it is just a cast to an rvalue (more precisely, an xvalue) that tells the compiler: *"treat this as a temporary; I don't need it anymore."* After `std::move(a)`, you must not rely on `a`'s value (except to destroy it or assign to it).

> **Note on the simplified `Move` example:** after a move, `source.data` is set to `nullptr`. Copying such a moved-from object with the copy constructor `Move(const Move& source) : Move{*source.data} {}` would dereference a null pointer. A moved-from object should only be destroyed or assigned to, unless the class documents a stronger moved-from state.

---

### 2.8 Move Assignment Operator

Like the copy assignment operator, the **move assignment operator** handles assignment, but it **steals** resources from an R-value source instead of copying them.

Signature:

```cpp
ClassName& operator=(ClassName&& source) noexcept;
```

Example:

```cpp
class Move {
private:
    int* data;
public:
    Move(int d) : data{new int{d}} {}
    ~Move() { delete data; }

    Move(const Move& source) : Move{*source.data} {}
    Move(Move&& source) noexcept : data{source.data} {
        source.data = nullptr;
    }

    // Move assignment
    Move& operator=(Move&& source) noexcept {
        if (this == &source) {
            return *this;
        }
        delete data;                  // free the existing resource
        data = source.data;           // steal the source's pointer
        source.data = nullptr;        // null out the source
        return *this;
    }
};
```

Key steps:

1. Self-assignment check.
2. Free the current object's resource.
3. Take ownership of the source's resource.
4. Leave the source in a safe, empty state.

---

### 2.9 The Rule of Five and the Rule of Zero

The **Rule of Five** extends the Rule of Three to include the move operations:

1. **Destructor**
2. **Copy constructor**
3. **Copy assignment operator**
4. **Move constructor**
5. **Move assignment operator**

If you define any one of them for a resource-owning class, you should consider defining all five. This is not just a style preference: the C++ standard specifies that if you declare a destructor, a copy constructor, or a copy assignment operator, the compiler will **not automatically generate** the move operations for you. Your class then silently falls back to copying — which may be slow or outright wrong. Being explicit about all five avoids this surprise.

#### The Rule of Zero

The **Rule of Zero** says: if your class does not directly manage resources, do not write any of the five special member functions. Let the compiler generate them.

This is the preferred modern style:

- Use `std::string`, `std::vector`, `std::unique_ptr`, `std::shared_ptr`, etc. as members.
- These types already implement correct copy/move/destroy behavior.
- Your class then gets correct behavior "for free" from the compiler-generated special members.

```cpp
// Rule of Zero: no special members needed
class Player {
private:
    std::string name;                    // manages its own memory
    std::vector<int> inventory;          // manages its own memory
    std::unique_ptr<Weapon> weapon;      // manages its own memory
public:
    Player(std::string n) : name{std::move(n)} {}
};
```

The compiler-generated destructor, move constructor, and move assignment all behave correctly. Because `std::unique_ptr` is move-only, the compiler-generated **copy constructor and copy assignment are implicitly deleted** — `Player` is a move-only type. If you need `Player` to be copyable, either use `std::shared_ptr` for `weapon`, or implement a deep copy and follow the Rule of Five.

> **Guideline:** Prefer the Rule of Zero. Only fall back to the Rule of Five (or Three) when you genuinely need to manage a raw resource yourself.

---

### 2.10 `= default` and `= delete`

C++ lets you explicitly control the generation of special member functions.

#### `= default`

You can ask the compiler to generate the default implementation:

```cpp
class Player {
public:
    Player() = default;                          // compiler-generated default constructor
    Player(const Player&) = default;             // compiler-generated copy constructor
    Player& operator=(const Player&) = default;  // compiler-generated copy assignment
    Player(Player&&) = default;                  // compiler-generated move constructor
    Player& operator=(Player&&) = default;       // compiler-generated move assignment
    ~Player() = default;                         // compiler-generated destructor
private:
    std::string name;
    int health = 0;
};
```

This is useful when you want to be explicit about which special members exist, without writing the bodies yourself. Note that declaring a destructor, a copy constructor, or a copy assignment operator **suppresses the implicit move operations** — so if you want moves to be generated, declare them with `= default` as shown above (or omit the destructor and copy operations entirely, and let the compiler generate all six).

#### `= delete`

You can forbid a special member function entirely:

```cpp
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;             // no copying
    NonCopyable& operator=(const NonCopyable&) = delete;  // no copy assignment
};
```

Attempting to copy a `NonCopyable` object produces a compile-time error. This is the idiomatic way to express "this class must not be copied" — much clearer than the pre-C++11 technique of declaring a private copy constructor without a definition.

---

### 2.11 Copy Elision and Return Value Optimization (RVO)

Modern compilers are aggressive about eliminating unnecessary copies. When you step through code in a debugger, you may see the copy constructor (or move constructor) **not** being called even though the source code suggests it should be. This is called **copy elision**. A common form is **Return Value Optimization (RVO)**:

```cpp
Move createMove() {
    Move temp{100};
    return temp;  // NRVO: the compiler may construct the result directly
}

Move m = createMove();  // C++17: the returned prvalue initializes m directly
```

There are two related cases:

- **Unnamed RVO / prvalue elision**: a temporary prvalue directly initializes an object, e.g. `Move m{Move{100}};` or `Move m = createMove();` (the returned prvalue initializes `m`). Since C++17, this elision is **guaranteed** by the standard.
- **Named RVO (NRVO)**: a named local variable is returned, e.g. `return temp;` above. This is still an **optional** optimization, not guaranteed by C++17. If the compiler does not elide it, the move constructor (or copy constructor, if no move is available) is used.

This is legal and beneficial. Don't be confused if your debugger shows fewer constructor calls than you expect — it is the compiler doing its job.

---

### 2.12 Part 2 Summary

| Concept | Key Point |
|---|---|
| Copy constructor | `const ClassName&`; creates a new object from an existing one; member-wise (shallow) by default |
| Shallow copy | Copies pointers, not pointed-to data; leads to double-free and dangling pointers |
| Deep copy | Allocates new memory and copies data; required for owning raw pointers |
| Copy assignment | `operator=(const ClassName&)`; must handle self-assignment; returns `*this` |
| Rule of Three | If you need a destructor, copy constructor, or copy assignment, you likely need all three |
| L-value vs R-value | L-value has a stable address; R-value is a temporary |
| R-value reference | `T&&`; binds to temporaries; enables move semantics |
| Move constructor | `ClassName&&`; steals resources, nulls out source; mark `noexcept` |
| Move assignment | `operator=(ClassName&&)`; like the move constructor but for assignment |
| Rule of Five | Destructor + copy ctor + copy assign + move ctor + move assign |
| Rule of Zero | Prefer members that manage their own resources; write no special members |
| `= default` / `= delete` | Explicitly request or forbid compiler-generated special members |
| Copy elision / RVO | Compiler may eliminate copies; guaranteed in many cases since C++17 |

---

## Part 3: Advanced Class Features

### 3.1 Friends

A **friend** of a class is a function or another class that is granted **access to the private (and protected) members** of the class. The friend is **not a member** of the class — it is an outsider that has been explicitly given permission to reach into the class internals.

#### Why do friends exist?

There are legitimate cases where non-member functions need to access private members:

- **Operator overloading** — for example, `operator<<` for stream output often needs access to private data.
- A closely cooperating class — for example, a `Matrix` class and a `Vector` class that need to access each other's internals.
- A test harness that needs to inspect private members.

The alternative — exposing private members via public getters and setters — can be less efficient (function-call overhead) or may leak implementation details. Friends provide **controlled access** without public exposure.

#### Declaring friends

Inside the class declaration, use the `friend` keyword:

```cpp
class Player {
    friend void displayPlayer(const Player& p);   // friend function
    friend class GameEngine;                       // friend class
private:
    std::string name;
    int health;
    int xp;
public:
    Player(std::string n, int h, int x) : name{n}, health{h}, xp{x} {}
};
```

Now `displayPlayer` can access `p.name`, `p.health`, and `p.xp` directly:

```cpp
void displayPlayer(const Player& p) {
    std::cout << p.name << " " << p.health << " " << p.xp << std::endl;
}
```

The friend declaration tells the compiler that this function is a friend, even if the function is defined elsewhere. Note that a friend declaration does not make the function visible to ordinary name lookup the way a namespace-scope declaration does; in many cases you still need a matching declaration at namespace scope, or you rely on argument-dependent lookup (ADL).

#### Key properties of friendship

1. **Friendship is granted, not taken.** A class must explicitly declare its friends. You cannot make yourself a friend of another class.

2. **Friendship is not symmetric.** If `A` declares `B` as a friend, `B` can access `A`'s private members — but `A` cannot access `B`'s private members unless `B` also declares `A` as a friend.

3. **Friendship is not transitive.** If `A` is a friend of `B`, and `B` is a friend of `C`, that does **not** mean `A` is a friend of `C`.

4. **Friendship is not inherited.** Derived classes do not inherit their base class's friendships.

#### Friend member functions

You can declare a specific member function of another class as a friend:

```cpp
class Player;  // forward declaration

class GameEngine {
public:
    void displayPlayer(const Player& p);
};

class Player {
    friend void GameEngine::displayPlayer(const Player& p);
private:
    std::string name;
    int health;
    int xp;
};
```

#### The encapsulation debate

Friends are controversial because they seem to violate encapsulation — a core principle of OOP that says class internals should be hidden. Whether friends **violate** or **enhance** encapsulation is a matter of design philosophy:

- **Enhancement view**: friends allow fine-grained access control — you can grant exactly the access needed to exactly the right function, without making members public.
- **Violation view**: friends break the "one true owner" model of class data and create hidden coupling between classes.

The practical consensus: use friends sparingly. Grant **minimal necessary friendship** only when it genuinely makes sense in your system design. Don't use friends merely to avoid writing getters — that is a code smell.

---

### 3.2 The `this` Pointer

Every non-static member function has access to a hidden parameter called `this`, which is a **pointer to the current object** — the object on which the member function was called.

```cpp
class Account {
private:
    double balance;
public:
    void setBalance(double bal) {
        this->balance = bal;  // this points to the current Account object
    }
};
```

#### Key facts about `this`

- `this` is a **reserved keyword**, available only inside non-static member functions.
- It is a **pointer** to the current object (`Account*` in the example above).
- In many other languages, the equivalent is `self` or `me`.
- All member access is implicitly done via `this` — when you write `balance = 0;`, the compiler actually generates `this->balance = 0;`.

#### When you must use `this`

You need `this` when a parameter name **shadows** a member name:

```cpp
void setBalance(double balance) {
    balance = balance;        // WRONG: both refer to the parameter
    this->balance = balance;  // CORRECT: this->balance is the member
}
```

#### Using `this` for object identity

You can compare `this` with another object's address to check whether two references refer to the same object:

```cpp
bool compareBalance(const Account& other) const {
    if (this == &other) {
        return true;  // same object: balances are trivially equal
    }
    return balance == other.balance;
}
```

This is a useful optimization when comparing objects — if they are the same object, you can skip expensive comparison logic.

#### Dereferencing `this`

Since `this` is a pointer, `*this` gives you the current object itself. This is used in cases like the assignment operator (which returns `*this` by reference) and in some design patterns.

---

### 3.3 `const` Correctness in Classes

The `const` qualifier can be applied to class member functions to indicate that the function **does not modify the object**. This is an important part of **const correctness** — a discipline that uses `const` systematically to make code safer and more self-documenting.

#### Const objects

A `const` object cannot be modified after construction:

```cpp
const Player villain{"Villain", 100, 55};
// villain.health = 200;  // ERROR: cannot modify a const object
```

#### Calling methods on const objects

Calling a **non-const** member function on a `const` object is a compile error:

```cpp
const Player villain{"Villain", 100, 55};
villain.setName("Super Villain");  // ERROR: setName might modify villain
```

But what about **getters** that don't modify the object?

```cpp
std::cout << villain.getName() << std::endl;  // ERROR too!
```

Why? Because the compiler cannot know that `getName` doesn't modify the object — unless you **tell it**.

#### The `const` member function qualifier

You declare a member function as `const` by placing the `const` keyword **after** the parameter list (and before the body or semicolon):

```cpp
class Player {
public:
    std::string getName() const;  // const member function
private:
    std::string name;
};
```

A `const` member function:

1. **Can be called on `const` objects** — because it promises not to modify them.
2. **Cannot modify any member variables** (unless they are `mutable`) — the compiler enforces this.
3. **Cannot call non-const member functions** on the object.

With `getName` declared `const`, this works:

```cpp
const Player villain{"Villain", 100, 55};
std::cout << villain.getName() << std::endl;  // OK
```

#### Const-correctness guidelines

- **All getters should be `const`.**
- **Any member function that does not modify the object should be `const`.**
- Member functions that do modify the object (setters, mutators) should **not** be `const`.
- If you have both a `const` and a non-`const` overload, the compiler chooses the appropriate one based on the object's constness.

#### `mutable` members

Occasionally you need a member that can be modified even in a `const` function — for example, a cache or a mutex. The `mutable` keyword allows this:

```cpp
class DataProcessor {
private:
    mutable std::mutex mtx;  // can be locked in const methods
    mutable int cache = 0;   // can be updated in const methods
public:
    int process() const {
        std::lock_guard<std::mutex> lock(mtx);  // OK: mtx is mutable
        cache++;                                 // OK: cache is mutable
        return cache;
    }
};
```

Use `mutable` sparingly — it breaks the "const means no modification" guarantee.

#### Const correctness is a discipline

Const correctness can become complex, but the core idea is simple: **use `const` wherever you can**, and the compiler will catch accidental modifications. This leads to safer code and clearer interfaces.

---

### 3.4 Static Class Members

**Static members** belong to the **class itself**, not to any particular object. They exist independently of objects and are shared across all instances of the class.

#### Static data members

Declare a static data member by prefixing the declaration with `static`:

```cpp
// player.h
class Player {
private:
    static int num_players;  // shared by all Player objects
    std::string name;
    int health;
    int xp;
public:
    static int get_num_players();  // static member function
};
```

#### Initializing static data members

Static data members must be **defined and initialized once** outside the class, typically in the `.cpp` file:

```cpp
// player.cpp
int Player::num_players = 0;  // definition + initialization
```

Before C++17, you **cannot** initialize a non-const static data member inside the class declaration — the compiler will error out. The member must be defined and initialized once outside the class (typically in the `.cpp` file), which ensures a single definition across all translation units and avoids linker errors.

> **Note:** `static constexpr` members can be initialized inline. Since C++17, a non-const static data member can also be initialized inside the class if it is declared `static inline`, for example `static inline int num_players = 0;`.

#### Static member functions

A static member function:

- Is declared with `static`.
- Is called on the **class itself**, not on an object: `Player::get_num_players()`.
- Has **no `this` pointer**, so it **cannot directly access** non-static data members or non-static member functions. It can only directly access static data members and other static member functions — though it can still operate on non-static members through an object or pointer passed in as a parameter.

```cpp
// player.cpp
int Player::get_num_players() {
    return num_players;  // only static members accessible
}
```

#### A common use case: object counting

A classic use case for static members is **counting active objects**. The constructor increments the counter; the destructor decrements it:

```cpp
// player.cpp
Player::Player(std::string name_val, int health_val, int xp_val)
    : name{name_val}, health{health_val}, xp{xp_val} {
    ++num_players;  // count construction
}

Player::~Player() {
    --num_players;  // count destruction
}

int Player::get_num_players() {
    return num_players;
}
```

Now you can ask the class how many players exist, without needing a specific object:

```cpp
std::cout << "Active players: " << Player::get_num_players() << std::endl;
```

This approach is much more reliable than using a global variable, because the increment/decrement logic is encapsulated in the class and automatically triggered by the compiler when objects are created and destroyed.

> **Careful with overloaded constructors and copy/move operations:** If you have multiple constructors, make sure every path that creates an object increments the counter (and every path that destroys it decrements). The same applies to the copy constructor and the move constructor — they also create new objects, so they must maintain the counter too. Using delegating constructors simplifies this: put the increment in the master constructor.

#### When to use static members

- Data that is shared by all instances (counters, shared configuration, class-wide constants).
- Utility functions that operate on the class level, not on a specific instance.
- Factory methods that create objects.

Avoid static members for data that should logically be per-object — that is what non-static members are for.

---

## Part 4: Structs vs Classes

### 4.1 The Main Technical Difference

In C++, a `struct` and a `class` are almost identical. The **main** language-level difference is the default member access level:

- Members of a `class` are **private** by default.
- Members of a `struct` are **public** by default.

A related difference is the default inheritance access: `class` inherits privately by default, while `struct` inherits publicly by default.

Everything else — constructors, destructors, inheritance, methods, static members, friends — works exactly the same for both. In every other respect, `struct` and `class` are interchangeable.

```cpp
// Class: name and getName are private by default
class PersonClass {
    std::string name;
    std::string getName() { return name; }
};

// Struct: name and getName are public by default
struct PersonStruct {
    std::string name;
    std::string getName() { return name; }
};

int main() {
    PersonClass pc;
    // pc.name = "Alice";  // ERROR: private

    PersonStruct ps;
    ps.name = "Alice";     // OK: public
    ps.getName();           // OK: public
}
```

### 4.2 When to Use Which

Since the technical difference is trivial, the choice between `struct` and `class` is about **communicating intent** to other programmers.

**Guidelines:**

- Use a **`struct`** when the type is primarily a **passive data container** — a bundle of public data with little or no behavior. This mirrors C's `struct` and is common for small records (for example, `Point { double x, y; }`, `Color { int r, g, b; }`).

  ```cpp
  struct Point {
      double x;
      double y;
  };
  ```

- Use a **`class`** when the type has **invariants, behavior, or complex internal state** — when you want to encapsulate data and provide controlled access through methods.

  ```cpp
  class BankAccount {
  private:
      double balance;  // invariant: balance >= 0
  public:
      void deposit(double amount);
      void withdraw(double amount);  // enforces the invariant
      double getBalance() const;
  };
  ```

**Don't** use a `struct` and then declare everything `private` — that is confusing. If you need encapsulation, use a `class`. Other C++ programmers have strong expectations about these keywords; follow the conventions.

---

## Summary

This tutorial covered the essential features of C++ classes beyond basic member variables. Rather than repeat every definition, here are the **core design principles** that tie everything together:

1. **Prefer initialization lists** over assignment in constructor bodies. They are more efficient, and they are the only way to initialize `const` members, reference members, and members without default constructors.

2. **Prefer smart pointers and STL containers** over raw pointers. This lets you follow the Rule of Zero: let the compiler generate the destructor, copy constructor, copy assignment, move constructor, and move assignment, and they will all behave correctly.

3. **Follow the Rule of Three or Five** when you must manage a resource manually. If you write a destructor, a copy constructor, or a copy assignment operator, you almost certainly need the others — and if you want efficient moves, you need the move operations as well.

4. **Use `const` wherever possible.** Const-correctness catches accidental modifications at compile time and documents your interface. Declare every getter and every non-mutating member function `const`.

5. **Use friends sparingly.** Grant minimal friendship only when the design genuinely calls for it — typically for operator overloading or tightly cooperating classes. Do not use friends merely to avoid writing getters.

6. **Understand copy and move semantics.** They are the foundation of efficient C++ resource management. A move constructor that steals resources instead of copying them can turn an O(n) operation into an O(1) operation.

7. **Let the compiler help you.** `= default` makes your intent explicit; `= delete` forbids operations that should not exist; `noexcept` on move operations enables important optimizations. Copy elision and RVO mean that many copies you write in source code never actually happen.

When in doubt, start from the Rule of Zero: build your classes from members that manage their own resources, and let the compiler generate the special member functions. Only reach for manual resource management — and the Rule of Five — when you have a concrete reason to do so.
