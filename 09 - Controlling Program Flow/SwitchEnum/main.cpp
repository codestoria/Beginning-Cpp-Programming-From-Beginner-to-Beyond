// Section 9
// Switch with enumeration

#include <iostream>

using namespace std;

int main()
{
    enum class Direction {
        left, right, up, down
    };

    Direction heading {Direction::left};

    switch (heading) {
        case Direction::left:
            cout << "Going left" << endl;
            break;
        case right:
            cout << "Going right" << endl;
            break;
        case up:
            cout << "Going up" << endl;
            break;
        case down:
            cout << "Going down" << endl;
            break;
        default:
            cout << "OK" << endl;
    }

    cout << endl;
    return 0;
}
