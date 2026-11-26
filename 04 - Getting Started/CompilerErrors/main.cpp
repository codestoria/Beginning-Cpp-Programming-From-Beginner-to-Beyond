// Demonstrates a compiler error: dividing a string literal by an integer is ill-formed

#include <iostream>

int main() {
    std::cout << ("Hello world" / 125) << std::endl;

    return 0;
}
