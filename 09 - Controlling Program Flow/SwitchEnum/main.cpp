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
        case Direction::right:
            cout << "Going right" << endl;
            break;
        case Direction::up:
            cout << "Going up" << endl;
            break;
        case Direction::down:
            cout << "Going down" << endl;
            break;
        default:
            cout << "OK" << endl;
    }

    cout << endl;
    return 0;
}
