#include "DatabaseManager.h"
#include <filesystem>
#include <stdio.h>
#include <sys/stat.h>
#include "Customer.h"
#include <algorithm>  // needed for std::transform
#include <cctype>
#include "SavingsAccount.h"
#include "CheckingAccount.h"
#include "BusinessAccount.h"

#define DATABASE_FILE "Data/BANK.db"

DatabaseManager::DatabaseManager() : db(nullptr), dbFile(DATABASE_FILE) {
    createDB();
    openDatabase();
    createTables();
}

DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

bool fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

bool DatabaseManager::createDB() {
    if (fileExists(DATABASE_FILE)) {
        return true;
    }

    sqlite3* tempDB;
    int exit = sqlite3_open(DATABASE_FILE, &tempDB);
    if (exit != SQLITE_OK) {
        std::cerr << "Error creating database: " << sqlite3_errmsg(tempDB) << std::endl;
        return false;
    }
    sqlite3_close(tempDB);
    return true;
}

bool DatabaseManager::openDatabase() {
    int exit = sqlite3_open(DATABASE_FILE, &db);
    if (exit != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
    }
}

bool DatabaseManager::createTables() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL,"
        "full_name TEXT NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "phone TEXT,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ");"

        "CREATE TABLE IF NOT EXISTS accounts ("
        "account_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "account_number TEXT UNIQUE NOT NULL,"
        "account_type TEXT NOT NULL,"
        "balance REAL DEFAULT 0.00,"
        "interest_rate REAL DEFAULT 0.00,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE"
        ");"

        "CREATE TABLE IF NOT EXISTS transactions ("
        "transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "account_id INTEGER NOT NULL,"
        "transaction_type TEXT NOT NULL,"
        "amount REAL NOT NULL,"
        "transaction_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
        "recipient_account INTEGER,"
        "FOREIGN KEY (account_id) REFERENCES accounts(account_id) ON DELETE CASCADE,"
        "FOREIGN KEY (recipient_account) REFERENCES accounts(account_id) ON DELETE SET NULL"
        ");";

            char* errMsg;
        int exit = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
        if (exit != SQLITE_OK) {
            std::cerr << "Error creating tables: " << errMsg << std::endl;
            sqlite3_free(errMsg);
            return false;
        }
        return true;
}

bool DatabaseManager::insertUser(const std::string& username, const std::string& password, const std::string& fullName, const std::string& email, const std::string& phone) {
    std::cout << "Inserting user: " << username << std::endl;
    std::string sql = "INSERT INTO users (username, password_hash, full_name, email, phone) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, fullName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, phone.c_str(), -1, SQLITE_STATIC);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error inserting user: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return exit == SQLITE_DONE;
}

