// Section 19
// Challenge 4 - Solution
// Copy Romeo and Juliet and add line numbers to non-empty lines

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

int main() {
    std::ifstream in_file {"romeoandjuliet.txt"};
    std::ofstream out_file {"romeoandjuliet_out.txt"};

    if (!in_file.is_open()) {
        std::cerr << "Error opening input file\n";
        return 1;
    }

    if (!out_file.is_open()) {
        std::cerr << "Error opening output file\n";
        return 1;
    }

    std::string line {};
    int line_number {0};
    while (std::getline(in_file, line)) {
        if (line.empty()) {
            out_file << '\n';
        } else {
            ++line_number;
            out_file << std::setw(7) << std::left << line_number << line << '\n';
        }
    }

    if (in_file.bad()) {
        std::cerr << "Error reading input file\n";
        return 1;
    }

    std::cout << "\nCopy complete\n";

    return 0;
}
