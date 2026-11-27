#ifndef ACCOUNT_UTIL_H
#define ACCOUNT_UTIL_H
#include <memory>
#include <vector>
#include "Account.h"
#include "Savings_Account.h"
#include "Checking_Account.h"
#include "Trust_Account.h"

// Utility functions for polymorphic Account pointers.

void display(const std::vector<std::unique_ptr<Account>> &accounts);
void deposit(std::vector<std::unique_ptr<Account>> &accounts, double amount);
void withdraw(std::vector<std::unique_ptr<Account>> &accounts, double amount);

// Utility functions for Savings Account objects.

void display(const std::vector<Savings_Account> &accounts);
void deposit(std::vector<Savings_Account> &accounts, double amount);
void withdraw(std::vector<Savings_Account> &accounts, double amount);

// Utility functions for Checking Account objects.
void display(const std::vector<Checking_Account> &accounts);
void deposit(std::vector<Checking_Account> &accounts, double amount);
void withdraw(std::vector<Checking_Account> &accounts, double amount);

// Utility functions for Trust Account objects.
void display(const std::vector<Trust_Account> &accounts);
void deposit(std::vector<Trust_Account> &accounts, double amount);
void withdraw(std::vector<Trust_Account> &accounts, double amount);

#endif