bool DatabaseManager::insertAccount(int userId, const std::string& accountNumber, const std::string& accountType, double balance, double interestRate) {
    std::cout << "Inserting account for user ID: " << userId << std::endl;
    std::string sql = "INSERT INTO accounts (user_id, account_number, account_type, balance, interest_rate) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_text(stmt, 2, accountNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, accountType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, balance);
    sqlite3_bind_double(stmt, 5, interestRate);

    exit = sqlite3_step(stmt);
    if (exit != SQLITE_DONE) {
        std::cerr << "Error inserting account: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return exit == SQLITE_DONE;
}

bool DatabaseManager::insertTransaction(int accountId, const std::string& transactionType, double amount, int recipientAccount) {
    std::string sql = "INSERT INTO transactions (account_id, transaction_type, amount, recipient_account) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, accountId);
    sqlite3_bind_text(stmt, 2, transactionType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, amount);
    if (recipientAccount != -1) {
        sqlite3_bind_int(stmt, 4, recipientAccount);
    }
    else {
        sqlite3_bind_null(stmt, 4);
    }

    int exit = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return exit == SQLITE_DONE;
}

bool DatabaseManager::updateAccountBalance(int accountId, double newBalance) {
    std::string sql = "UPDATE accounts SET balance = ? WHERE account_id = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_double(stmt, 1, newBalance);
    sqlite3_bind_int(stmt, 2, accountId);

    int exit = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return exit == SQLITE_DONE;
}

bool DatabaseManager::transferFunds(int senderAccount, int receiverAccount, double amount) {
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    if (!updateAccountBalance(senderAccount, -amount) || !updateAccountBalance(receiverAccount, amount)) {
        sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        return false;
    }
    insertTransaction(senderAccount, "transfer", -amount, receiverAccount);
    insertTransaction(receiverAccount, "transfer", amount, senderAccount);

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return true;
}

int DatabaseManager::getLastInsertId() {
    return static_cast<int>(sqlite3_last_insert_rowid(db));
}

bool DatabaseManager::deleteAccount(int accountId) {
    std::string sql = "DELETE FROM accounts WHERE account_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_int(stmt, 1, accountId);
    int exit = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return exit == SQLITE_DONE;
}

bool DatabaseManager::updateAccount(int accountId, double newBalance, double newInterestRate) {
    std::string sql = "UPDATE accounts SET balance = ?, interest_rate = ? WHERE account_id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    sqlite3_bind_double(stmt, 1, newBalance);
    sqlite3_bind_double(stmt, 2, newInterestRate);
    sqlite3_bind_int(stmt, 3, accountId);
    int exit = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return exit == SQLITE_DONE;
}

bool DatabaseManager::validateUser(const std::string& userID, const std::string& password) {
    // Convert the userID (string) to an integer
    int uid;
    try {
        uid = std::stoi(userID);
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid user ID format." << std::endl;
        return false;
    }

    std::string sql = "SELECT password_hash FROM users WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, uid);
    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) {
        // Retrieve the stored plain-text password (from the password_hash column)
        const unsigned char* dbPassword = sqlite3_column_text(stmt, 0);
        bool valid = (password == reinterpret_cast<const char*>(dbPassword));
        sqlite3_finalize(stmt);
        return valid;
    }
    else {
        sqlite3_finalize(stmt);
        return false;  // User not found or query error
    }
}

Customer* DatabaseManager::getUser(const std::string& userID)
{
    int uid;
    try {
        uid = std::stoi(userID);
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid user ID format." << std::endl;
        return nullptr;
    }
    
    std::string sql = "SELECT user_id, full_name, email, phone FROM users WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement in getUser: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    
    sqlite3_bind_int(stmt, 1, uid);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Retrieve columns: user_id, full_name, email, phone
        int dbUserID = sqlite3_column_int(stmt, 0);
        std::string fullName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        sqlite3_finalize(stmt);
        // Create a Customer using the user_id (converted back to string)
        return new Customer(std::to_string(dbUserID), fullName, email, phone, Role::Personal);
    }
    else {
        sqlite3_finalize(stmt);
        return nullptr;
    }
}

std::vector<Account*> DatabaseManager::getAccountsForUser(const std::string& userID)
{
    std::vector<Account*> accounts;
    int uid;
    try {
        uid = std::stoi(userID);
    }
    catch (const std::exception& e) {
        std::cerr << "Invalid user ID format in getAccountsForUser." << std::endl;
        return accounts;
    }

    std::string sql = "SELECT account_id, account_number, account_type, balance, interest_rate FROM accounts WHERE user_id = ?;";
    sqlite3_stmt* stmt;
    int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        std::cerr << "Error preparing statement in getAccountsForUser: " << sqlite3_errmsg(db) << std::endl;
        return accounts;
    }

    sqlite3_bind_int(stmt, 1, uid);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int accountId = sqlite3_column_int(stmt, 0);
        std::string accountNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string accountType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double balance = sqlite3_column_double(stmt, 3);
        double interestRate = sqlite3_column_double(stmt, 4);

        // Convert accountType to lowercase for comparison
        std::string typeLower = accountType;
        std::transform(typeLower.begin(), typeLower.end(), typeLower.begin(), ::tolower);

        Account* account = nullptr;
        if (typeLower == "savings") {
            // SavingsAccount: constructor (accountNumber, owner, initialBalance, interestRate)
            account = new SavingsAccount(accountNumber, nullptr, balance, interestRate);
        }
        else if (typeLower == "chequing") {
            // CheckingAccount: constructor (accountNumber, owner, initialBalance)
            account = new CheckingAccount(accountNumber, nullptr, balance);
        }
        else if (typeLower == "business") {
            // BusinessAccount: constructor (accountNumber, owner, initialBalance, limit, fee)
            account = new BusinessAccount(accountNumber, nullptr, balance);
        }
        // Add other account types if needed.

        if (account) {
            account->databaseId = accountId;
            accounts.push_back(account);
        }
    }

    sqlite3_finalize(stmt);
    return accounts;
}

