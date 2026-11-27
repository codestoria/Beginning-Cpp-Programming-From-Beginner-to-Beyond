// Section 19
// Stream manipulators - Boolean
// boolalpha and noboolalpha

#include <iostream>
#include <iomanip>  // Required for stream manipulators

int main() {
    std::cout << "noboolalpha - default  (10 == 10) : " << (10 == 10) << std::endl;
    std::cout << "noboolalpha - default  (10 == 20) : " << (10 == 20) << std::endl;

    // Enable textual true/false output for booleans
    std::cout << std::boolalpha;
    std::cout << "boolalpha   (10 == 10) : " << (10 == 10) << std::endl;
    std::cout << "boolalpha   (10 == 20) : " << (10 == 20) << std::endl;

    // The boolalpha setting persists for subsequent boolean outputs
    std::cout << "boolalpha   (10 == 10) : " << (10 == 10) << std::endl;
    std::cout << "boolalpha   (10 == 20) : " << (10 == 20) << std::endl;

    // Toggle back to numeric 0/1 output
    std::cout << std::noboolalpha;
    std::cout << "noboolalpha  (10 == 10) : " << (10 == 10) << std::endl;
    std::cout << "noboolalpha  (10 == 20) : " << (10 == 20) << std::endl;

    // Re-enable textual true/false using setf()
    std::cout.setf(std::ios::boolalpha);
    std::cout << "boolalpha   (10 == 10) : " << (10 == 10) << std::endl;
    std::cout << "boolalpha   (10 == 20) : " << (10 == 20) << std::endl;

    // Clear the boolalpha flag to restore the default 0/1 output
    std::cout << std::resetiosflags(std::ios::boolalpha);
    std::cout << "Default  (10 == 10) : " << (10 == 10) << std::endl;
    std::cout << "Default  (10 == 20) : " << (10 == 20) << std::endl;

    return 0;
}
