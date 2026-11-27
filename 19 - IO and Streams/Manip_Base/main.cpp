// Section 19
// Stream manipulators - Integer manipulators
// dec, hex, oct, showbase, uppercase, nouppercase, showpos

#include <iostream>
#include <iomanip>

int main() {
    int num {255};

    // Display values using different numeric bases
    std::cout << "\n------------------------------------------------------------" << std::endl;
    std::cout << std::dec << num << std::endl;
    std::cout << std::hex << num << std::endl;
    std::cout << std::oct << num << std::endl;

    // Display values with base prefixes for hexadecimal and octal
    std::cout << "\n------------------------------------------------------------" << std::endl;
    std::cout << std::showbase;
    std::cout << std::dec << num << std::endl;
    std::cout << std::hex << num << std::endl;
    std::cout << std::oct << num << std::endl;

    // Display hexadecimal digits in uppercase
    std::cout << "\n------------------------------------------------------------" << std::endl;
    std::cout << std::showbase << std::uppercase;
    std::cout << std::hex << num << std::endl;

    // Show the plus sign for positive decimal values
    std::cout << "\n------------------------------------------------------------" << std::endl;
    std::cout << std::showpos;
    std::cout << std::dec << num << std::endl;
    std::cout << std::hex << num << std::endl;
    std::cout << std::oct << num << std::endl;

    // Set format flags using the setf() method
    std::cout.setf(std::ios::showbase);
    std::cout.setf(std::ios::uppercase);
    std::cout.setf(std::ios::showpos);
    std::cout << "\n--- setf flags active ---\n"
              << std::dec << num << " | "
              << std::hex << num << " | "
              << std::oct << num << std::endl;

    // Reset format flags back to default state
    std::cout << std::resetiosflags(std::ios::basefield);
    std::cout << std::resetiosflags(std::ios::showbase);
    std::cout << std::resetiosflags(std::ios::showpos);
    std::cout << std::resetiosflags(std::ios::uppercase);

    std::cout << "\n------------------------------------------------------------" << std::endl;
    std::cout << "Enter an integer : ";
    std::cin >> num;

    std::cout << "Decimal default  : " << num << std::endl;

    std::cout << "Hexadecimal      : " << std::hex << num << std::endl;
    std::cout << "Hexadecimal      : " << std::hex << std::uppercase << num << std::endl;
    std::cout << "Hexadecimal      : " << std::hex << num << std::endl;
    std::cout << "Hexadecimal      : " << std::hex << std::nouppercase << num << std::endl;

    std::cout << "Octal            : " << std::oct << num << std::endl;
    std::cout << "Hexadecimal      : " << std::hex << std::showbase << num << std::endl;
    std::cout << "Octal            : " << std::oct << std::noshowbase << num << std::endl;

    std::cout << std::endl << std::endl;
    return 0;
}
