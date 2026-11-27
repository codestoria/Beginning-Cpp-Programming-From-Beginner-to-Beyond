#include <iostream>
#include <memory>
#include "Account.h"
#include "Checking_Account.h"
#include "IllegalBalanceException.h"

using namespace std;

int main() {
    try {
        std::unique_ptr<Account> moes_account = std::make_unique<Checking_Account>("Moe", -10.0);
        std::cout << *moes_account << std::endl;
    } catch (const IllegalBalanceException &ex) {
        std::cerr << "Couldn't create account - invalid balance" << std::endl;
    }

    std::cout << "Program completed successfully" << std::endl;

    return 0;
}