Account* DatabaseManager::getAccountByAccountNumber(const std::string& accountNumber)
{
    std::string sql = "SELECT account_id, user_id, account_number, account_type, balance, interest_rate FROM accounts WHERE account_number = ?;";
    sqlite3_stmt* stmt;
    int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exit != SQLITE_OK) {
        std::cerr << "Error preparing statement in getAccountByAccountNumber: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    Account* account = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int accountId = sqlite3_column_int(stmt, 0);
        std::string accNum = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string accountType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double balance = sqlite3_column_double(stmt, 4);
        double interestRate = sqlite3_column_double(stmt, 5);

        // Depending on accountType, create the appropriate account object.
        // Here we handle Savings, Checking, and Business accounts. Extend as needed.
        if (accountType == "savings") {
            account = new SavingsAccount(accNum, nullptr, balance, interestRate);
        }
        else if (accountType == "chequing") {
            // Assuming CheckingAccount constructor signature: (accountNumber, owner, balance)
            account = new CheckingAccount(accNum, nullptr, balance);
        }
        else if (accountType == "business") {
            // Assuming BusinessAccount constructor signature: (accountNumber, owner, balance)
            account = new BusinessAccount(accNum, nullptr, balance);
        }
        // Set the databaseId so that future updates work properly.
        if (account) {
            account->databaseId = accountId;
        }
    }
    sqlite3_finalize(stmt);
    return account;
}

std::vector<TransactionRecord> DatabaseManager::getTransactionsForAccount(int accountId)
{
    std::vector<TransactionRecord> records;
	// SQL query to retrieve transaction records for a given account ID
    std::string sql = "SELECT transaction_id, transaction_type, amount, transaction_date, recipient_account FROM transactions WHERE account_id = ?;";
    sqlite3_stmt* stmt;

	// prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement in getTransactionsForAccount: " << sqlite3_errmsg(db) << std::endl;
        return records;
    }

    sqlite3_bind_int(stmt, 1, accountId);

	// go through the result set
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        TransactionRecord record;
        record.transactionId = sqlite3_column_int(stmt, 0);
        record.transactionType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        record.amount = sqlite3_column_double(stmt, 2);
        record.transactionDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		// recipient_account may be NULL, so check the type before reading
        if (sqlite3_column_type(stmt, 4) == SQLITE_NULL)
            record.recipientAccount = -1;
        else
            record.recipientAccount = sqlite3_column_int(stmt, 4);

        records.push_back(record);
    }

	//release the statement
    sqlite3_finalize(stmt);
    return records;
}

std::vector<Account*> DatabaseManager::getAllAccounts()
{
    std::vector<Account*> accounts;
    std::string sql = "SELECT account_id, account_number, account_type, balance, interest_rate FROM accounts;";
    sqlite3_stmt* stmt;
    int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        std::cerr << "Error preparing statement in getAllAccounts: " << sqlite3_errmsg(db) << std::endl;
        return accounts;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int accountId = sqlite3_column_int(stmt, 0);
        std::string accountNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string accountType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double balance = sqlite3_column_double(stmt, 3);
        double interestRate = sqlite3_column_double(stmt, 4);

        Account* account = nullptr;
		// Convert accountType to lowercase for comparison
        std::string typeLower = accountType;
        std::transform(typeLower.begin(), typeLower.end(), typeLower.begin(), ::tolower);
        if (typeLower == "savings") {
            account = new SavingsAccount(accountNumber, nullptr, balance, interestRate);
        }
        else if (typeLower == "chequing") {
            account = new CheckingAccount(accountNumber, nullptr, balance);
        }
        else if (typeLower == "business") {
            account = new BusinessAccount(accountNumber, nullptr, balance);
        }
		// Add other account types if needed.

        if (account) {
            account->databaseId = accountId;
            accounts.push_back(account);
        }
    }

    sqlite3_finalize(stmt);
    return accounts;
}
