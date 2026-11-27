#include "Account.h"
#include <cmath>

Account::Account(std::string name, double balance) : name{name}, balance{balance} {
    if (!std::isfinite(balance) || balance < 0.0)
        throw IllegalBalanceException{};
}

bool Account::deposit(double amount) {
    if (!std::isfinite(amount) || amount < 0 || !std::isfinite(balance + amount))
        return false;
    else {
        balance += amount;
        return true;
    }
}

bool Account::withdraw(double amount) {
    if (std::isfinite(amount) && amount > 0 && balance - amount >= 0) {
        balance -= amount;
        return true;
    } else
        return false;
}

void Account::print(std::ostream &os) const {
    const auto flags = os.flags();
    const auto precision = os.precision();
    os.precision(2);
    os << std::fixed;
    os << "[Account: " << name << ": " << balance << "]";
    os.flags(flags);
    os.precision(precision);
}
