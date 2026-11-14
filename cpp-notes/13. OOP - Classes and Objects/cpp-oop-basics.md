# C++ OOP Basics: Classes, Objects, Encapsulation, and File Organization

## Table of Contents

1. [Why Object-Oriented Programming?](#1-why-object-oriented-programming)
2. [What Are Classes and Objects?](#2-what-are-classes-and-objects)
3. [Declaring a Class and Creating Objects](#3-declaring-a-class-and-creating-objects)
4. [Accessing Class Members](#4-accessing-class-members)
5. [Public and Private: Controlling Access](#5-public-and-private-controlling-access)
6. [Implementing Member Methods](#6-implementing-member-methods)
7. [Separating Specification from Implementation](#7-separating-specification-from-implementation)
8. [Include Guards: Preventing Multiple Inclusion](#8-include-guards-preventing-multiple-inclusion)
9. [Putting It All Together](#9-putting-it-all-together)
10. [Summary](#10-summary)

---

## 1. Why Object-Oriented Programming?

### 1.1 The Procedural Approach and Its Limitations

Before diving into classes and objects, it helps to understand why object-oriented programming (OOP) exists. Up to this point, you have been writing **procedural programs** — programs built around functions.

In procedural programming:
- The **function** is the central building block.
- Data is declared separately from the functions that operate on it.
- Whenever a function needs to process data, that data is passed in as an argument.

This approach works well for small programs, but it has real limitations as programs grow:

**Problem 1: Functions must know the structure of data.**
If you have hundreds of functions and many of them expect a specific data structure, changing that structure forces you to update every affected function. This creates a ripple effect that can be expensive in programmer time and testing effort.

**Problem 2: Programs become harder to understand.**
As procedural programs grow, the web of connections between functions becomes difficult to trace. This makes maintenance, extension, and debugging progressively harder.

**Problem 3: Code becomes fragile.**
When you add new functionality or fix a bug in a large procedural program, the chance of accidentally introducing another bug is high.

**Problem 4: Reusability is difficult.**
Reusing functions and data structures from one program in another is hard because their behavior becomes difficult to visualize over time.

### 1.2 The Object-Oriented Solution

Object-oriented programming addresses these problems by modeling your software in terms of **classes** and **objects** that represent real-world entities in your problem domain.

For example:
- If you are building a photo application, your classes might be `Album`, `Photo`, `Slideshow`, and `Location`.
- If you are building a student enrollment system, your classes might be `Student`, `Course`, and `Professor`.

This is powerful because of **abstraction**. Instead of thinking in terms of `firstName`, `lastName`, `studentID`, and `creditHours`, you can think in terms of a `Student` — and all that detail comes along with it. Operations like `register`, `dropCourse`, and `addCourse` come along with it too.

### 1.3 Key Benefits of OOP

**Encapsulation** means that objects contain both data and the operations that work on that data. The data and operations live together in the class where they belong, rather than being spread across many functions that each pass and receive data.

**Information hiding** takes encapsulation further. OOP allows you to hide implementation-specific logic inside a class so that it is available only within the class. You provide a **public interface** to the class and hide everything else. Users of the class cannot mess with implementation-specific code because they never knew about it in the first place.

**Reusability** means using something from one program in another. Since classes are encapsulated units of data and operations, reusing them in other applications is much easier — and since the class has already been tested, you get that benefit as well.

**Inheritance** allows you to create a new class based on an existing class by only adding or modifying the elements you need. For example, if you have a basic `Account` class with `balance`, `withdraw`, and `deposit`, you can derive a `TrustAccount` class that adds special rules (e.g., limited to three withdrawals per year, each capped at 10% of the balance) without modifying the original `Account` class at all. This leads to reusability and supports **polymorphism**: with **public inheritance**, every derived class is still an `Account`, so code written against `Account` can work with `TrustAccount` too. In C++, *runtime* polymorphism additionally requires that the relevant base-class functions be declared `virtual` and be called through a base-class pointer or reference.

### 1.4 A Word of Caution

OOP is not a panacea:
- OOP won't make bad code better — it will likely make it worse.
- Not everything decomposes neatly into a class.
- For small, short-lived programs (e.g., a quick internal automation script), procedural or scripting approaches may be perfectly appropriate.
- OOP generally has a steeper learning curve than procedural programming.
- OOP programs tend to be larger and can sometimes be slower due to more going on behind the scenes.

However, among object-oriented languages, C++ is one of the most efficient.

---

## 2. What Are Classes and Objects?

### 2.1 Classes as Blueprints

A **class** is a blueprint from which objects are created. It is a **user-defined type**. One of the goals of OOP is to make user-defined types feel like they are part of the programming language — so that when you create your classes, you can use them just like you use `int`, `double`, and so forth: easily and simply.

Classes have:
- **Attributes** (data)
- **Methods** (functions)

Classes can hide data and methods that are only used internally by marking them `private`, while exposing a well-defined interface through `public` members. The goal of a class is to provide a well-defined **public interface** that the user of the class can easily use to solve their problem.

Examples of classes:
- `Account`, `Employee`, `Image` — classes you might create in your applications
- `std::vector`, `std::string` — classes from the C++ standard library

All of these model real-world entities.

### 2.2 Objects as Instances

An **object** is created from a class and represents a specific **instance** of that class. If you have an `Account` class, you can create a `frankAccount` object that is a specific instance modeling Frank's account information. In a real banking application, you could have hundreds of thousands of account objects, each representing an individual account.

Each object has its own **identity**, and each can use the methods defined in the class.

### 2.3 An Analogy with Primitive Types

Consider this code:

```cpp
int highScore;
int lowScore;

Account frankAccount;
Account jimAccount;

std::vector<int> scores;
std::string name;
```

`int` (and other primitive types such as `double`) is not a class — but the analogy helps. When you declare `int highScore`, you are told:
- What the valid values are for that variable
- What operations you can perform on it (addition, subtraction, multiplication, etc.)

Now think of `int` as a "class" and `highScore` and `lowScore` as "objects" created from it. They are instances of integer, each with a value and an identity.

Similarly, `Account` is a user-defined type, and `frankAccount` and `jimAccount` are instances of the `Account` class — they are objects. Each has an identity and values associated with it. The syntax is deliberately similar to primitive types, which makes user-defined types intuitive to use.

---

## 3. Declaring a Class and Creating Objects

### 3.1 Basic Class Declaration Syntax

The syntax for defining a class is simple:

```cpp
class ClassName {
    // attributes and methods
};  // <-- don't forget this semicolon!
```

Key points:
- Use the `class` keyword followed by the class name.
- **Best practice**: capitalize class names for user-defined classes.
- The class body is enclosed in curly braces.
- The declaration **must** be terminated with a semicolon. Forgetting this semicolon produces strange compiler errors because the compiler gets lost trying to figure out where it is.

Inside the class body, you supply the declarations that define the **structure** and **behavior** of your class — its attributes and methods.

Strictly speaking, the form shown above is a **class definition**, because it includes the class body. A **forward declaration** is just `class Account;` — it announces the name without giving the body and is used when only the name is needed (for example, to declare pointers or references to the type). In everyday C++ writing the terms "declaration" and "definition" are often used loosely.

### 3.2 Example: A Player Class

```cpp
class Player {
public:
    std::string name {"Player"};
    int health {100};
    int xp {0};

    void talk(const std::string& text);   // declaration only
    bool isDead() const;                  // declaration only
};
```

These are **declarations only** — the method bodies must be supplied (see Section 6) before the class can be linked into a program.

Here:
- `name`, `health`, and `xp` are **attributes** (instance variables).
- `talk` and `isDead` are **methods**. ...

The in-class initializers (`{"Player"}`, `{100}`, `{0}`) are covered more fully in Section 3.5; they ensure that members are not left uninitialized.

### 3.3 Creating Objects

Once the class is declared, you can create objects that are instances of that class:

```cpp
Player frank;       // frank is a Player object
Player hero;        // hero is a Player object

Player *enemy = new Player();  // enemy is a pointer to a Player on the heap
// ... use enemy ...
delete enemy;       // free the heap storage when done
```

The syntax should feel familiar — it's the same way you create primitive types.

### 3.4 Objects Work Like Any Other Variable

Once you have objects, you can use them like any other variable in C++. For example:

```cpp
// Array of Player objects
Player players[] {frank, hero};

// Vector of Player objects
std::vector<Player> playerVec {frank};
playerVec.push_back(hero);
```

This is powerful: you can create your own user-defined types and use them just like any other variable, which makes your code easy to read and intuitive.

### 3.5 In-Class Initialization (C++11 and Later)

C++11 introduced the ability to provide default initializers directly in the class declaration:

```cpp
class Player {
public:
    std::string name {"Player"};
    int health {100};
    int xp {0};
    // ...
};

class Account {
    std::string name {"Account"};
    double balance {0.0};
    // ...
};
```

Any object created from these classes will get these initial values. This is convenient and we will see later how constructors and initializer lists provide even more control over initialization.

> **Note**: Without initializers, primitive members like `int` and `double` are left
> uninitialized for automatic objects; reading them before assignment is undefined
> behavior. The `std::string` member is empty because `std::string` has a default
> constructor that produces an empty string.

---

## 4. Accessing Class Members

### 4.1 The Dot Operator

In this section we use a simplified `Account`/`Player` whose members are `public`, so that we can focus on the **syntax** of member access. In Section 5 we will see why real classes keep their data `private` and expose `public` methods instead.

```cpp
// Simplified Account used only to demonstrate member-access syntax.
// Note: withdraw() is declared but not defined here — it is not called in this section.
class Account {
public:
    std::string name;
    double balance {0.0};

    bool deposit(double amount) { balance += amount; return true; }
    bool withdraw(double amount);
};

Account frankAccount;               // balance is initialized to 0.0
frankAccount.deposit(1000.0);       // call a method; balance is now 1000.0
frankAccount.balance = 500.0;       // OK here: 'balance' is public in this simplified
                                    // class — direct assignment bypasses deposit();
                                    // this line only demonstrates member-access syntax
```

The object goes to the **left** of the dot operator, so the method is called on that specific object. In this case, `deposit` is called on `frankAccount`.

### 4.2 The Arrow Operator

If you have a **pointer** to an object, you must first dereference the pointer to get to the object. There are two ways to do this:

**Option 1: Explicit dereference with `*` and then `.`**

```cpp
Player *enemy = new Player();
(*enemy).name = "Enemy";
(*enemy).health = 100;
```

The parentheses are required because the dot operator has **higher precedence** than the dereference operator — without them, `*enemy.name` would be parsed as `*(enemy.name)`, which is wrong.

**Option 2: The arrow operator `->`**

```cpp
enemy->name = "Enemy";
enemy->health = 100;
enemy->talk("I will destroy you");
```

The arrow operator (`->`, a dash followed by a greater-than sign) has the same effect as `(*pointer).member` but is much more intuitive to read and write. It is used very often.

### 4.3 Summary

| Situation | Operator | Example |
|-----------|----------|---------|
| Object | `.` | `frank.health` |
| Pointer to object | `->` | `enemy->health` |
| Pointer to object (explicit) | `(*p).` | `(*enemy).health` |

> **Note**: Some class members may not be accessible from outside the class — we'll see why in the next section.

---

## 5. Public and Private: Controlling Access

### 5.1 The Three Access Modifiers

C++ has three basic class member access modifiers:

| Modifier | Accessibility |
|----------|---------------|
| `public` | Accessible everywhere |
| `private` | Accessible only by other members of the same class (or friends of the class) |
| `protected` | Like `private`, but also accessible to derived classes (used with inheritance) |

For now, you can think of `protected` and `private` as being pretty much the same — the difference appears when we use inheritance.

### 5.2 How to Use Access Modifiers

In the body of the class, you specify the access modifier followed by a colon. Everything declared from that point forward has that access modifier applied to it, until you specify a different one:

```cpp
class Player {
private:
    std::string name;
    int health;
    int xp;

public:
    void talk(std::string text);
    bool isDead();
};
```

**Important**: By default, everything in a `class` is **private** if no access modifier is specified.

### 5.3 Why Hide Data?

The idea is to **control access** to certain data and methods. For example, you want to prevent users from directly setting a player's health to 1000 when the maximum health is 100. Instead, you create a public method that sets the health property, and inside that method you enforce the maximum:

```cpp
class Player {
private:
    int health {100};   // default value; avoids reading an uninitialized member

public:
    void setHealth(int h) {
        if (h > 100) h = 100;
        if (h < 0) h = 0;
        health = h;
    }
};
```

Since `setHealth` is a class member, it has access to the private `health` property — but external code does not.

### 5.4 What Happens When You Violate Access?

Simple: you get a **compiler error**. For example:

```cpp
Player frank;
frank.name = "Frank";   // ERROR: 'name' is private within this context
frank.health = 1000;    // ERROR: 'health' is private
```

The same applies whether you use the dot operator or the arrow operator — access control is independent of how you reach the member.

### 5.5 The Debugging Benefit

Private data makes testing and debugging much easier. Consider an account whose balance is somehow 20 million dollars when it should not be. If `balance` is private, the **only** place that error could have occurred is inside the class's own methods. There is no way any other part of the program could have changed that value. You can find and fix the error much more easily.

### 5.6 A Note on Header Files and `using namespace`

When writing class declarations in header files, avoid `using namespace std;`. Instead, be explicit and write `std::string`, `std::vector`, etc. This is because everything that includes the header will inherit the namespace, which is unnecessary and can cause conflicts.

---

## 6. Implementing Member Methods

### 6.1 Methods Are Like Functions

Class methods are implemented in a manner very similar to regular functions — but they are generally **less complex** because member methods have direct access to the class's member attributes. You don't have to pass so many arguments around.

C++ gives you many options regarding **where** to write the code for member methods.

### 6.2 Option 1: Inside the Class Declaration

You can define member methods right inside the class declaration:

```cpp
class Account {
private:
    double balance {0.0};

public:
    void setBalance(double bal) {
        balance = bal;
    }

    double getBalance() const {
        return balance;
    }
};
```

If you do this, the methods become **implicitly inline**. This is okay for small, relatively simple methods. Notice:
- The syntax is exactly as it was for defining regular C++ functions.
- The public methods have direct access to the private `balance` because they are members of the same class.
- `setBalance` sets the balance to the value passed in; `getBalance` returns a copy of the balance.
- These public methods are **necessary** to access `balance` because `balance` is private.

### 6.3 Option 2: Outside the Class Declaration

For larger, more complex methods, we tend to implement them **outside** the class declaration. To do so, we must tell the compiler that we're implementing a method that belongs to a specific class. We use the **class name**, followed by the **scope resolution operator** (`::`), followed by the **method name**:

```cpp
class Account {
private:
    double balance {0.0};

public:
    void setBalance(double bal);  // declaration only
    double getBalance() const;    // declaration only
};

// Implementations outside the class
void Account::setBalance(double bal) {
    balance = bal;
}

double Account::getBalance() const {
    return balance;
}
```

Inside the class declaration, we simply provide the **method prototype** (declaration). This tells the compiler:
- The name of the method
- The parameters it expects
- The return type

The compiler can then **type-check** the method implementation when it sees it later. By prefixing the method name with `Account::`, we unambiguously tell the compiler that these implementations belong to the `Account` class.

### 6.4 Option 3: Separate Files (The Professional Approach)

As programs get larger, it's very common to **separate a class's specification from its implementation**. This makes the class much easier to manage.

The convention:
- A **header file** (`.h` or `.hpp`) contains the class specification (declaration).
- A **source file** (`.cpp`) contains the class implementation (method definitions).

For example:

**account.h** — the specification:

```cpp
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
private:
    std::string name;
    double balance {0.0};

public:
    void setBalance(double bal);
    double getBalance() const;
    bool deposit(double amount);
    bool withdraw(double amount);
};

#endif
```

**account.cpp** — the implementation:

```cpp
#include "account.h"

void Account::setBalance(double bal) {
    balance = bal;
}

double Account::getBalance() const {
    return balance;
}

bool Account::deposit(double amount) {
    if (amount <= 0) return false;
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    return true;
}
```

**main.cpp** — the driver:

```cpp
#include "account.h"

int main() {
    Account frankAccount;
    frankAccount.setBalance(1000.0);
    frankAccount.deposit(200.0);
    frankAccount.withdraw(500.0);
    return 0;
}
```

### 6.5 Angle Brackets vs. Double Quotes

There is an important difference between the two forms of `#include`:

| Form | Use |
|------|-----|
| `#include <iostream>` | System header files — the compiler knows where these are located |
| `#include "account.h"` | Header files local to this project — in most implementations the compiler first searches the directory of the including file, then the configured include paths |

**Rule**: In ordinary class organization, always include `.h` files, never include `.cpp` files. (Template libraries sometimes include `.ipp` / `.tpp` implementation files — that is a later topic.)

### 6.6 How Compilation Works

When the program is compiled:
1. Both `main.cpp` and `account.cpp` are compiled independently.
2. The resulting object files are **linked** together to produce a single executable.

The `account.cpp` file includes `account.h` so the compiler knows what the `Account` class looks like when compiling the method implementations. The `main.cpp` file also includes `account.h` so it knows what an `Account` is.

---

## 7. Separating Specification from Implementation

### 7.1 Why Separate Files?

Separating a class's specification from its implementation:
- Makes the class much easier to manage.
- Allows the interface to be shared without exposing the implementation.
- Enables faster compilation (changing a `.cpp` file doesn't require recompiling files that only include the `.h`).
- Follows the principle of information hiding at the file level.

### 7.2 The Header File

The header file (`.h` or `.hpp`) contains:
- The class **definition** (the member declarations that make up the class body; often loosely called the "class declaration")
- Any necessary `#include` directives (e.g., `#include <string>` if the class uses `std::string`)
- An **include guard** (see next section)

### 7.3 The Source File

The source file (`.cpp`) contains:
- `#include "classname.h"` at the top
- The method implementations, each prefixed with `ClassName::`

### 7.4 The Driver File

The driver file (e.g., `main.cpp`) contains:
- `#include "classname.h"`
- The `main` function and any code that uses the class

Remember: when the program is compiled, **both** `.cpp` files are compiled and then linked into a single executable.

---

## 8. Include Guards: Preventing Multiple Inclusion

### 8.1 The Problem

If the same header file is included more than once **in the same translation unit** — for
example, because `main.cpp` includes both `account.h` and another header that itself
includes `account.h` — the compiler will process the class definition more than once. For
a class definition this leads to a **redefinition** error, and for non-`inline` function or
variable definitions in the header it also leads to a redefinition error at compile time.
An include guard ensures the header's contents are processed only once per translation
unit.

Note that an include guard does **not** solve the separate problem of multiple definition
at **link time**. If a header contains a non-`inline` function or variable definition and
that header is included by several `.cpp` files, each translation unit gets its own copy
of the definition, and the linker will report a multiple definition error. The fix is to
move the definition into a single `.cpp` file (or mark it `inline`; C++17 inline variables
cover the variable case).

### 8.2 The Solution: Include Guards

An **include guard** is a series of preprocessor directives that guarantees the file will be included only once, no matter how many times it's included:

```cpp
#ifndef ACCOUNT_H
#define ACCOUNT_H

// ... entire class declaration ...

#endif
```

### 8.3 How It Works

1. `#ifndef ACCOUNT_H` checks whether the preprocessor symbol `ACCOUNT_H` has been defined.
2. If it **has** been defined, the preprocessor skips the entire file and jumps to `#endif`.
3. If it **has not** been defined, the symbol is defined (via `#define`) and the file is processed as usual.

### 8.4 Naming Conventions

The symbol name can be anything unique in the program, but a common convention is:
- Use the file name
- In uppercase
- Replace the dot with an underscore
- Optionally add a suffix such as `_INCLUDED`

For example: `ACCOUNT_H`, `ACCOUNT_H_INCLUDED`, `PLAYER_H`.

**Do not** use identifiers that begin with an underscore followed by an uppercase letter
(such as `_ACCOUNT_H`), or identifiers containing a double underscore (such as
`ACCOUNT__H`). Identifiers like these are reserved for the compiler and standard library,
and user code must not define them.

### 8.5 `#pragma once`

Many compilers also support the `#pragma once` directive:

```cpp
#pragma once

// ... class declaration ...
```

The effect is the same as an include guard. However, while many compilers support it, some don't — so check your compiler documentation to be sure. Include guards are more portable.

---

## 9. Putting It All Together

Let's walk through a complete example of implementing the `Account` class with methods defined both inside and outside the class declaration, and then separating it into files.

### 9.1 Step 1: Define the Class with Mixed Method Implementations

```cpp
// account.h
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
private:
    std::string name;
    double balance {0.0};

public:
    // Implemented inline (small, simple)
    void setName(const std::string& n) { name = n; }
    std::string getName() const { return name; }
    void setBalance(double bal) { balance = bal; }
    double getBalance() const { return balance; }

    // Declared only — implemented outside the class
    bool deposit(double amount);
    bool withdraw(double amount);
};

#endif
```

```cpp
// account.cpp
#include "account.h"

bool Account::deposit(double amount) {
    if (amount <= 0) return false;
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}
```

```cpp
// main.cpp
#include <iostream>
#include "account.h"

int main() {
    Account frankAccount;
    frankAccount.setName("Frank's Account");
    frankAccount.setBalance(1000.0);

    if (frankAccount.deposit(200.0)) {
        std::cout << "Deposit OK\n";        // balance is now 1200
    }

    if (frankAccount.withdraw(500.0)) {
        std::cout << "Withdrawal OK\n";     // balance is now 700
    }

    if (!frankAccount.withdraw(1500.0)) {
        std::cout << "Not sufficient funds\n";  // fails, balance stays 700
    }

    return 0;
}
```

### 9.2 What Happens at Compile Time

1. `main.cpp` is compiled. It includes `account.h`, so the compiler knows the `Account` class interface.
2. `account.cpp` is compiled. It includes `account.h`, so the compiler can type-check the method implementations against the declarations.
3. Both object files are linked together into a single executable.

### 9.3 The Debugging View

When running this in a debugger, you can watch the `frankAccount` object change:
- Initially: `name` is empty (std::string default), `balance` is 0.0 (from the in-class initializer).
- After `setName`: `name` becomes "Frank's Account".
- After `setBalance`: `balance` becomes 1000.
- After `deposit(200)`: `balance` becomes 1200.
- After `withdraw(500)`: `balance` becomes 700.
- After `withdraw(1500)`: `withdraw` returns false, `balance` stays 700.

Notice that because `name` and `balance` are private, you **must** use the public methods to change them. This is exactly the point of information hiding.

---

## 10. Summary

### 10.1 Key Concepts

| Concept | Meaning |
|---------|---------|
| **Class** | A blueprint for creating objects; a user-defined type |
| **Object** | An instance of a class with its own identity and values |
| **Attribute** | Data member of a class |
| **Method** | Function member of a class |
| **Encapsulation** | Bundling data and operations together in a class |
| **Information hiding** | Restricting access to implementation details via `private` |
| **Access modifier** | `public`, `private`, or `protected` — controls visibility |

### 10.2 Access Operators

| Situation | Operator | Example |
|-----------|----------|---------|
| Object | `.` | `frank.health` |
| Pointer to object | `->` | `enemy->health` |
| Pointer to object (explicit) | `(*p).` | `(*enemy).health` |

### 10.3 Method Implementation Options

| Option | Where | When to Use |
|--------|-------|-------------|
| Inline | Inside class declaration | Small, simple methods |
| Outside class | In same file, using `ClassName::` | Larger, more complex methods |
| Separate files | `.h` for declaration, `.cpp` for implementation | Larger programs (recommended) |

### 10.4 File Organization

| File | Contents |
|------|----------|
| `classname.h` | Class declaration, include guard, necessary includes |
| `classname.cpp` | Method implementations (each prefixed with `ClassName::`) |
| `main.cpp` | Driver code, includes `classname.h` |

### 10.5 Include Guards

```cpp
#ifndef CLASSNAME_H
#define CLASSNAME_H
// ... declaration ...
#endif
```

This ensures the header is processed only once per translation unit, even if it is included multiple times in the same `.cpp` file.

### 10.6 The Golden Rules

1. **Always** terminate a class declaration with a semicolon.
2. **Always** wrap header files in include guards.
3. **Never** include `.cpp` files — only include `.h` files.
4. **Always** use `ClassName::` when implementing methods outside the class declaration.
5. **Prefer** `private` attributes with `public` methods (getters/setters) to control access.
6. **Avoid** `using namespace std;` in header files — use `std::` explicitly.

---

## Next Steps

Now that you understand the fundamentals of classes, objects, member methods, and access control, you are ready to explore more advanced object-oriented topics in C++:

- **Constructors** — controlling how objects are initialized
- **Initializer lists** — efficient member initialization
- **Default, overloaded, copy, and move constructors** — different ways to construct objects
- **Shallow vs. deep copying** — what happens when classes contain pointers
- **Destructors** — cleaning up when objects are destroyed
- **The `this` pointer** — referring to the current object
- **Static class members** — attributes and methods that belong to the class itself, not to individual objects
- **Structs vs. classes** — the differences in C++
- **Friendship** — granting controlled access to private members

These topics build directly on what you have learned here. The fundamental pattern — declare a class, create objects, access members through a well-defined interface, and separate specification from implementation — will serve you throughout your C++ journey.