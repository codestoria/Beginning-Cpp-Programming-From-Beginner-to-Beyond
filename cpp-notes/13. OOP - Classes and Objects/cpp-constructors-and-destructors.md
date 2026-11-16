# C++ OOP: Constructors and Destructors

## Table of Contents

1. [What Is a Constructor, and Why Do We Need One?](#1-what-is-a-constructor-and-why-do-we-need-one)
2. [The Default Constructor](#2-the-default-constructor)
3. [Overloading Constructors](#3-overloading-constructors)
4. [Constructor Initialization Lists](#4-constructor-initialization-lists)
5. [Delegating Constructors](#5-delegating-constructors)
6. [Constructor Parameters and Default Values](#6-constructor-parameters-and-default-values)
7. [Destructors](#7-destructors)
8. [Summary](#8-summary)

---

## 1. What Is a Constructor, and Why Do We Need One?

### 1.1 The Problem: Raw Memory Is Not Yet an Object

When an object is created, the compiler's first step is to **allocate raw memory** for it. At that moment, the bytes in that memory are indeterminate — they hold whatever bit patterns happened to be left over from previous use of that memory region. For your class, those bytes **have no meaning yet**.

Consider what happens if a class has no constructor:

```cpp
class Player {
public:
    std::string name;
    int health;
    int xp;
};

int main() {
    Player p;                 // memory allocated, but health and xp are indeterminate
    std::cout << p.health;    // undefined behavior: reading an uninitialized int
}
```

Two different things happen to the two kinds of members here:

- `std::string name;` — `std::string` is a **class type**. It has its own constructor, and the compiler automatically calls it. So `name` becomes a valid, empty string.
- `int health;` and `int xp;` — these are **built-in types**. They have no constructor. Nothing is run for them, so they keep whatever bits were already in memory. **Reading such a value is undefined behavior** — your program may print garbage, crash, or appear to work and then misbehave later.

This is the fundamental motivation for constructors: **raw memory is not a valid object until you make it one.**

### 1.2 The Solution: A Constructor Establishes a Known State

A **constructor** is a special member function whose job is to turn that raw memory into a valid, initialized object. It gives you a place to set every member to a known value:

```cpp
class Player {
private:
    std::string name;
    int health;
    int xp;
public:
    Player() {          // <-- constructor
        name   = "None";
        health = 0;
        xp     = 0;
    }
};
```

Now every `Player` that is created starts life in a well-defined state. No garbage. No undefined behavior.

### 1.3 Key Properties of Constructors

| Property | Explanation |
| --- | --- |
| **Same name as the class** | `Player()` constructs a `Player`. |
| **No return type** (not even `void`) | A constructor is not called to produce a value; its only effect is to initialize the object being created. The expression `Player()` creates a `Player` object — it does not denote a value returned by the constructor. |
| **Can be overloaded** | You may define several constructors with different parameter lists. The compiler selects the right one. (Section 3 covers this.) |
| **Called automatically** | You never call a constructor directly. The compiler invokes it during object creation. |

> **Why "no return type"?**
> A constructor's *only* observable effect is to initialize the object being created. The expression `Player()` has type `Player` because it *is* a `Player` object, not because the constructor returns anything. Giving constructors a return type would blur the distinction between "creating an object" and "calling a function that yields a value".

---

## 2. The Default Constructor

### 2.1 Definition

A **default constructor** is a constructor that can be called **with no arguments**. It is also called the *no-args constructor*.

### 2.2 The Compiler-Generated Default Constructor

If you declare **no constructors at all**, the compiler implicitly declares a default constructor for you. If the implicitly-declared default constructor would be ill-formed — for example, because a base class is not default-constructible, or because a non-static data member of reference type (or of a class type that is not default-constructible) has **no default member initializer** — it is defined as **deleted**, and any use of it is an error. When it *is* usable, this generated constructor does only limited work:

- It **default-constructs** class-type members (for example, `std::string` becomes empty).
- It **leaves built-in members** (`int`, `double`, pointers) with **indeterminate values**, unless the member has a **default member initializer** (e.g. `int health = 100;`), in which case that initializer is used.

Note that the second point applies to **default-initialization**, such as `Account a;` or `new Account`. With **value-initialization**, such as `Account a{};` or `new Account()`, built-in members are zero-initialized first — **provided the class has no user-provided default constructor**. If a user-provided default constructor exists, it alone runs and no prior zero-initialization happens.

```cpp
class Account {
private:
    std::string name;   // default-constructed to an empty string
    double balance;     // indeterminate!
    // No constructors defined.
};

int main() {
    Account frank;               // OK: compiler-generated default constructor
    Account* jim = new Account;  // Also OK
    // frank.balance holds garbage — do not read it!
}
```

> ⚠️ **Common pitfall**
> The compiler-generated default constructor does **not** initialize built-in members. Reading such a member is undefined behavior. This is one of the most common sources of subtle bugs in C++ — the program seems to work in debug builds and then crashes in release builds.

### 2.3 Write Your Own Default Constructor

The safe approach is to write a no-args constructor so that every object starts in a known state:

```cpp
class Account {
private:
    std::string name;
    double balance;
public:
    Account() {            // your own default constructor
        name    = "None";
        balance = 0.0;
    }
};
```

### 2.4 The Critical Rule: Defining *Any* Constructor Removes the Implicit Default

This is one of the most important rules in this part:

> **Once you declare any user-declared constructor — even one that takes arguments — the compiler no longer implicitly declares a default constructor.**

If your code still needs a no-args constructor, **you must write it yourself**.

```cpp
class Account {
private:
    std::string name;
    double balance;
public:
    Account(std::string name_val, double balance_val) {
        name    = name_val;
        balance = balance_val;
    }
    // No no-args constructor defined!
};

int main() {
    Account frank;                // ERROR: no default constructor available
    Account bill("Bill", 100.0);  // OK
}
```

> **Why this rule?**
> Once you have declared a constructor, the compiler assumes that a default-constructed object may not make sense for your class. For example, an `Account` with no owner and no balance might be meaningless. If you *do* want a default constructor in addition to your parameterized one, write it explicitly (`Account() = default;` if the generated one is exactly what you want). The language forces you to be intentional.

---

## 3. Overloading Constructors

A class can have **as many constructors as needed**, as long as each has a **unique signature** (a unique parameter list). The compiler selects the correct one based on the arguments you provide at the point of object creation.

```cpp
class Player {
private:
    std::string name;
    int health;
    int xp;
public:
    Player() { name = "None"; health = 0; xp = 0; }

    Player(std::string name_val) {
        name   = name_val;
        health = 0;
        xp     = 0;
    }

    Player(std::string name_val, int health_val, int xp_val) {
        name   = name_val;
        health = health_val;
        xp     = xp_val;
    }
};

int main() {
    Player  empty;                                 // calls the no-args constructor
    Player  hero("Hero");                          // calls the one-string constructor
    Player  frank("Frank", 100, 4);                // calls the three-arg constructor
    Player* enemy = new Player("Enemy", 1000, 0);  // heap allocation, three-arg constructor
}
```

> **What if the compiler cannot decide?**
> If two constructors could both accept the given arguments, the compiler produces an **ambiguity error** rather than guessing. We will see a concrete example in [Section 6](#6-constructor-parameters-and-default-values).

---

## 4. Constructor Initialization Lists

### 4.1 The Problem with Assignment Inside the Body

So far we have initialized members by assigning values **inside the constructor body**:

```cpp
Player::Player() {
    name   = "None";   // assignment, not initialization
    health = 0;
    xp     = 0;
}
```

This *works*, but it is not true initialization. **By the time the constructor body runs, every member has already been initialized: class-type members by their default constructor (or by a default member initializer, if one exists), and built-in members by a default member initializer if one exists — otherwise they are left indeterminate.** So `name = "None"` does not mean *"create `name` with the value `"None"`"*. It means *"create `name` as an empty string, then replace its contents with `"None"`"*.

For a `std::string`, that sequence is:

1. The default constructor runs, allocating a small internal buffer (or none).
2. The assignment operator runs, possibly allocating a new buffer and copying `"None"` into it.

For complex types, this can be wasteful: you may allocate memory, immediately free it, and allocate again.

### 4.2 The Initialization-List Syntax

C++ lets you initialize members **directly, before the constructor body runs**, using a **constructor initialization list**:

```cpp
Player::Player()
    : name{"None"}, health{0}, xp{0} {
    // constructor body (can be empty)
}
```

The syntax:

1. After the parameter list, write a colon `:`.
2. Follow it with a **comma-separated list** of `member{value}` (or `member(value)`) initializers.
3. Then write the constructor body (which may be empty).

Now `name` is initialized *directly* to `"None"` — the empty string is **never created**, so there is no wasted allocation.

### 4.3 Declaration Order Matters, Not Initialization-List Order

This is a subtle rule that trips up many beginners:

> **Members are initialized in the order they are declared in the class, not the order they appear in the initialization list.**

```cpp
class Example {
private:
    int a;   // declared first
    int b;   // declared second
public:
    Example() : b{2}, a{1} {}  // a is initialized first, then b
};
```

Here, `a` is initialized before `b` because `a` is *declared* first — the order `b{2}, a{1}` in the list is **ignored**. The compiler may warn you when the initialization-list order differs from the declaration order.

> **Best practice:** keep the initialization list in the same order as the member declarations.

### 4.4 Initializing with Parameters

The same technique works for parameterized constructors:

```cpp
Player::Player(std::string name_val, int health_val, int xp_val)
    : name{name_val}, health{health_val}, xp{xp_val} {
}
```

### 4.5 When Initialization Lists Are *Mandatory*

For some members, **assignment inside the constructor body is impossible**. You *must* use an initialization list:

| Member kind | Why assignment is impossible |
| --- | --- |
| `const` members | They cannot be assigned after construction. |
| Reference members | They must be bound at construction and cannot be rebound. |
| Members of types without a default constructor | They must be given arguments at construction — unless a default member initializer supplies them. |

```cpp
class Config {
private:
    const int    max_players;   // must be initialized in the list
    std::string& log_file;      // reference member: must be initialized in the list
public:
    Config(int max, std::string& file)
        : max_players{max}, log_file{file} {   // required
    }
};
```

### 4.6 Rule of Thumb

- For **built-in types**, the difference between assignment and initialization is usually small in performance: the member is initialized (left indeterminate if there is no default member initializer) and then assigned. The main benefits of an initialization list here are **clarity** and **avoiding an indeterminate intermediate value**.
- For **class-type members**, the difference can be **significant** — you may save an allocation/free pair.

> ✅ **Rule of thumb:** always use initialization lists unless you have a specific reason not to.

---

## 5. Delegating Constructors

### 5.1 The Problem: Duplicated Initialization Logic

When you overload constructors, you often end up **duplicating** initialization logic:

```cpp
Player::Player()
    : name{"None"}, health{0}, xp{0} {}

Player::Player(std::string name_val)
    : name{name_val}, health{0}, xp{0} {}

Player::Player(std::string name_val, int health_val, int xp_val)
    : name{name_val}, health{health_val}, xp{xp_val} {}
```

The three initialization lists are nearly identical. **Duplicated code is a bug magnet:** when you change the initialization logic, you might forget to update every constructor.

### 5.2 The Solution: Delegation

C++ solves this with **delegating constructors**: one constructor can call **another constructor of the same class** from its initialization list.

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

### 5.3 Rules for Delegation

- After the parameter list, write `:` followed by the **class name** and the arguments to the target constructor.
- You **cannot also initialize members** in the same initialization list — delegation replaces member initialization.
- Delegation only works in the **initialization list**, not in the constructor body.
- If constructor A delegates to constructor B, **both bodies run**: B's body first, then A's.

### 5.4 Why This Matters

This eliminates duplication. Change the master constructor, and **all delegating constructors follow automatically**. The class has exactly one place where the real initialization logic lives.

---

## 6. Constructor Parameters and Default Values

### 6.1 Default Parameters

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
Player empty;                          // name="None",  health=0,   xp=0
Player hero{"Hero"};                   // name="Hero",  health=0,   xp=0
Player frank{"Frank", 100};            // name="Frank", health=100, xp=0
Player villain{"Villain", 100, 55};    // all three supplied
```

This one constructor replaces the no-args, one-arg, two-arg, and three-arg constructors from earlier sections.

### 6.2 Ambiguity Warning

If you combine default parameters with other constructors, be careful not to create **ambiguous overloads**:

```cpp
class Player {
public:
    Player(std::string name_val = "None", int health_val = 0, int xp_val = 0);
    Player();  // AMBIGUOUS with the above when called with no arguments!
};
```

For `Player p;`, both constructors are viable: the no-args constructor accepts zero arguments, and the constructor with default parameters can also be called with zero arguments (all parameters use their defaults). Because both are equally valid, the compiler cannot decide which one to call and emits an error.

> **Guidance:** default parameters are convenient when they make sense, but sometimes you want to *force* the caller to provide certain values explicitly. Use them judiciously, and avoid mixing them with a competing no-args constructor.

---

## 7. Destructors

### 7.1 What Is a Destructor?

A **destructor** is a special member function called **automatically when an object is destroyed**. It is the counterpart to the constructor:

- The **constructor** establishes the object's initial state (and may acquire resources).
- The **destructor** releases those resources and performs any final cleanup.

### 7.2 Key Properties

| Property | Explanation |
| --- | --- |
| **Same name as the class, preceded by `~`** | `~Player()` destroys a `Player`. |
| **No return type and no parameters** | Same reasoning as for constructors: there is no value to return and no arguments to supply. |
| **Cannot be overloaded** | Exactly **one** destructor per class. |
| **Called automatically** | You never call it directly. |

A destructor is called automatically when:

- A local (stack) object **goes out of scope**.
- `delete` is called on a pointer to a **dynamically allocated** object.
- An object **containing a member object** is destroyed; the member object's destructor runs as part of the containing object's destruction.

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

> **Why can't destructors be overloaded?**
> Because the **compiler** decides when to call the destructor — you do not supply arguments. If there were multiple destructors, the compiler would have no way to choose one.

### 7.3 What Goes in a Destructor?

The destructor is the right place to:

- Release dynamically allocated memory (`delete` / `delete[]`).
- Close files or network connections.
- Release other resources (locks, handles, etc.).

If you don't write a destructor, the compiler generates one that performs no custom cleanup. It still destroys all member objects (in reverse order of their declaration) and then all base-class subobjects (in reverse order of construction).

### 7.4 Order of Destruction

When a block of code ends, local objects are destroyed in **reverse order of their creation** (LIFO — last in, first out). This mirrors the stack discipline:

```cpp
{
    Player frank;    // constructed first
    Player hero;     // constructed second
    Player villain;  // constructed third
}   // villain destroyed first, then hero, then frank
```

Understanding this order matters when objects depend on each other. For example, if `hero` holds a **reference** to `frank`, you need to know that `frank` outlives `hero` — which is guaranteed by the reverse-order rule.

---

## 8. Summary

| Concept | Key point |
| --- | --- |
| **Constructor** | Same name as the class, no return type, can be overloaded; turns raw memory into a valid object. |
| **Default constructor** | Implicitly declared only if there are *no* user-declared constructors; it may be defined as deleted. Write your own to avoid indeterminate values. |
| **Initialization list** | True initialization, more efficient than assignment; **required** for `const`, reference, and non-default-constructible members. |
| **Declaration order** | Members are initialized in declaration order, **not** initialization-list order. |
| **Delegating constructor** | One constructor calls another to eliminate duplicated initialization logic. |
| **Default parameters** | Reduce the number of overloads; watch for ambiguity with other constructors. |
| **Destructor** | One per class, no parameters; releases resources; called in reverse order of construction. |

---

### The Big Picture

Every C++ object goes through a **lifecycle**:

1. **Memory is allocated** — raw, meaningless bytes.
2. **A constructor runs** — the object becomes valid and known.
3. **The object is used** — methods are called, members are read and written.
4. **The destructor runs** — resources are released, in reverse order of construction.

Constructors and destructors exist to make **steps 2 and 4 automatic and reliable**. Once you understand *why* they exist — not just *how* to write them — you will naturally reach for initialization lists, delegating constructors, and well-scoped destructors, and you will avoid the most common pitfalls (uninitialized built-in members, missing default constructors, and resource leaks).
