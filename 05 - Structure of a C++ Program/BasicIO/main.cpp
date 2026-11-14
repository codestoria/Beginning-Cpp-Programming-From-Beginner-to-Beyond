// Section 5
// Basic I/O using cin and cout

#include <iostream>

int main()
{
    // cout example
    std::cout << "Hello world!" << std::endl;
    std::cout << "Hello";
    std::cout << "World" << std::endl;

    std::cout << "Hello world!" << std::endl;
    std::cout << "Hello" << " world!" << std::endl;
    std::cout << "Hello" << " world!\n";
    std::cout << "Hello\nOut\nThere\n";

    // cin example
    int num1;
    int num2;
    double num3;

    std::cout << "Enter an integer: ";
    std::cin >> num1;
    std::cout << "You entered: " << num1 << std::endl;

    return 0;
}
