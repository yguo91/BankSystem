#ifndef ACCOUNTFACTORY_H
#define ACCOUNTFACTORY_H

#include "Account.h"
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "BusinessAccount.h"
#include "Customer.h"
#include "DatabaseManager.h"

enum class AccountType {
    Savings,
    Checking,
    Business
};

class AccountFactory {
public:
    static void initializeCounter(DatabaseManager* dbManager);
    static Account* createAccount(AccountType type, Customer* owner, double initialBalance);
};

#endif // ACCOUNTFACTORY_H
