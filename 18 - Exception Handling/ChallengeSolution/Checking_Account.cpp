#include "Checking_Account.h"
#include <cmath>

Checking_Account::Checking_Account(std::string name, double balance) : Account{name, balance} {}

bool Checking_Account::withdraw(double amount) {
    if (!std::isfinite(amount) || amount <= 0)
        return false;
    amount += per_check_fee;
    return Account::withdraw(amount);
}

bool Checking_Account::deposit(double amount) { return Account::deposit(amount); }

void Checking_Account::print(std::ostream &os) const {
    const auto flags = os.flags();
    const auto precision = os.precision();
    os.precision(2);
    os << std::fixed;
    os << "[Checking_Account: " << name << ": " << balance << "]";
    os.flags(flags);
    os.precision(precision);
}
