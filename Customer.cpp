#include "Customer.h"
#include <algorithm>

Customer::Customer(const std::string& id, const std::string& n, const std::string& email, const std::string& phone, Role r)
    : customerID(id), name(n), email(email), phone(phone), role(r) {}

void Customer::addAccount(Account* account) {
    accounts.push_back(account);
}

void Customer::removeAccount(const std::string& accountNumber) {
    accounts.erase(std::remove_if(accounts.begin(), accounts.end(),
        [&](Account* a) { return a->accountNumber == accountNumber; }), accounts.end());
}

std::vector<Transaction*> Customer::getTransactionHistory() {
    std::vector<Transaction*> history;
    for (auto acc : accounts) {
        for (auto txn : acc->transactions)
            history.push_back(txn);
    }
    return history;
}

void Customer::receiveNotification(const std::string& message) {
    notifier.update(message);
}
