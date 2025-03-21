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
#include "DatabaseManager.h"  // Added for DB access
#include "DepositTransaction.h"

class Bank {
public:
    std::vector<Customer*> customers;
    std::vector<Account*> accounts;
    TransactionManager transactionManager;
    Logger* logger;
    ConfigManager* configManager;
    BankFacade bankFacade;
    DatabaseManager* databaseManager;  // Added database manager

    Bank();
    ~Bank();
    void addCustomer(Customer* customer);
    void cacheCustomer(Customer* customer);
    void removeCustomer(const std::string& customerID);
    Account* createAccount(Customer* targetCustomer, AccountType type, double initialBalance, Customer* initiator);
    //Account* createAccount(Customer* customer, AccountType type, double initialBalance);
    bool processTransaction(Transaction* transaction);
    void generateReport();
    void notifyCustomers(const std::string& message);
    // Delete an account belonging to a customer.
    bool deleteAccount(Customer* customer, const std::string& accountNumber);
    // Update account details (for example, balance or interest rate) for an account.
    bool updateAccountDetails(Customer* currentUser, const std::string& accountNumber, double newBalance, double newInterestRate);
    // Apply interest to a savings account.
    bool applyInterestToAccount(Customer* customer, const std::string& accountNumber);
    Customer* createNewUser(const std::string& username,
        const std::string& password,
        const std::string& fullName,
        const std::string& email,
        const std::string& phone);

    DatabaseManager* getDatabaseManager() const;  // Getter for database manager

    Customer* findCustomerById(const std::string& id);

    // Add this function declaration in the Bank class to handle the transactions
    //bool Bank::updateTransactionInDB(Transaction* transaction)
    bool updateTransactionInDB(DepositTransaction* transaction);
};

#endif // BANK_H