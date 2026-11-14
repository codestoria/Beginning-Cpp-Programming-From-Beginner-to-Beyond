// Section 11
// Random Numbers

#include <iostream>
#include <cstdlib>  // rand(), srand(), RAND_MAX
#include <ctime>    // time()

using namespace std;

int main() {
    int random_number {0};
    size_t count {10};  // number of random numbers to generate
    int min {1};        // lower bound (inclusive)
    int max {6};        // upper bound (inclusive)

    // Seed the random number generator.
    // If you don't seed the generator, you will get the same
    // sequence of random numbers on every run.
    cout << "RAND_MAX on my system is: " << RAND_MAX << endl;
    srand(static_cast<unsigned int>(time(nullptr)));

    for (size_t i{1}; i <= count; ++i) {
        // Generate a random number in [min, max]
        random_number = rand() % (max - min + 1) + min;
        cout << random_number << endl;
    }

    cout << endl;
    return 0;
}
