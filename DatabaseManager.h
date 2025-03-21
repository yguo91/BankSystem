#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include "Customer.h"
#include "Account.h"

//for storing transaction records
struct TransactionRecord {
    int transactionId;
    std::string transactionType;
    double amount;
    std::string transactionDate;
	int recipientAccount; // -1 if not applicable
};

class DatabaseManager {
private:
    sqlite3* db;
    std::string dbFile;

public:
    DatabaseManager();
    ~DatabaseManager();

    bool createDB();

    bool openDatabase();
    void closeDatabase();
    bool createTables();

    bool insertUser(const std::string& username, const std::string& password, const std::string& fullName, const std::string& email, const std::string& phone);
    bool insertAccount(int userId, const std::string& accountNumber, const std::string& accountType, double balance, double interestRate);
    bool insertTransaction(int accountId, const std::string& transactionType, double amount, int recipientAccount = -1);

    bool updateAccountBalance(int accountId, double newBalance);
    bool transferFunds(int senderAccount, int receiverAccount, double amount);
    int getLastInsertId();
    bool deleteAccount(int accountId);
    bool updateAccount(int accountId, double newBalance, double newInterestRate);

    bool validateUser(const std::string& userID, const std::string& password);
    
    Customer* getUser(const std::string& userID);

    sqlite3* getDB() const { return db; }

    std::vector<Account*> getAccountsForUser(const std::string& userID);

    // Retrieves an account from the database using the account number.
    Account* getAccountByAccountNumber(const std::string& accountNumber);

    std::vector<TransactionRecord> getTransactionsForAccount(int accountId);

    // Retrieves all accounts from the database.
    std::vector<Account*> getAllAccounts();
};

#endif // DATABASE_MANAGER_H