// Section 20
// Function templates - Generic function definitions for multiple data types

#include <iostream>
#include <string>

// Template function: returns the smaller of two values of the same type T
template <typename T>
T min(T a, T b) {
    return (a < b) ? a : b;
}

// Template function: accepts two potentially different types and prints them
template <typename T1, typename T2>
void func(T1 a, T2 b) {
    std::cout << a << " " << b << std::endl;
}

// Custom user-defined type Person
struct Person {
    std::string name;
    int age;

    // Overload less‑than operator: compare Person instances by age
    bool operator<(const Person &rhs) const {
        return this->age < rhs.age;
    }
};

// Overload stream insertion operator to support printing Person objects
std::ostream &operator<<(std::ostream &os, const Person &p) {
    os << p.name;
    return os;
}

// Generic swap function: swaps two variables of the same type T using references
template <typename T>
void my_swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

int main() {
    int x{100};
    int y{200};
    std::cout << x << ", " << y << std::endl;

    my_swap(x, y);
    std::cout << x << ", " << y << std::endl;

    Person p1{"Curly", 15};
    Person p2{"Moe", 30};

    Person p3 = min(p1, p2);
    std::cout << p3.name << " is younger" << std::endl;

    func(p1, p2);

    // Explicit template argument specification
    std::cout << min<int>(2, 3) << std::endl;  // 2

    // Implicit template argument deduction
    std::cout << min(2, 3) << std::endl;               // 2
    std::cout << min('A', 'B') << std::endl;           // A
    std::cout << min(12.5, 9.2) << std::endl;          // 9.2
    std::cout << min(5 + 2 * 2, 7 + 40) << std::endl;  // 9

    // Explicit template parameters
    func<int, int>(10, 20);

    // Type deduction from function arguments
    func(10, 20);
    func<char, double>('A', 12.4);
    func('A', 12.4);
    func(1000, "Testing");
    func(2000, std::string{"Frank"});

    return 0;
}
