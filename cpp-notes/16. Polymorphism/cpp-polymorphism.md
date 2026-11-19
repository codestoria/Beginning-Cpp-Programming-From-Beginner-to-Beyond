# C++ Polymorphism

## Table of Contents

1. [What Is Polymorphism?](#1-what-is-polymorphism)
2. [Using Base Class Pointers](#2-using-base-class-pointers)
3. [Virtual Functions](#3-virtual-functions)
4. [Virtual Destructors](#4-virtual-destructors)
5. [The `override` Specifier (C++11)](#5-the-override-specifier-c11)
6. [The `final` Specifier (C++11)](#6-the-final-specifier-c11)
7. [Base Class References for Polymorphism](#7-base-class-references-for-polymorphism)
8. [Pure Virtual Functions and Abstract Classes](#8-pure-virtual-functions-and-abstract-classes)
9. [Abstract Classes as Interfaces](#9-abstract-classes-as-interfaces)
10. [Summary](#summary)

---

## 1. What Is Polymorphism?

**Polymorphism** literally means "many forms." In C++, it means that the same function call can behave differently depending on the actual type of the object it is invoked on. This lets you write code that works with a *family* of related types through a single common interface, instead of writing separate logic for each type.

### 1.1 Why Polymorphism Matters

Imagine you are building a banking system with many account types: savings, checking, trust, and more. Every account supports a `withdraw` method, but the rules are different for each type.

Without polymorphism, you would have to write separate logic for every account type. Your code would be full of `if/else` checks to figure out what kind of account you are dealing with. Every time you added a new account type, you would have to find and update every place in your code that works with accounts.

Polymorphism solves this. It lets you **program at a higher level of abstraction**: you just say "withdraw from this account," and the correct version of the function runs automatically, based on the actual type of the object.

### 1.2 Two Types of Polymorphism in C++

C++ supports two broad categories of polymorphism, based on *when* the function call is connected to its implementation.

| Type | Also called | When binding happens | Examples |
| --- | --- | --- | --- |
| Compile-time polymorphism | Static binding / early binding | Before the program runs | Function overloading, operator overloading |
| Runtime polymorphism | Dynamic binding / late binding | While the program executes | Function overriding with `virtual` + base pointers/references |

> **Binding** means associating a function call with the actual code that runs.

#### Compile-time polymorphism

You have already used compile-time polymorphism:

- When you overload functions (same name, different parameters)
- When you overload operators (e.g., `operator+` for your classes)

The compiler figures out which version to call before the program even starts. This is fast, and it is the default behavior in C++.

#### Runtime polymorphism

Runtime polymorphism defers the decision until the program is running. This is what lets a base class pointer "know" what type of object it is actually pointing to, and call the correct derived class method.

To enable runtime polymorphism, you need three things:

1. A public inheritance hierarchy
2. A base class pointer or base class reference
3. `virtual` functions in the base class

### 1.3 The Problem with Static Binding

Let's look at a simple example to see why the default behavior is not always what you want. For brevity, `#include` directives (such as `<iostream>`, `<vector>`, and `<ostream>`) are omitted from most examples in this document, but you will need them in real code.

```cpp
class Account {
public:
    void withdraw(double amount) {
        std::cout << "Account::withdraw\n";
    }
};

class Trust : public Account {
public:
    void withdraw(double amount) {
        std::cout << "Trust::withdraw\n";
    }
};
```

What happens when we use a base class pointer?

```cpp
Account* p = new Trust();
p->withdraw(1000); // Which version runs?
// Note: do NOT delete p here — Account has no virtual destructor yet,
// so deleting through a base pointer would be undefined behavior.
```

**By default, this calls `Account::withdraw`, not `Trust::withdraw`.**

Why? Because the compiler only sees that `p` is a pointer to `Account`. With static binding, it binds the call to the base class version at compile time — even though the object is actually a `Trust`.

This is almost never what you want. The fix is **virtual functions** (covered in Section 3), which switch the behavior to dynamic binding.

### 1.4 The Same Problem with Function Parameters

The same issue appears when you pass objects to functions that take a base class reference:

```cpp
void do_withdraw(Account& obj) {
    obj.withdraw(1000);
}

Trust t;
do_withdraw(t); // Calls Account::withdraw, not Trust::withdraw
```

Again, static binding uses only the type of the reference (`Account&`), not the actual object type.

> Without polymorphism, you would have to write a separate `do_withdraw` function for every account type. That defeats the whole purpose of inheritance.

---

## 2. Using Base Class Pointers

### 2.1 Why Base Class Pointers Are Useful

A base class pointer can point to *any* object of a class derived from that base (thanks to the "is-a" relationship of public inheritance).

This is the foundation of polymorphic code. It lets you:

- Treat many different derived types the same way
- Store mixed types in a single array or vector
- Write generic functions that work with the entire hierarchy
- Add new derived classes later without changing existing code

### 2.2 Basic Example: Individual Pointers

Once `withdraw` is declared `virtual` in the base class (Section 3 shows how), each pointer calls the correct method:

```cpp
// Assume Savings, Checking, and Trust all publicly inherit from Account
// and override withdraw(). We will see how to declare withdraw() as
// virtual in the next section.
Account* p1 = new Account();
Account* p2 = new Savings();
Account* p3 = new Checking();
Account* p4 = new Trust();

p1->withdraw(1000); // Account::withdraw
p2->withdraw(1000); // Savings::withdraw
p3->withdraw(1000); // Checking::withdraw
p4->withdraw(1000); // Trust::withdraw

// Don't forget to clean up.
// This will only be safe once Account declares a virtual destructor
// (see Section 4). Without it, deleting p2/p3/p4 would be undefined behavior.
// Note: cleanup is deferred to the end of Section 2.3 so the same pointers
// can be reused in the collection examples below.
```

> **Note:** If `withdraw` were *not* virtual, every one of these calls would execute `Account::withdraw`. The `virtual` keyword is what makes the pointer respect the actual object type.

### 2.3 The Real Power: Collections of Base Pointers

The biggest benefit appears when you put base class pointers into an array or vector. You can then loop over them and call the same method on every element — and the right version runs every time.

#### With an array:

```cpp
Account* accounts_arr[] = {p1, p2, p3, p4};

for (int i = 0; i < 4; ++i) {
    accounts_arr[i]->withdraw(1000);
}
```

#### With a vector:

```cpp
std::vector<Account*> accounts = {p1, p2, p3, p4};

for (const auto& acc : accounts) {
    acc->withdraw(1000);
}

// Cleanup — safe only if Account declares a virtual destructor.
// See Section 4 for why this matters.
delete p1; delete p2; delete p3; delete p4;
```

### 2.4 Easy Extensibility

If you later add a new account type (e.g., `BondAccount`), you do **not** need to change the loop code at all. As long as the new class inherits from `Account` and overrides `withdraw`, it will work automatically.

This is the **open/closed principle** in action: code is open for extension but closed for modification.

### 2.5 Important Note on Raw Pointers

The examples above use raw pointers for simplicity. In real code, you should prefer **smart pointers** (`std::unique_ptr`, `std::shared_ptr`) to avoid memory leaks and manual cleanup. Polymorphism works exactly the same way with smart pointers.

---

## 3. Virtual Functions

### 3.1 The `virtual` Keyword

To enable dynamic binding for a method, declare it as `virtual` in the base class. That is the single change you need.

### 3.2 Syntax

```cpp
class Account {
public:
    virtual void withdraw(double amount) {
        std::cout << "Account::withdraw\n";
    }
};
```

Once a function is declared `virtual` in the base class, it remains virtual **all the way down the inheritance hierarchy**. You do not have to write `virtual` again in derived classes. What matters more is marking the derived function with `override` (see Section 5), which lets the compiler verify that it really overrides a base class virtual function.

### 3.3 Overriding Requirements

For a derived class function to actually *override* the base class virtual function, it must be **compatible** with the base version:

- Same function name
- Same parameter list
- Same cv-qualification (`const` / `volatile`)
- Same ref-qualifier (`&` / `&&`), if any
- Same return type, or a covariant return type (a pointer or reference to a class derived from the return type of the base function)
- A compatible exception specification: the derived override must not have a *looser* exception specification than the base version (e.g. if the base function is `noexcept`, the derived override must also be `noexcept`)

If these do not match, the derived function does **not** override the base version. Depending on the kind of mismatch, it may either be a separate function that *hides* the base class function of the same name (calls through a base pointer or reference then still use the base version), or it may simply fail to compile (for example, an incompatible return type or exception specification). Both cases are common sources of bugs, which is why `override` (Section 5) is so valuable.

### 3.4 When Dynamic Binding Applies

Virtual functions are only dynamically bound when called via:

- A pointer to the base class
- A reference to the base class

If you call the method directly on an object (not through a pointer/reference), it uses static binding as usual.

```cpp
Trust t;
t.withdraw(1000);    // Static binding: Trust::withdraw

Account& ref = t;
ref.withdraw(1000);  // Dynamic binding: Trust::withdraw
```

### 3.5 Key Takeaway

Adding `virtual` to a base class method is all it takes to unlock runtime polymorphism. One keyword gives you enormous flexibility.

---

## 4. Virtual Destructors

### 4.1 Why Virtual Destructors Are Essential for Polymorphic Base Classes

If a class has any virtual function and objects may be deleted through a base class pointer, it **should** have a virtual destructor. This is one of the most important rules in C++.

Consider what happens when you `delete` a derived class object through a base class pointer:

```cpp
Account* p = new Trust();
delete p; // What happens here?
```

If the destructor is **not** virtual:

- The behavior is **undefined** according to the C++ standard
- In typical implementations, only the `Account` destructor is called and the `Trust` destructor never runs
- Any resources owned by the derived class may be leaked

This is a serious bug, and it will cause memory leaks, unclosed files, unfreed buffers, and crashes.

### 4.2 The Fix

Declare the base class destructor as `virtual`:

```cpp
class Account {
public:
    virtual void withdraw(double amount) { /* ... */ }
    virtual ~Account() { std::cout << "Account destructor\n"; }
};
```

When the destructor is virtual, `delete p` will:

1. Call the derived class destructor first
2. Then call the base class destructor

This is the correct order, and all resources are properly released.

### 4.3 Rule of Thumb

> If a class has at least one virtual function and objects may be deleted through a base class pointer, give it a virtual destructor. In practice, this means almost every polymorphic base class should have one.

### 4.4 Can Constructors Be Virtual?

No. Virtual constructors do not exist in C++, and they would not make sense — construction happens from the base up, and the object's type is not fully established until construction finishes.

---

## 5. The `override` Specifier (C++11)

### 5.1 The Problem It Solves

It is very easy to accidentally write a function that you *think* overrides a virtual function, but does not — because of a tiny difference in the signature.

For example:

```cpp
class Base {
public:
    virtual void say_hello() const {
        std::cout << "Hello from Base\n";
    }
};

class Derived : public Base {
public:
    // Oops! Forgot the const.
    void say_hello() {
        std::cout << "Hello from Derived\n";
    }
};
```

This compiles fine, but `say_hello` in `Derived` does **not** override the base class version. It is a separate function, and calls through a base pointer will still use `Base::say_hello`.

These bugs are notoriously hard to find.

### 5.2 How `override` Helps

Add the `override` specifier to the end of the function prototype in the derived class. This tells the compiler: *"I intend to override a base class virtual function. Check my work."*

```cpp
class Derived : public Base {
public:
    void say_hello() override { // Compiler error!
        std::cout << "Hello from Derived\n";
    }
};
```

If the signature does not match a base class virtual function, the compiler will immediately produce an error. You catch the bug at compile time instead of runtime.

### 5.3 Best Practice

> Always use `override` on every function you intend to override a virtual base class function.

---

## 6. The `final` Specifier (C++11)

### 6.1 Purpose

`final` is the opposite of extensibility. It lets you lock down a method or an entire class so it cannot be overridden or inherited from.

This is useful for:

- Performance optimization (the compiler can make stronger assumptions)
- Enforcing design decisions (this class is not meant to be a base class)
- Preventing further derivation in a hierarchy where it would be dangerous

### 6.2 Two Uses

#### 1. At the class level

Prevents the class from being used as a base class.

```cpp
class MyClass final {
    // ...
};

class Derived : public MyClass { }; // Compiler error
```

#### 2. At the method level

Prevents a virtual function from being overridden further down the hierarchy.

```cpp
class Base {
public:
    virtual void do_something();
};

class Derived : public Base {
public:
    void do_something() override final; // Cannot be overridden again
};

class MoreDerived : public Derived {
    void do_something() override; // Compiler error
};
```

---

## 7. Base Class References for Polymorphism

### 7.1 References Work Too

So far we have used pointers for polymorphism. Base class references also enable dynamic binding, and they are especially useful as function parameters.

### 7.2 Basic Example

```cpp
Trust t;
Account& ref = t;

ref.withdraw(1000); // Dynamic binding: Trust::withdraw
```

Just like pointers, the reference sees the actual object type at runtime and calls the correct method.

### 7.3 Using References in Functions

References are the natural choice when writing helper functions that should work with any class in the hierarchy:

```cpp
void do_withdraw(Account& acc, double amount) {
    acc.withdraw(amount);
}
```

You can pass *any* account type to this function:

```cpp
Account a;
Savings s;
Checking c;
Trust t;

do_withdraw(a, 1000); // Account::withdraw
do_withdraw(s, 1000); // Savings::withdraw
do_withdraw(c, 1000); // Checking::withdraw
do_withdraw(t, 1000); // Trust::withdraw
```

You write the function once, and it works for the entire hierarchy — and for any new classes you add later.

### 7.4 Pointers vs. References

|  | Pointers | References |
| --- | --- | --- |
| Supports dynamic polymorphism | Yes | Yes |
| Can be null / reassigned | Yes | No |
| Good for collections (arrays, vectors) | Yes | No |
| Good for function parameters | Yes | Often cleaner |

Use whichever fits your use case. Both give you the same polymorphic behavior.

---

## 8. Pure Virtual Functions and Abstract Classes

### 8.1 Why We Need Abstract Classes

Some base classes are so general that it does not make sense to create objects of them directly.

For example:

- A `Shape` class — you never draw "a shape"; you draw circles, lines, squares
- A `Player` class — there is no generic player, only wizards, warriors, etc.

(In an earlier section `Account` was used as a concrete, instantiable class. In a real banking design you might instead make `Account` abstract and derive `Savings`, `Checking`, and `Trust` from it — but the point here is simply that some base concepts are too general to instantiate.)

These classes exist only to be base classes. We want to:

1. Prevent them from being instantiated
2. Force all derived classes to implement certain methods

We do this with **pure virtual functions**.

### 8.2 Pure Virtual Function Syntax

A pure virtual function is declared with `= 0` at the end:

```cpp
class Shape {
public:
    virtual void draw() = 0;       // pure virtual
    virtual void rotate() = 0;     // pure virtual
    virtual ~Shape() = default;    // virtual destructor
};
```

### 8.3 Abstract Classes

A class with at least one pure virtual function is an **abstract class**. You cannot create an object of an abstract class:

```cpp
Shape s;               // Compiler error: cannot instantiate abstract class
Shape* p = new Shape(); // Compiler error
```

### 8.4 Rules for Derived Classes

When you derive from an abstract class:

- If you **override all** pure virtual functions → the derived class is **concrete** (you can instantiate it)
- If you **do not override all** pure virtual functions → the derived class is also **abstract**

Example of a concrete derived class:

```cpp
class Circle : public Shape {
public:
    void draw() override {
        std::cout << "Drawing a circle\n";
    }
    void rotate() override {
        std::cout << "Rotating a circle\n";
    }
};
```

### 8.5 Using Abstract Base Classes

Even though you cannot instantiate `Shape`, you can still use `Shape` pointers and references to work with concrete derived objects polymorphically:

```cpp
class Line : public Shape {
public:
    void draw() override { std::cout << "Drawing a line\n"; }
    void rotate() override { std::cout << "Rotating a line\n"; }
};

class Square : public Shape {
public:
    void draw() override { std::cout << "Drawing a square\n"; }
    void rotate() override { std::cout << "Rotating a square\n"; }
};

std::vector<Shape*> shapes = {
    new Circle(),
    new Line(),
    new Square()
};

for (const auto& shape : shapes) {
    shape->draw();
    shape->rotate();
}

for (auto* shape : shapes) {
    delete shape;
}
```

This is extremely powerful. Your rendering code only talks to the `Shape` interface. It does not need to know anything about circles, lines, or squares.

### 8.6 Note on Pure Virtual Function Bodies

Pure virtual functions usually have no implementation. It is technically possible to provide a body for one (which derived classes can call explicitly as `Base::func()`), but it is rarely needed. The point of a pure virtual function is to define an interface, not default behavior.

---

## 9. Abstract Classes as Interfaces

### 9.1 What Is an Interface Class?

An **interface class** is an abstract class that consists mainly of pure virtual functions. It defines a set of capabilities (a "contract") that any class can implement. Typically it also has a virtual destructor, and it may include small non-virtual helper functions, such as a friend `operator<<` that forwards to a pure virtual `print` method.

Unlike Java or C#, C++ does not have a special `interface` keyword. Instead, we use abstract classes with pure virtual functions to achieve the same effect.

Interface classes are typically named with a leading `I` by convention (e.g., `IPrintable`, `ISerializable`).

### 9.2 Why Interfaces Are Useful

Interfaces let you add a common capability to classes from completely different inheritance hierarchies.

For example:

- You want both `Account` objects and `Dog` objects to be printable with `std::cout`
- `Account` and `Dog` have nothing to do with each other
- But both can implement the `IPrintable` interface

### 9.3 Example: `IPrintable` Interface

```cpp
class IPrintable {
public:
    virtual void print(std::ostream& os) const = 0;
    virtual ~IPrintable() = default;

    friend std::ostream& operator<<(std::ostream& os, const IPrintable& obj) {
        obj.print(os);
        return os;
    }
};
```

Now any class that publicly inherits from `IPrintable` and implements `print` can be used with `std::cout`.

#### Implementing the interface

```cpp
class Account : public IPrintable {
public:
    void print(std::ostream& os) const override {
        os << "Account display";
    }
};

class Dog : public IPrintable {
public:
    void print(std::ostream& os) const override {
        os << "Woof woof";
    }
};
```

#### Using it

```cpp
Account a;
Dog d;

std::cout << a << '\n'; // Account display
std::cout << d << '\n'; // Woof woof
```

You can also write functions that accept `IPrintable` and work with *any* printable object:

```cpp
void print_object(const IPrintable& obj) {
    std::cout << obj << '\n';
}
```

### 9.4 Key Benefit

Interfaces let you decouple *what* a class can do from *what the class is*. This is one of the most flexible design tools in object-oriented programming.

---

## Summary

### Key Rules

1. **Static binding** is the C++ default — fast, but does not follow the actual object type.
2. **Runtime polymorphism** requires three things: public inheritance, base pointer/reference, and `virtual` functions.
3. A function declared `virtual` in the base class is virtual for the entire inheritance chain.
4. If a class has virtual functions **and may be deleted through a base class pointer**, it **should** have a virtual destructor. In practice, almost every polymorphic base class should have one.
5. For a function to override a base virtual function, its declaration must be compatible with the base version: same name, parameter list, and `const`/ref-qualifiers; a covariant return type is also allowed, and the exception specification must not be looser than the base's.
6. Always use `override` on overridden functions to catch signature mismatches at compile time.
7. Use `final` to prevent further overriding or inheritance.
8. A class with at least one pure virtual function (`= 0`) is abstract and cannot be instantiated.
9. An interface class is an abstract class consisting mainly of pure virtual functions, typically with a virtual destructor and possibly small non-virtual helper functions — it defines a contract.

### Best Practices

- Start with `virtual` in the base class, not in derived classes.
- Give polymorphic base classes a `virtual` destructor, especially when objects may be deleted through a base class pointer. (Derived classes do not need to repeat `virtual`; once the base destructor is virtual, derived destructors are implicitly virtual too.)
- Mark every overridden method with `override`.
- Use base class references for function parameters; use pointers for collections.
- Prefer smart pointers over raw pointers for polymorphic objects.
- Use abstract base classes when the base concept is too general to instantiate.
- Use interface classes to add shared capabilities across unrelated class hierarchies.
