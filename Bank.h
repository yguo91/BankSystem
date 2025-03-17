#ifndef BANK_H
#define BANK_H
#define DATABASE_FILE "Data/BANK.db"

#include <vector>
#include <string>
#include "Customer.h"
#include "Account.h"

class Bank {
private:
    std::vector<Customer> customers;
    std::vector<Account*> accounts;

public:
    Bank() = default;

    void addCustomer(const Customer& newCustomer);
    void removeCustomer(const std::string& customerID);
    Account* createAccount(Customer& customer, Account* account);
    void listCustomers() const;
    void listAccounts() const;
};

#endif // BANK_H

