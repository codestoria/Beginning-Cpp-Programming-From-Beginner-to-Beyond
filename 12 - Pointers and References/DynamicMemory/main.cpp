// Section 12
// Dynamic Memory

#include <iostream>

using namespace std;

int main() {
    int *int_ptr {nullptr};
    int_ptr = new int;                       // allocate the int on the heap
    cout << "int_ptr: " << int_ptr << endl;  // print heap address
    delete int_ptr;                          // release it

    long long input_size {0};
    double *temp_ptr {nullptr};

    cout << "How many temps? ";
    if (!(cin >> input_size) || input_size < 0) {
        cerr << "Invalid size." << endl;
        return 1;
    }

    size_t size = static_cast<size_t>(input_size);
    temp_ptr = new double[size];               // allocate the storage on the heap
    cout << "temp_ptr: " << temp_ptr << endl;  // print heap address
    delete[] temp_ptr;                         // release it

    cout << endl;

    return 0;
}
