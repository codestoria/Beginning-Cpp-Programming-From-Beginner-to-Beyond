// Section 9
// For Loop

#include <iostream>
#include <vector>

using namespace std;

int main()
{
    // 1. Print numbers 1 to 10
    for (int i {1}; i <= 10; ++i) {
        cout << i << endl;
    }
    cout << "---------------------" << endl;

    // 2. Print 1, 3, 5, 7, 9
    for (int i {1}; i <= 10; i += 2) {
        cout << i << endl;
    }
    cout << "---------------------" << endl;

    // 3. Countdown
    for (int i {10}; i > 0; --i) {
        cout << i << endl;
    }
    cout << "Blastoff!" << endl;
    cout << "---------------------" << endl;

    // 4. Print numbers from 10 to 100 that are divisible by 15
    for (int i {10}; i <= 100; i += 10) {
        if (i % 15 == 0) {
            cout << i << endl;
        }
    }
    cout << "---------------------" << endl;

    // 5. Two variables changing together
    for (int i {1}, j {5}; i <= 5; ++i, ++j) {
        cout << i << " + " << j << " = " << (i + j) << endl;
    }
    cout << "---------------------" << endl;

    // 6. Print 1 to 100, newline every 5 numbers
    for (int i {1}; i <= 100; ++i) {
        cout << i;
        if (i % 5 == 0) {
            cout << endl;
        } else {
            cout << " ";
        }
    }
    cout << "---------------------" << endl;

    // 7. Print 1 to 100, newline every 10 numbers
    for (int i {1}; i <= 100; ++i) {
        cout << i << ((i % 10 == 0) ? "\n" : " ");
    }
    cout << "---------------------" << endl;

    // 8. Iterate over a vector using indices
    vector<int> nums {10, 20, 30, 40, 50};
    for (size_t i {0}; i < nums.size(); ++i) {
        cout << nums[i] << endl;
    }

    cout << endl;
    return 0;
}
