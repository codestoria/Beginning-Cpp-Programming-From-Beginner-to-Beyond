// Section 12
// Challenge - Solution

/* Write a C++ function named apply_all that expects two arrays of integers and their sizes and
   dynamically allocates a new array of integers whose size is the product of the 2 array sizes.

   The function should loop through the second array and multiply each element of array2
   by each element of array1, and store the product in the newly created array.

   The function should return a pointer to the newly allocated array.

   You can also write a print function that expects a pointer to an array of integers and its size
   and display the elements in the array.
*/

#include <iostream>
#include <cstddef>

using namespace std;

// Function prototypes
int *apply_all(const int *const arr1, size_t size1,
               const int *const arr2, size_t size2);
void print(const int *const arr, size_t size);

/*******************************************************************
This function expects:
    arr1  - a pointer to an array of integers
    size1 - the number of integers in arr1
    arr2  - a pointer to another array of integers
    size2 - the number of integers in arr2

    The function dynamically allocates a new array whose size is
    size1 * size2. Then it loops through each element of arr2 and
    multiplies it by each element of arr1. Each product is stored
    in the newly created array.
********************************************************************/
int *apply_all(const int *const arr1, size_t size1,
               const int *const arr2, size_t size2) {
    int *new_array = new int[size1 * size2];

    size_t position {0};
    for (size_t i {0}; i < size2; ++i) {
        for (size_t j {0}; j < size1; ++j) {
            new_array[position] = arr1[j] * arr2[i];
            ++position;
        }
    }

    return new_array;
}

/*******************************************************************
This function expects:
    arr  - a pointer to an array of integers
    size - the number of integers in arr

    The function loops through arr and displays all the integers
    in the array.
********************************************************************/
void print(const int *const arr, size_t size) {
    cout << "[ ";
    for (size_t i {0}; i < size; ++i)
        cout << arr[i] << " ";
    cout << "]";
    cout << endl;
}

int main() {
    constexpr size_t array1_size {5};
    constexpr size_t array2_size {3};

    int array1[] {1, 2, 3, 4, 5};
    int array2[] {10, 20, 30};

    cout << "Array 1: ";
    print(array1, array1_size);

    cout << "Array 2: ";
    print(array2, array2_size);

    int *results = apply_all(array1, array1_size, array2, array2_size);
    constexpr size_t results_size {array1_size * array2_size};

    cout << "Result: ";
    print(results, results_size);

    delete[] results;

    return 0;
}
