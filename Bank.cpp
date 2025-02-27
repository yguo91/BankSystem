#include "Bank.h"
#include <iostream>

void Bank::addCustomer(const Customer& newCustomer) {
    customers.push_back(newCustomer);
}

void Bank::removeCustomer(const std::string& customerID) {
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if (it->getCustomerID() == customerID) {
            customers.erase(it);
            return;
        }
    }
}

Account* Bank::createAccount(Customer& customer, Account* account) {
    customer.addAccount(account);
    accounts.push_back(account);
    return account;
}

void Bank::listCustomers() const {
    for (const auto& customer : customers) {
        std::cout << "Customer ID: " << customer.getCustomerID() << ", Name: " << customer.getName() << std::endl;
    }
}

void Bank::listAccounts() const {
    for (const auto& account : accounts) {
        std::cout << "Account Number: " << account->getAccountNumber()
            << ", Balance: " << account->getBalance() << std::endl;
    }
}
