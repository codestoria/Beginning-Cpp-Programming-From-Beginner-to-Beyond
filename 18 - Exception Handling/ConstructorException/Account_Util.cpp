#include <iostream>
#include <vector>
#include "Account_Util.h"

// Displays the Account objects pointed to in the vector.
void display(const std::vector<std::unique_ptr<Account>> &accounts) {
    std::cout << "\n=== Accounts===========================================" << std::endl;
    for (const auto &acc : accounts) {
        if (acc)
            std::cout << *acc << std::endl;
    }
}

// Attempts to deposit the supplied amount into each Account object.
void deposit(std::vector<std::unique_ptr<Account>> &accounts, double amount) {
    std::cout << "\n=== Depositing to Accounts =================================" << std::endl;
    for (const auto &acc : accounts) {
        if (!acc)
            continue;
        if (acc->deposit(amount))
            std::cout << "Deposited " << amount << " to " << *acc << std::endl;
        else
            std::cout << "Failed Deposit of " << amount << " to " << *acc << std::endl;
    }
}

// Attempts to withdraw the supplied amount from each Account object.
void withdraw(std::vector<std::unique_ptr<Account>> &accounts, double amount) {
    std::cout << "\n=== Withdrawing from Accounts ==============================" << std::endl;
    for (const auto &acc : accounts) {
        if (!acc)
            continue;
        if (acc->withdraw(amount))
            std::cout << "Withdrew " << amount << " from " << *acc << std::endl;
        else
            std::cout << "Failed Withdrawal of " << amount << " from " << *acc << std::endl;
    }
}
