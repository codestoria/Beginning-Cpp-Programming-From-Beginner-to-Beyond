// Work-in-progress demo: reads input without using favorite_number and omits the second line

#include <iostream>

int main() {
    int favorite_number;

    std::cout << "Enter your favorite number between 1 and 100: ";
    std::cin >> favorite_number;
    std::cout << "Amazing! That's my favorite number too!" << std::endl;

    return 0;
}
