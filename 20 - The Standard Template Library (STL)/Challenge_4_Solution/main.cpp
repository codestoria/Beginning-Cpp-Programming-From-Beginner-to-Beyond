// Section 20
// Challenge 4 - Solution
// Determine if a string is a palindrome
// Use a queue and a stack to solve the problem

#include <cctype>
#include <stack>
#include <queue>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

bool is_palindrome(const std::string& s)
{
    std::stack<char> stk;
    std::queue<char> q;

    // Keep only alphabetic characters, convert to uppercase, push to both queue and stack
    for (char c : s)
        if (std::isalpha(c)) {
            c = std::toupper(c);
            q.push(c);
            stk.push(c);
        }

    char c1{};
    char c2{};

    // Compare queue front and stack top one by one
    while (!q.empty()) {
        c1 = q.front();
        q.pop();

        c2 = stk.top();
        stk.pop();

        if (c1 != c2)
            return false;
    }

    return true;
}

int main()
{
    std::vector<std::string> test_strings{ "a", "aa", "aba", "abba", "abbcbba", "ab", "abc", "radar", "bob", "ana",
        "avid diva", "Amore, Roma", "A Toyota's a toyota", "A Santa at NASA", "C++",
        "A man, a plan, a cat, a ham, a yak, a yam, a hat, a canal-Panama!", "This is a palindrome", "palindrome" };

    std::cout << std::boolalpha;
    std::cout << std::setw(8) << std::left << "Result" << "String" << std::endl;

    for (const auto& s : test_strings) {
        std::cout << std::setw(8) << std::left << is_palindrome(s) << s << std::endl;
    }

    std::cout << std::endl;

    return 0;
}
