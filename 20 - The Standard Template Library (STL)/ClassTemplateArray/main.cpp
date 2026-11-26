// Section 20
// Class template - Array

// Since C++11, the standard library provides std::array as a template-based fixed-size array.
// Prefer std::array over raw C-style arrays whenever possible.

// Class templates can also take non-type template parameters, such as N here.

#include <iostream>
#include <string>

template <typename T, int N>
class Array {
    // Initialized from the non-type template parameter N; exposed via get_size().
    int size{N};

    // N is a compile-time constant, allowing a fixed-size member array of length N.
    T values[N];

    friend std::ostream &operator<<(std::ostream &os, const Array<T, N> &arr) {
        os << "[ ";
        for (const auto &val : arr.values) {
            os << val << " ";
        }
        os << "]" << std::endl;
        return os;
    }

public:
    Array() = default;

    Array(T init_val) {
        for (auto &item : values) {
            item = init_val;
        }
    }

    void fill(T val) {
        for (auto &item : values) {
            item = val;
        }
    }

    int get_size() const {
        return size;
    }

    // Subscript operator allowing indexed read/write access to elements.
    T &operator[](int index) {
        return values[index];
    }
};

int main() {
    Array<int, 5> nums;
    std::cout << "The size of nums is: " << nums.get_size() << std::endl;
    std::cout << nums << std::endl;

    nums.fill(0);
    std::cout << "The size of nums is: " << nums.get_size() << std::endl;
    std::cout << nums << std::endl;

    nums.fill(10);
    std::cout << nums << std::endl;

    nums[0] = 1000;
    nums[3] = 2000;
    std::cout << nums << std::endl;

    Array<int, 100> nums2{1};
    std::cout << "The size of nums2 is: " << nums2.get_size() << std::endl;
    std::cout << nums2 << std::endl;

    Array<std::string, 10> strings(std::string{"Frank"});
    std::cout << "The size of strings is: " << strings.get_size() << std::endl;
    std::cout << strings << std::endl;

    strings[0] = std::string{"Larry"};
    std::cout << strings << std::endl;

    strings.fill(std::string{"X"});
    std::cout << strings << std::endl;

    return 0;
}
