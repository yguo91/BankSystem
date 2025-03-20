#include "Bank.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "DepositTransaction.h"
#include "WithdrawalTransaction.h"
#include "TransferTransaction.h"

Bank::Bank() {
    logger = Logger::getInstance();
    configManager = ConfigManager::getInstance();
    databaseManager = new DatabaseManager(); // Initialize the database connection
}

Bank::~Bank() {
    // Clean up the database manager
    delete databaseManager;
}

void Bank::addCustomer(Customer* customer) {
    customers.push_back(customer);
    // Insert customer into database.
    if (!databaseManager->insertUser(customer->name, "default_password", customer->name, customer->email, customer->phone)) {
        logger->log("Failed to insert customer into database: " + customer->name);
    }
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
    
    // Determine the account type string and interest rate for the database.
    std::string accountTypeStr;
    double interestRate = 0.0;
    if (type == AccountType::Savings) {
        accountTypeStr = "savings";
        // Assuming SavingsAccount has an interestRate attribute:
        SavingsAccount* sAcc = dynamic_cast<SavingsAccount*>(acc);
        if (sAcc)
            interestRate = sAcc->interestRate;
    }
    else if (type == AccountType::Checking) {
        accountTypeStr = "chequing";
    }
    else if (type == AccountType::Business) {
        accountTypeStr = "business";
    }
    else {
        accountTypeStr = "unknown";
    }

    // Convert customer's customerID to an integer (assuming it represents a user ID).
    int userId = 0;
    try {
        userId = std::stoi(customer->customerID);
    }
    catch (std::exception& e) {
        // Handle the conversion error as needed.
        logger->log("Error converting customerID to integer: " + customer->customerID);
    }

    if (databaseManager->insertAccount(userId, acc->accountNumber, accountTypeStr, initialBalance, interestRate)) {
        // After a successful insertion, set the account's databaseId.
        // This assumes you have added a method like getLastInsertId() to DatabaseManager.
        acc->databaseId = databaseManager->getLastInsertId();
    }
    else {
        logger->log("Failed to insert account into database: " + acc->accountNumber);
    }

    return acc;


}

bool Bank::processTransaction(Transaction* transaction) {
    bool success = transactionManager.executeTransaction(transaction);
    if (success) {
        int accountId = transaction->sourceAccount->databaseId;
        std::string transactionType;
        if (dynamic_cast<DepositTransaction*>(transaction))
            transactionType = "deposit";
        else if (dynamic_cast<WithdrawalTransaction*>(transaction))
            transactionType = "withdrawal";
        else if (dynamic_cast<TransferTransaction*>(transaction))
            transactionType = "transfer";

		//check if transaction is null
        if (transaction == nullptr) {
            logger->log("Error: Null transaction pointer");
            return false;
        }

        double amount = transaction->amount;

        if (transactionType == "transfer") {
            // Cast to TransferTransaction to access destinationAccount safely.
            TransferTransaction* transferTxn = dynamic_cast<TransferTransaction*>(transaction);
            if (transferTxn && transferTxn->destinationAccount) {
                int recipientAccountId = transferTxn->destinationAccount->databaseId;
                if (!databaseManager->transferFunds(accountId, recipientAccountId, amount)) {
                    logger->log("Failed to process fund transfer in database for account: " + transaction->sourceAccount->accountNumber);
                    return false;
                }
            }
            else {
                logger->log("Error: Invalid transfer transaction or missing destination account");
                return false;
            }
        }
        else {
            if (!databaseManager->insertTransaction(accountId, transactionType, amount)) {
                logger->log("Failed to insert transaction into database for account: " + transaction->sourceAccount->accountNumber);
                return false;
            }
            // Also update the account balance in the database.
            if (!databaseManager->updateAccountBalance(accountId, transaction->sourceAccount->balance)) {
                logger->log("Failed to update account balance in database for account: " + transaction->sourceAccount->accountNumber);
                return false;
            }
        }
    }
    return success;
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
