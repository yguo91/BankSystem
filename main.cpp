#include "DatabaseManager.h"
#include <iostream>

//this is the tester for the database manager, it can be delete while further edit. 03-17-1:49pm

int main() {
    std::cout << "Starting DatabaseManager test..." << std::endl;

    DatabaseManager dbManager;

    if (!dbManager.openDatabase()) {
        std::cerr << "Failed to open database!" << std::endl;
        return 1;
    }

    std::cout << "Database opened successfully." << std::endl;

    if (!dbManager.createTables()) {
        std::cerr << "Failed to create tables!" << std::endl;
        return 1;
    }

    std::cout << "Tables created successfully." << std::endl;

    // Test insert user
    if (dbManager.insertUser("test_user", "test_pass", "Test User", "test@example.com", "1234567890")) {
        std::cout << "User inserted successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to insert user." << std::endl;
    }

    // Test insert account
    if (dbManager.insertAccount(1, "ACC-1001", "savings", 5000.00, 2.5)) {
        std::cout << "Account inserted successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to insert account." << std::endl;
    }

    // Test insert transaction
    if (dbManager.insertTransaction(1, "deposit", 1000.00)) {
        std::cout << "Transaction inserted successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to insert transaction." << std::endl;
    }

    // Test update balance
    if (dbManager.updateAccountBalance(1, 6000.00)) {
        std::cout << "Account balance updated successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to update account balance." << std::endl;
    }

    // Test fund transfer
    if (dbManager.insertAccount(2, "ACC-1002", "chequing", 3000.00, 1.0)) {
        std::cout << "Second account inserted successfully." << std::endl;
    }
    if (dbManager.transferFunds(1, 2, 500.00)) {
        std::cout << "Funds transferred successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to transfer funds." << std::endl;
    }

    dbManager.closeDatabase();
    std::cout << "Database closed successfully." << std::endl;

    return 0;
}