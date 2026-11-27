// Section 13
// Move Constructor

#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class Move {
private:
    int *data;

public:
    void set_data_value(int d) {
        if (data) *data = d;
        else data = new int{d};
    }
    int get_data_value() const {
        if (!data) throw std::logic_error{"Move object has no value"};
        return *data;
    }

    // Constructor
    Move(int d);

    // Copy Constructor
    Move(const Move &source);

    // Move Constructor
    Move(Move &&source) noexcept;

    // Assignment is outside the scope of this constructor example.
    Move &operator=(const Move &) = delete;
    Move &operator=(Move &&) = delete;

    // Destructor
    ~Move();
};

Move::Move(int d) {
    data = new int;
    *data = d;
    cout << "Constructor for: " << d << endl;
}

// Copy constructor.
Move::Move(const Move &source) : data{source.data ? new int{*source.data} : nullptr} {
    cout << "Copy constructor - deep copy" << endl;
}

// Move constructor.
Move::Move(Move &&source) noexcept : data{source.data} {
    source.data = nullptr;
    if (data) cout << "Move constructor - moving resource: " << *data << endl;
    else cout << "Move constructor - moving an empty object" << endl;
}

Move::~Move() {
    if (data != nullptr) {
        cout << "Destructor freeing data for: " << *data << endl;
    } else {
        cout << "Destructor freeing data for nullptr" << endl;
    }
    delete data;
}

int main() {
    vector<Move> vec;

    vec.push_back(Move{10});
    vec.push_back(Move{20});
    vec.push_back(Move{30});
    vec.push_back(Move{40});
    vec.push_back(Move{50});
    vec.push_back(Move{60});
    vec.push_back(Move{70});
    vec.push_back(Move{80});

    return 0;
}
