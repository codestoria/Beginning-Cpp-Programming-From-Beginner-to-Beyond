// Section 9
// Range-based For Loop

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

int main()
{
    int scores[] {10, 20, 30};

    for (auto score : scores) {
        cout << score << endl;
    }

    vector<double> temperatures {87.9, 77.9, 80.0, 72.5};
    double average_temp {};
    double total {};

    for (auto temp : temperatures) {
        total += temp;
    }

    if (!temperatures.empty()) {
        average_temp = total / temperatures.size();
    }

    cout << fixed << setprecision(1);
    cout << "Average temperature is " << average_temp << endl;

    for (auto val : {1, 2, 3, 4, 5}) {
        cout << val << endl;
    }

    string test_text {"This is a test"};

    for (auto c : test_text) {
        if (c != ' ') {
            cout << c;
        }
    }
    cout << endl;

    for (auto c : test_text) {
        if (c == ' ') {
            cout << "\t";
        } else {
            cout << c;
        }
    }
    cout << endl;

    cout << endl;
    return 0;
}
