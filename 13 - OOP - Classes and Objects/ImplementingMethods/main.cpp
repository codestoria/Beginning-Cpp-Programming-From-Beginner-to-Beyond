// Section 13
// Implementing Member Methods 1

#include <iostream>
#include <string>

using namespace std;

class Account {
private:
    // Attributes.
    string name;
    double balance{0.0};

public:
    // Methods.
    // These methods are defined inline.
    void set_balance(double bal) { balance = bal; }
    double get_balance() { return balance; }

    // These methods are defined outside the class declaration.
    void set_name(string n);
    string get_name();

    bool deposit(double amount);
    bool withdraw(double amount);
};

void Account::set_name(string n) {
    name = n;
}

string Account::get_name() {
    return name;
}

bool Account::deposit(double amount) {
    if (amount < 0)
        return false;
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount >= 0 && balance >= amount) {
        balance -= amount;
        return true;
    } else {
        return false;
    }
}

int main() {
    Account frank_account;
    frank_account.set_name("Frank's account");
    frank_account.set_balance(1000.0);

    if (frank_account.deposit(200.0))
        cout << "Deposit OK" << endl;
    else
        cout << "Deposit Not allowed" << endl;

    if (frank_account.withdraw(500.0))
        cout << "Withdrawal OK" << endl;
    else
        cout << "Not sufficient funds" << endl;

    if (frank_account.withdraw(1500.0))
        cout << "Withdraw OK" << endl;
    else
        cout << "Not sufficient funds" << endl;

    return 0;
}
