// Section 19
// Challenge 2 - Solution
// Automated Grader

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>

// Function prototypes
void print_header();
void print_footer(double average);
void print_student(const std::string &student, int score);
int process_response(const std::string &response, const std::string &answer_key);

void print_header() {
    std::cout << std::setw(25) << std::left << "Student"
              << std::setw(5) << std::right << "Score" << std::endl;
    std::cout << std::setw(28) << std::setfill('-') << "" << std::endl;
    std::cout << std::setfill(' ');
}

void print_footer(double average) {
    std::cout << std::setw(27) << std::setfill('-') << "" << '\n';
    std::cout << std::setfill(' ');
    std::cout << std::setw(25) << std::left << "Average score"
              << std::setw(5) << std::right << average << '\n';
}

void print_student(const std::string &student, int score) {
    std::cout << std::setw(25) << std::left << student
              << std::setw(5) << std::right << score << std::endl;
}

// Return the count of correct answers
int process_response(const std::string &response, const std::string &answer_key) {
    int score {0};
    const size_t limit = std::min(response.size(), answer_key.size());
    for (size_t i = 0; i < limit; ++i) {
        if (response.at(i) == answer_key.at(i)) {
            score++;
        }
    }
    return score;
}

int main() {
    std::ifstream in_file;
    std::string answer_key {};
    std::string name {};
    std::string response {};
    int running_sum {0};
    int total_students {0};
    double average_score {0.0};

    in_file.open("responses.txt");
    if (!in_file) {
        std::cerr << "Problem opening file" << std::endl;
        return 1;
    }

    // Assumes formatted input where student names and responses contain no spaces
    in_file >> answer_key;

    print_header();
    std::cout << std::setprecision(1) << std::fixed;

    while (in_file >> name >> response) {
        ++total_students;
        int score = process_response(response, answer_key);
        running_sum += score;
        print_student(name, score);
    }

    if (total_students != 0) {
        average_score = static_cast<double>(running_sum) / total_students;
    }

    print_footer(average_score);

    if (in_file.bad()) {
        std::cerr << "Error reading file\n";
        return 1;
    }

    return 0;
}
