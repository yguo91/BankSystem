#include "AccountFactory.h"
#include <sstream>

Account* AccountFactory::createAccount(AccountType type, Customer* owner, double initialBalance) {
    static int accCounter = 1000000;
    std::stringstream ss;
    ss << "ACC-" << accCounter++;
    std::string accNum = ss.str();

    switch (type) {
    case AccountType::Savings:
        return new SavingsAccount(accNum, owner, initialBalance);
    case AccountType::Checking:
        return new CheckingAccount(accNum, owner, initialBalance);
    case AccountType::Business:
        return new BusinessAccount(accNum, owner, initialBalance);
    default:
        return nullptr;
    }
}
