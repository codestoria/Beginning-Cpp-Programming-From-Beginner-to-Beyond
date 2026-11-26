// Section 12
// Return Pointer

#include <iostream>
#include <cstddef>

using namespace std;

int *create_array(size_t size, int init_value = 0) {
    int *new_storage {nullptr};
    new_storage = new int[size];
    for (size_t i {0}; i < size; ++i)
        *(new_storage + i) = init_value;
    return new_storage;
}

void display(const int *const array, size_t size) {
    for (size_t i {0}; i < size; ++i)
        cout << array[i] << " ";
    cout << endl;
}

int main() {
    int *my_array {nullptr};
    size_t size {0};
    int init_value {};

    cout << "\nHow many integers would you like to allocate? ";
    if (!(cin >> size) || size == 0) {
        cerr << "Invalid array size." << endl;
        return 1;
    }

    cout << "What value would you like them initialized to? ";
    if (!(cin >> init_value)) {
        cerr << "Invalid initial value." << endl;
        return 1;
    }

    my_array = create_array(size, init_value);
    cout << "\n--------------------------------------" << endl;

    display(my_array, size);
    delete[] my_array;
    my_array = nullptr;

    return 0;
}
