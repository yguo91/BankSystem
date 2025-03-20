#include "Account.h"

Account::Account(const std::string& accNum, Customer* cust, double initialBalance)
    : accountNumber(accNum), balance(initialBalance), owner(cust), databaseId(0) {}
