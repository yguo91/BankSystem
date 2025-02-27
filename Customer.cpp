#include "Customer.h"
#include <iostream>

Customer::Customer(std::string id, std::string name, std::string email, std::string phone)
    : customerID(id), name(name), email(email), phone(phone) {}

std::string Customer::getCustomerID() const {
    return customerID;
}

std::string Customer::getName() const {
    return name;
}

void Customer::addAccount(Account* account) {
    accounts.push_back(account);
}

void Customer::removeAccount(const std::string& accountNumber) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if ((*it)->getAccountNumber() == accountNumber) {
            accounts.erase(it);
            return;
        }
    }
}

void Customer::listAccounts() const {
    for (const auto& account : accounts) {
        std::cout << "Account Number: " << account->getAccountNumber()
            << ", Balance: " << account->getBalance() << std::endl;
    }
}
