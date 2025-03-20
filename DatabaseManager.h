#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

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
};

#endif // DATABASE_MANAGER_H