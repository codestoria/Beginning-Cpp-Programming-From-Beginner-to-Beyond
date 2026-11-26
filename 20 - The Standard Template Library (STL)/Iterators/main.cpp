// Section 20
// Iterators
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>

// Print all integers in a vector using a range-based for loop
void display(const std::vector<int> &vec) {
    std::cout << "[ ";
    for (auto const &i : vec) {
        std::cout << i << " ";
    }
    std::cout << "]" << std::endl;
}

void test1() {
    std::cout << "\n=============================" << std::endl;
    std::vector<int> nums1 {1, 2, 3, 4, 5};

    auto it = nums1.begin();          // Points to element 1
    std::cout << *it << std::endl;

    it++;                             // Points to element 2
    std::cout << *it << std::endl;

    it += 2;                          // Points to element 4
    std::cout << *it << std::endl;

    it -= 2;                          // Points to element 2
    std::cout << *it << std::endl;

    it = nums1.end() - 1;             // Points to element 5
    std::cout << *it << std::endl;
}

void test2() {
    std::cout << "\n=============================" << std::endl;
    // Print all vector elements using a forward iterator
    std::vector<int> nums1 {1, 2, 3, 4, 5};

    std::vector<int>::iterator it = nums1.begin();
    while (it != nums1.end()) {
        std::cout << *it << std::endl;
        it++;
    }

    // Overwrite every element in the vector with 0
    it = nums1.begin();
    while (it != nums1.end()) {
        *it = 0;
        it++;
    }

    display(nums1);
}

void test3() {
    std::cout << "\n=============================" << std::endl;
    // Use a const_iterator; the referenced elements cannot be modified
    std::vector<int> nums1 {1, 2, 3, 4, 5};

    std::vector<int>::const_iterator it1 = nums1.begin();
    // auto it1 = nums1.cbegin();

    while (it1 != nums1.end()) {
        std::cout << *it1 << std::endl;
        it1++;
    }

    // Attempting to assign through a const_iterator will produce a compile error
    it1 = nums1.begin();
    while (it1 != nums1.end()) {
        // *it1 = 0;  // Compile error - read-only access
        it1++;
    }
}

void test4() {
    // Reverse iterator: traverses the vector from last element toward first
    std::vector<int> vec {1, 2, 3, 4};
    auto it1 = vec.rbegin();  // Reverse iterator starting at element 4
    while (it1 != vec.rend()) {
        std::cout << *it1 << std::endl;
        it1++;
    }

    // Constant reverse iterator over a list of strings
    std::list<std::string> names {"Larry", "Moe", "Curly"};
    auto it2 = names.crbegin();  // Const reverse iterator pointing to "Curly"
    std::cout << *it2 << std::endl;
    it2++;  // Points to "Moe"
    std::cout << *it2 << std::endl;

    // Iterator over a map, visiting key-value pairs
    std::map<std::string, std::string> favorites {
        {"Frank", "C++"},
        {"Bill", "Java"},
        {"James", "Haskell"}
    };
    auto it3 = favorites.begin();  // Iterator over map<string, string> pairs
    while (it3 != favorites.end()) {
        std::cout << it3->first << " : " << it3->second << std::endl;
        it3++;
    }
}

void test5() {
    // Iterate over a contiguous subrange of vector elements
    std::vector<int> vec {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto start = vec.begin() + 2;
    auto finish = vec.end() - 3;

    while (start != finish) {
        std::cout << *start << std::endl;
        start++;
    }
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();

    return 0;
}
