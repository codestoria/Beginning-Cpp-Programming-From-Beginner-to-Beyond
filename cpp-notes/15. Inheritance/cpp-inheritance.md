# C++ Inheritance

## Table of Contents

1. [Why Inheritance Matters](#1-why-inheritance-matters)
2. [Core Terminology & Notation](#2-core-terminology--notation)
3. [Inheritance vs. Composition](#3-inheritance-vs-composition)
4. [Syntax for Deriving Classes](#4-syntax-for-deriving-classes)
5. [The `protected` Access Modifier](#5-the-protected-access-modifier)
6. [Constructors and Destructors](#6-constructors-and-destructors)
7. [Copy/Move Constructors and Assignment Operators](#7-copymove-constructors-and-assignment-operators)
8. [Redefining Base Class Methods](#8-redefining-base-class-methods)
9. [Multiple Inheritance](#9-multiple-inheritance)
10. [Full Worked Example: Account Hierarchy](#10-full-worked-example-account-hierarchy)
11. [Key Takeaways](#11-key-takeaways)

---

## 1. Why Inheritance Matters

When you build software, you will often notice that several classes share common traits. Consider these examples:

- Every bank account has a balance and supports deposits and withdrawals. However, a savings account adds interest rules, and a checking account adds fee structures.
- Every game character has health, a position, and the ability to move. However, a boss adds unique attack patterns.

If you write each class from scratch, you end up with **duplicated code**, harder maintenance, and inconsistent behavior. Inheritance solves this problem: it lets you create new classes *from existing classes*, reusing tested, proven logic while adding specialized features.

### What Is Inheritance?

Inheritance is an object-oriented mechanism that lets a new class **inherit data and behavior from an existing class**.

- You can extend the class with new attributes and methods.
- You can redefine inherited methods so they behave differently.
- You do **not** need to modify the original (base) class source code.

### Core Benefits

1. **Code reuse**: Reuse validated logic instead of rewriting it.
2. **Hierarchical structure**: Model real-world relationships cleanly.
3. **Maintainability**: Fix or update logic once in a base class, and all derived classes benefit.

> A valid inheritance must make logical sense. You would never derive `Planet` from `Employee` — they share no meaningful "is-a" relationship.

---

## 2. Core Terminology & Notation

Before writing code, we define the standard vocabulary used with inheritance.

### Basic Class Roles

| Term | Synonyms | Definition |
| --- | --- | --- |
| **Base class** | Parent class, superclass | The existing class being inherited from. |
| **Derived class** | Child class, subclass | The new class created from the base class; it inherits attributes and methods. |

### Inheritance Types

- **Single inheritance**: A derived class inherits from exactly one base class.
- **Multiple inheritance**: A derived class inherits from two or more unrelated base classes (an advanced topic, covered later).

### The "Is-A" Relationship

Public inheritance models an **is-a** relationship:

- A `Savings_Account` **is an** `Account`.
- A `Student` **is a** `Person`.
- A `Circle` **is a** `Shape`.

This relationship is transitive: if `Student` is-a `Person` and `GradStudent` is-a `Student`, then `GradStudent` is also a `Person`.

### Generalization vs. Specialization

- **Generalization**: Extracting shared features from multiple classes into a more abstract base class.
- **Specialization**: Creating a new derived class that adds unique features to a base class.

Programmers use both to build **class hierarchies** — tree-like structures where higher levels are general and lower levels are specific.

### UML Diagram Notation

In UML class diagrams:

- Draw a solid line from the derived class to the base class.
- Attach a closed, hollow triangle arrowhead that **points to the base class**.

---

## 3. Inheritance vs. Composition

Inheritance is not the only way to reuse code. **Composition** is another reuse mechanism that models a **has-a** relationship. (In strict UML, composition and aggregation are distinct — composition implies strong ownership, while aggregation implies a weaker association.)

### Key Difference

| Relationship | Mechanism | Example |
| --- | --- | --- |
| **Is-A** | Public inheritance | A `Student` is a `Person`. |
| **Has-A** | Composition | A `Person` has an `Account`. |

### When to Choose Which

- Use **public inheritance** only when a true is-a relationship exists.
- Prefer **composition** when the relationship is "has-a" or when you want to keep designs simpler.
- Inheritance introduces more complexity and tight coupling; composition is often more flexible.

> Rule of thumb: If you can model the relationship with composition, consider it first. Only use public inheritance when the is-a logic is clear and natural.

---

## 4. Syntax for Deriving Classes

### Basic Declaration

To create a derived class:

```cpp
class DerivedClassName : access_specifier BaseClassName {
    // derived class members
};
```

- `access_specifier` can be `public`, `protected`, or `private`.
- If you omit the specifier, `class` uses **private inheritance** by default; `struct` uses **public inheritance** by default.

### Three Inheritance Modes

1. **Public inheritance**
   - The most common form.
   - Models true is-a relationships.
   - This guide focuses primarily on public inheritance.
2. **Protected inheritance**
   - Not an is-a relationship.
   - Used for implementation reuse; base public members become protected in the derived class.
3. **Private inheritance**
   - Not an is-a relationship.
   - Used to implement one class "in terms of" another; all base public/protected members become private in the derived class.

### Minimal Example

```cpp
// Base class
class Account {
public:
    void deposit(double amount);
    void withdraw(double amount);
private:
    double balance;
};

// Derived class (public inheritance)
class Savings_Account : public Account {
public:
    // specialized deposit with interest bonus
    void deposit(double amount);
private:
    double interest_rate;
};
```

---

## 5. The `protected` Access Modifier

C++ provides three member access levels. You already know `public` and `private`; inheritance introduces the third: `protected`.

### What `protected` Means

A `protected` member:

1. ✅ Is accessible to **member functions of the class itself**.
2. ✅ Is accessible to **member functions of any derived class**.
3. ❌ Is **not** directly accessible from objects of the base or derived class (just like `private`).

> Think of `protected` as "private for objects, but accessible to inherited classes." It balances encapsulation with derived-class flexibility.

### Access Rules by Inheritance Type

The access level of inherited members changes based on the inheritance type.

#### Public Inheritance (most common)

| Base member access | Access in derived class |
| --- | --- |
| `public` | Remains `public` |
| `protected` | Remains `protected` |
| `private` | **Inherited but not directly accessible** |

#### Protected Inheritance

| Base member access | Access in derived class |
| --- | --- |
| `public` | Becomes `protected` |
| `protected` | Remains `protected` |
| `private` | Inherited but not directly accessible |

#### Private Inheritance

| Base member access | Access in derived class |
| --- | --- |
| `public` | Becomes `private` |
| `protected` | Becomes `private` |
| `private` | Inherited but not directly accessible |

### Critical Clarification: "Not Accessible" ≠ "Not Inherited"

A very common beginner misunderstanding:

> Private base members are still present in the derived object — they occupy memory and are part of the object. The derived class simply cannot access them by name.

You can still modify private base members indirectly through public or protected base class methods (getters/setters).

### Example: Access in Action

```cpp
class Base {
public:
    int a{0};
protected:
    int b{0};
private:
    int c{0};
};

class Derived : public Base {
public:
    void access_members() {
        a = 100;  // OK: public stays public
        b = 200;  // OK: protected stays protected
        // c = 300;  // COMPILER ERROR: c is private in Base
    }
};

int main() {
    Base base_obj;
    base_obj.a = 100;  // OK
    // base_obj.b = 200;  // ERROR: protected
    // base_obj.c = 300;  // ERROR: private

    Derived derived_obj;
    derived_obj.a = 100;  // OK
    // derived_obj.b = 200;  // ERROR: protected
    // derived_obj.c = 300;  // ERROR: inaccessible
}
```

> **Friend note**: A `friend` of the base class can access all base members (public, protected, private), but friendship is not inherited.

---

## 6. Constructors and Destructors

A derived object contains a "base part" plus its own part. C++ enforces strict initialization and cleanup order.

### Construction Order

When a derived object is created:

1. The **base class constructor** runs first (to initialize the base part).
2. Then the **derived class constructor** runs (to initialize the derived part).

### Destruction Order

When a derived object is destroyed:

1. The **derived class destructor** runs first.
2. Then the **base class destructor** runs.

> Destructors always run in the **reverse order** of constructors. This rule holds for inheritance hierarchies of any depth.

### What Is Not Inherited

Derived classes do **not** automatically inherit:

- Base class constructors
- Base class destructors
- Base class overloaded assignment operators
- Base class friend relationships

You can, however, explicitly invoke base class versions from the derived class.

### Default Constructor Behavior

If you do not specify which base constructor to call, the derived class automatically calls the **base class no-arg constructor**.

### Explicitly Calling a Base Constructor

To pass arguments to a base class constructor, use the **member initializer list**:

```cpp
class Base {
private:
    int value;
public:
    Base() : value{0} {}
    Base(int x) : value{x} {}
};

class Derived : public Base {
private:
    int doubled_value;
public:
    // Call base no-arg constructor
    Derived() : Base{}, doubled_value{0} {}

    // Call base int constructor, then init derived member
    Derived(int x) : Base{x}, doubled_value{x * 2} {}
};
```

List the base constructor first in the initializer list for readability. Note that initialization order is fixed: the base is always initialized before derived members, regardless of the order they appear in the initializer list.

### Inheriting Constructors with `using` (C++11)

You can explicitly inherit base class constructors:

```cpp
class Derived : public Base {
public:
    using Base::Base;  // inherit all non-special base constructors
};
```

**Caveats**:

- Only the base part is initialized from the inherited constructor's arguments; derived members are default-initialized or use their default member initializers.
- Default/copy/move constructors are not inherited.
- If you define a derived constructor with the same signature, it hides that inherited constructor; other inherited constructors remain available.

> Recommendation: Prefer writing your own derived constructors and explicitly calling the base versions. The code is clearer and less error-prone.

---

## 7. Copy/Move Constructors and Assignment Operators

### Compiler-Generated Versions

If you do not write copy/move constructors or copy/move assignment operators, the compiler will generate them automatically and correctly call the base class versions. For simple classes, this is sufficient.

### Custom Copy Constructor

If you need a custom copy constructor (e.g., for deep-copying dynamic memory), you **must explicitly invoke the base class copy constructor** in the initializer list.

```cpp
Derived::Derived(const Derived& other)
    : Base{other},       // copy the Base subobject of other
      doubled_value{other.doubled_value}
{
}
```

#### Object Slicing

Object slicing occurs when a derived object is copied into a base-class object by value, or assigned to a base-class object. In that case, only the base subobject is copied; the derived part is "sliced off." Passing by reference or pointer does **not** slice — the reference/pointer simply binds to the base subobject while the derived part remains intact.

### Custom Copy Assignment

For custom assignment operators, explicitly call the base class assignment:

```cpp
Derived& Derived::operator=(const Derived& rhs) {
    if (this == &rhs) return *this;       // check self-assignment

    Base::operator=(rhs);                 // assign base part
    doubled_value = rhs.doubled_value;    // assign derived part

    return *this;
}
```

> If you forget to call `Base::operator=`, the base part will not be assigned — a common and subtle bug.

### Move Semantics

Move constructors and move assignment operators follow the exact same pattern: explicitly call the base class move version from the derived class.

---

## 8. Redefining Base Class Methods

One of the most powerful uses of inheritance is **redefining** a base class method so it behaves differently in the derived class. Note: for non-virtual methods this is called *redefinition/hiding*; *overriding* is reserved for virtual functions.

### How to Redefine (Hide) a Method

Declare a method in the derived class with the **same name and same parameter list** as the base class method. When the base method is non-virtual, this is called **redefinition** or **hiding**. True overriding only applies to virtual functions.

```cpp
class Account {
public:
    void deposit(double amount) {
        balance += amount;
    }
protected:
    double balance{0};
};

class Savings_Account : public Account {
public:
    void deposit(double amount) {
        double bonus = amount * (interest_rate / 100.0);
        Account::deposit(amount + bonus);  // call base class version
    }
private:
    double interest_rate{5.0};
};
```

### Always Reuse Base Logic

Never duplicate the base class implementation. Instead, call `BaseClassName::method_name(...)` and add only the specialized logic. This way, if the base method is updated, the derived class automatically benefits.

### Static Binding (Default Behavior)

C++ uses **static binding** by default: the compiler decides which method to call based on the **declared type** of the variable/pointer at compile time.

```cpp
Account acc;
acc.deposit(100);        // calls Account::deposit

Savings_Account sav;
sav.deposit(100);        // calls Savings_Account::deposit

Account* ptr = &sav;
ptr->deposit(100);       // calls Account::deposit (static binding!)
```

Even though `ptr` points to a `Savings_Account`, the compiler sees only `Account*` and calls the base version. Dynamic (runtime) binding is achieved with `virtual` functions — the topic of polymorphism, covered in advanced courses.

---

## 9. Multiple Inheritance

Multiple inheritance lets a derived class inherit from **two or more base classes at once**.

### Syntax

```cpp
class Department_Chair : public Faculty, public Administrator {
    // ...
};
```

### Use Cases

- Useful for combining interfaces or mixin classes.
- Common in some library and framework code.

### Why It Is Generally Avoided

Multiple inheritance can introduce significant complexity:

- Name conflicts between base class members.
- The "diamond problem" (ambiguous shared base class subobjects).
- Harder-to-read and harder-to-maintain code.

> For most application code, you can refactor to use composition instead of multiple inheritance, resulting in a cleaner design.

---

## 10. Full Worked Example: Account Hierarchy

Below is a complete, minimal example demonstrating public inheritance, constructors, method redefinition, and static binding.

```cpp
#include <iostream>
#include <string>
#include <utility>

// ---------- Base Class ----------
class Account {
    friend std::ostream& operator<<(std::ostream& os, const Account& acc);
protected:
    std::string name;
    double balance;
public:
    Account() : name{"Account"}, balance{0.0} {}
    Account(std::string name, double balance)
        : name{std::move(name)}, balance{balance} {}

    bool deposit(double amount) {
        if (amount < 0) return false;
        balance += amount;
        return true;
    }

    bool withdraw(double amount) {
        if (amount < 0 || balance < amount) return false;
        balance -= amount;
        return true;
    }

    double get_balance() const { return balance; }
};

std::ostream& operator<<(std::ostream& os, const Account& acc) {
    os << "[Account] " << acc.name << ": $" << acc.balance;
    return os;
}

// ---------- Derived Class ----------
class Savings_Account : public Account {
    friend std::ostream& operator<<(std::ostream& os, const Savings_Account& acc);
private:
    double int_rate;
public:
    Savings_Account() : Account{}, int_rate{0.0} {}
    Savings_Account(std::string name, double balance, double rate)
        : Account{std::move(name), balance}, int_rate{rate} {}

    // Redefine deposit with interest bonus
    bool deposit(double amount) {
        double bonus = amount * (int_rate / 100.0);
        return Account::deposit(amount + bonus);
    }
};

std::ostream& operator<<(std::ostream& os, const Savings_Account& acc) {
    os << "[Savings] " << acc.name << ": $" << acc.balance
       << " | Rate: " << acc.int_rate << "%";
    return os;
}

// ---------- Demo ----------
int main() {
    Account acc{"General", 1000.0};
    std::cout << acc << "\n";
    acc.deposit(500.0);
    std::cout << "After deposit: " << acc << "\n";

    Savings_Account sav{"High Yield", 1000.0, 5.0};
    std::cout << sav << "\n";
    sav.deposit(1000.0);  // deposits 1000 + 50 bonus
    std::cout << "After deposit: " << sav << "\n";
}
```

---

## 11. Key Takeaways

1. **Public inheritance = is-a**. Use it only when the derived class truly is a subtype of the base.
2. **Prefer composition** for has-a relationships; it introduces less coupling.
3. **`protected`** members are accessible to derived classes but not to objects.
4. **Private base members are inherited but not directly accessible**. Use base class methods to work with them.
5. **Constructors run base-first, derived-second**. Destructors run in reverse order.
6. **Explicitly call base class versions** for constructors, copy/move operations, and redefined methods.
7. **Static binding** is the default: method calls are resolved at compile time based on the declared type.
8. **Multiple inheritance** is powerful but complex; avoid it when simpler designs work.

This foundation prepares you for the next major C++ topic: **polymorphism and virtual functions**, where inheritance truly shines.
