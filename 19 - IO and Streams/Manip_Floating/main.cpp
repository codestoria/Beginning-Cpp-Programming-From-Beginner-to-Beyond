// Section 19
// Stream manipulators - Floating point manipulators
// scientific, setprecision, fixed, showpoint, showpos, uppercase

#include <iostream>
#include <iomanip>

int main() {
    double num1 {123456789.987654321};
    double num2 {1234.5678};
    double num3 {1234.0};

    // Using default floating-point formatting
    std::cout << "--Defaults ----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Precision 2: scientific notation is automatically selected for large values
    std::cout << std::setprecision(2);
    std::cout << "--Precision 2----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Using precision 5
    std::cout << std::setprecision(5);
    std::cout << "--Precision 5----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Using precision 9
    std::cout << std::setprecision(9);
    std::cout << "--Precision 9----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Precision 3 with fixed notation
    // Precision now specifies digits after the decimal point
    std::cout << std::setprecision(3) << std::fixed;
    std::cout << "--Precision 3 - fixed ----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Precision 3 with scientific notation
    // Precision specifies digits after the decimal point
    std::cout << std::setprecision(3) << std::scientific;
    std::cout << "--Precision 3 - scientific  ----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Scientific notation with uppercase 'E'
    std::cout << std::setprecision(3) << std::scientific << std::uppercase;
    std::cout << "--Precision 3 - scientific - uppercase  ----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Fixed notation with explicit plus sign for positive values
    std::cout << std::setprecision(3) << std::fixed << std::showpos;
    std::cout << "--Precision 3 - fixed - showpos ----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Clear fixed / scientific flags and disable showpos
    std::cout.unsetf(std::ios::scientific | std::ios::fixed);
    std::cout << std::resetiosflags(std::ios::showpos);

    // Show trailing decimal point and zeros up to precision 10
    std::cout << std::setprecision(10) << std::showpoint;
    std::cout << "-- precision 10 - showpoint ----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    // Restore all default floating-point formatting
    std::cout.unsetf(std::ios::scientific | std::ios::fixed);
    std::cout << std::setprecision(6);
    std::cout << std::resetiosflags(std::ios::showpos);
    std::cout << std::resetiosflags(std::ios::showpoint);

    std::cout << "--Back to defaults----------------------------" << std::endl;
    std::cout << num1 << std::endl;
    std::cout << num2 << std::endl;
    std::cout << num3 << std::endl;

    return 0;
}
