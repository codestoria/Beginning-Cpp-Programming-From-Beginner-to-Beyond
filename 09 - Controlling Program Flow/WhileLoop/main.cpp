// Section 9
// While Loop

#include <iostream>

using namespace std;

int main()
{
    // 1. Countdown
    int countdown_num {};
    cout << "Enter a positive integer - start the countdown: ";
    cin >> countdown_num;

    while (countdown_num > 0) {
        cout << countdown_num << endl;
        --countdown_num;
    }
    cout << "Blastoff!" << endl;

    // 2. Count up
    int count_up_num {};
    cout << "Enter a positive integer to count up to: ";
    cin >> count_up_num;

    int i {1};
    while (count_up_num >= i) {
        cout << i << endl;
        ++i;
    }

    // 3. Validate integer less than 100
    int limit_number {};
    cout << "Enter an integer less than 100: ";
    cin >> limit_number;

    while (limit_number >= 100) {
        cout << "Enter an integer less than 100: ";
        cin >> limit_number;
    }

    cout << "Thanks" << endl;

    // 4. Validate integer between 1 and 5
    bool done {false};
    int menu_number {0};

    while (!done) {
        cout << "Enter an integer between 1 and 5: ";
        cin >> menu_number;

        if (menu_number < 1 || menu_number > 5) {
            cout << "Out of range, try again" << endl;
        } else {
            cout << "Thanks!" << endl;
            done = true;
        }
    }

    cout << endl;
    return 0;
}
