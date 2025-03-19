#include "Bank.h"
#include <algorithm>
#include <iostream>

Bank::Bank() {
    logger = Logger::getInstance();
    configManager = ConfigManager::getInstance();
}

void Bank::addCustomer(Customer* customer) {
    customers.push_back(customer);
}

void Bank::removeCustomer(const std::string& customerID) {
    customers.erase(std::remove_if(customers.begin(), customers.end(), [&](Customer* c) {
        return c->customerID == customerID;
        }), customers.end());
}

Account* Bank::createAccount(Customer* customer, AccountType type, double initialBalance) {
    Account* acc = AccountFactory::createAccount(type, customer, initialBalance);
    customer->addAccount(acc);
    accounts.push_back(acc);
    return acc;
}

bool Bank::processTransaction(Transaction* transaction) {
    return transactionManager.executeTransaction(transaction);
}

void Bank::generateReport() {
    std::cout << "Generating system report...\n";
    for (auto txn : transactionManager.transactionLog)
        std::cout << txn->transactionID << " : " << txn->amount << " on " << txn->timestamp;
}

void Bank::notifyCustomers(const std::string& message) {
    for (auto cust : customers)
        cust->receiveNotification(message);
}
