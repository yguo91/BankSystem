#ifndef BANK_H
#define BANK_H

#include <vector>
#include "Customer.h"
#include "Account.h"
#include "TransactionManager.h"
#include "Logger.h"
#include "ConfigManager.h"
#include "BankFacade.h"
#include "AccountFactory.h"

class Bank {
public:
    std::vector<Customer*> customers;
    std::vector<Account*> accounts;
    TransactionManager transactionManager;
    Logger* logger;
    ConfigManager* configManager;
    BankFacade bankFacade;

    Bank();
    void addCustomer(Customer* customer);
    void removeCustomer(const std::string& customerID);
    Account* createAccount(Customer* customer, AccountType type, double initialBalance);
    bool processTransaction(Transaction* transaction);
    void generateReport();
    void notifyCustomers(const std::string& message);
};

#endif // BANK_H
