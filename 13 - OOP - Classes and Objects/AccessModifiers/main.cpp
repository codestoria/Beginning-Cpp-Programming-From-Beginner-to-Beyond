// Section 13
// Access Modifiers

#include <iostream>
#include <string>

using namespace std;

class Player {
private:
    // Attributes.
    string name{"Player"};
    int health{0};
    int xp{0};

public:
    // Methods.
    void talk(string text_to_say) { cout << name << " says " << text_to_say << endl; }
    bool is_dead();
};

int main() {
    Player frank;
    //  frank.name = "Frank";
    //  cout << frank.health << endl;
    frank.talk("Hello there");

    return 0;
}
