#ifndef SECTION13_IMPLEMENTINGMETHODS2_ACCOUNT_H
#define SECTION13_IMPLEMENTINGMETHODS2_ACCOUNT_H

#include <string>

class Account {
private:
    // Attributes.
    std::string name;
    double balance{0.0};

public:
    // Methods.
    // These methods are defined inline.
    void set_balance(double bal) { balance = bal; }
    double get_balance() { return balance; }

    // These methods are defined outside the class declaration.
    void set_name(std::string n);
    std::string get_name();

    bool deposit(double amount);
    bool withdraw(double amount);
};

#endif  // SECTION13_IMPLEMENTINGMETHODS2_ACCOUNT_H
