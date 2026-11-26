/**
 * @file main.cpp
 * @brief Section 4 Challenge: read a favorite number (1-100) and print the formatted response.
 */
#include <iostream>

int main() {
    int favorite_number{};  // Value-initialize to avoid uninitialized variable warnings

    // Prompt user and read the favorite number from standard input
    std::cout << "Enter your favorite number between 1 and 100: ";
    std::cin >> favorite_number;

    // Print both lines matching the challenge output format
    std::cout << "Amazing!! That's my favorite number too!" << std::endl;
    std::cout << "No really!!, " << favorite_number << " is my favorite number!" << std::endl;

    return 0;
}
