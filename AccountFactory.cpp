#include "AccountFactory.h"
#include <sstream>
#include <sqlite3.h>

static int accCounter = 1000000;

void AccountFactory::initializeCounter(DatabaseManager* dbManager)
{
    // Query the database for the maximum account number (numeric part).
    // Assumes account_number format is "ACC-<number>"
    std::string sql = "SELECT MAX(CAST(SUBSTR(account_number, 5) AS INTEGER)) FROM accounts;";
    sqlite3_stmt* stmt;

    // Get the raw sqlite3 pointer from DatabaseManager (assume a getter is available)
    sqlite3* db = dbManager->getDB();
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int maxVal = sqlite3_column_int(stmt, 0);
            if (maxVal >= accCounter) {
                accCounter = maxVal + 1;
            }
        }
    }
    sqlite3_finalize(stmt);
}

Account* AccountFactory::createAccount(AccountType type, Customer* owner, double initialBalance) {    
    std::stringstream ss;
    ss << "ACC-" << accCounter++;
    std::string accNum = ss.str();

    switch (type) {
    case AccountType::Savings:
        return new SavingsAccount(accNum, owner, initialBalance);
    case AccountType::Checking:
        return new CheckingAccount(accNum, owner, initialBalance);
    case AccountType::Business:
        return new BusinessAccount(accNum, owner, initialBalance);
    default:
        return nullptr;
    }
}
