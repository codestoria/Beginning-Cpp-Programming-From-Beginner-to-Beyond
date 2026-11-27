#include "Savings_Account.h"
#include <cmath>
#include <stdexcept>

Savings_Account::Savings_Account(std::string name, double balance, double int_rate)
    : Account{name, balance}, int_rate{int_rate} {
    if (!std::isfinite(int_rate))
        throw std::invalid_argument{"Interest rate must be finite"};
}

// Reject invalid amounts before calculating and adding interest.
bool Savings_Account::deposit(double amount) {
    if (!std::isfinite(amount) || amount < 0.0)
        return false;
    amount += amount * (int_rate / 100.0);
    return Account::deposit(amount);
}

bool Savings_Account::withdraw(double amount) { return Account::withdraw(amount); }

void Savings_Account::print(std::ostream &os) const {
    const auto flags = os.flags();
    const auto precision = os.precision();
    os.precision(2);
    os << std::fixed;
    os << "[Savings_Account: " << name << ": " << balance << ", " << int_rate << "]";
    os.flags(flags);
    os.precision(precision);
}
