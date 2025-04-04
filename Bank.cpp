#include "Bank.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "DepositTransaction.h"
#include "WithdrawalTransaction.h"
#include "TransferTransaction.h"
#include "Transaction.h"

Bank::Bank() {
    logger = Logger::getInstance();
    configManager = ConfigManager::getInstance();
    databaseManager = new DatabaseManager(); // Initialize the database connection

    // Initialize the account number counter based on the database.
    AccountFactory::initializeCounter(databaseManager);
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

void Bank::cacheCustomer(Customer* customer)
{
    customers.push_back(customer);
}

void Bank::removeCustomer(const std::string& customerID) {
    customers.erase(std::remove_if(customers.begin(), customers.end(), [&](Customer* c) {
        return c->customerID == customerID;
        }), customers.end());
}

Account* Bank::createAccount(Customer* targetCustomer, AccountType type, double initialBalance, Customer* initiator) {
	// Check if the initiator is an admin.
    if (initiator == nullptr || initiator->role != Role::Admin) {
        logger->log("Permission denied: Only admin can create accounts for customers.");
        return nullptr;
    }
    if (targetCustomer == nullptr) {
        logger->log("Error: Target customer is null.");
        return nullptr;
    }
    
    Account* acc = AccountFactory::createAccount(type, targetCustomer, initialBalance);
    if (!acc) {
        logger->log("Error: Account creation failed.");
        return nullptr;
    }

    targetCustomer->addAccount(acc);
    accounts.push_back(acc);

	// Determine the account type for logging and database insertion.
    std::string accountTypeStr;
    double interestRate = 0.0;
    if (type == AccountType::Savings) {
        accountTypeStr = "savings";
		// If this is a Savings account, set the interest rate.
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

	// Insert the account into the database.
    int userId = 0;
    try {
        userId = std::stoi(targetCustomer->customerID);
    }
    catch (std::exception& e) {
        logger->log("Error converting customerID to integer: " + targetCustomer->customerID);
    }

    if (databaseManager->insertAccount(userId, acc->accountNumber, accountTypeStr, initialBalance, interestRate)) {
		// Set the database ID for the account.
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

// Delete (Close) Account
bool Bank::deleteAccount(Customer* customer, const std::string& accountNumber) {
    // Find the account in customer's account list.
    auto it = std::find_if(customer->accounts.begin(), customer->accounts.end(),
        [&](Account* a) { return a->accountNumber == accountNumber; });
    if (it != customer->accounts.end()) {
        Account* account = *it;
        // Remove from database.
        if (databaseManager->deleteAccount(account->databaseId)) {
            // Remove from Bank's account vector.
            accounts.erase(std::remove(accounts.begin(), accounts.end(), account), accounts.end());
            // Remove from customer's account list.
            customer->removeAccount(accountNumber);
            Logger::getInstance()->log("Deleted account: " + account->accountNumber);
            // Free memory if appropriate.
            delete account;
            return true;
        }
        else {
            Logger::getInstance()->log("Failed to delete account in database: " + account->accountNumber);
        }
    }
    else {
        Logger::getInstance()->log("Account not found: " + accountNumber);
    }
    return false;
}

// Update Account Details
bool Bank::updateAccountDetails(Customer* currentUser, const std::string& accountNumber, double newBalance, double newInterestRate) {
    if (currentUser->role != Role::Admin) {
        Logger::getInstance()->log("Permission denied: Only admin can modify account details.");
        return false;
    }

    // Find the account in the global accounts list.
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [&](Account* a) { return a->accountNumber == accountNumber; });

    Account* account = nullptr;
    if (it != accounts.end()) {
        account = *it;
    }
    else {
        // Account not found in memory, try to load it from the database.
        account = databaseManager->getAccountByAccountNumber(accountNumber);
        if (account) {
            // Add the loaded account to the global accounts list.
            accounts.push_back(account);
            Logger::getInstance()->log("Loaded account " + accountNumber + " from the database.");
        }
        else {
            Logger::getInstance()->log("Account not found for update: " + accountNumber);
            return false;
        }
    }

    // Debug log: Print out the account's database ID and account number.
    Logger::getInstance()->log("Debug: Updating account with accountNumber: " + account->accountNumber +
        ", databaseId: " + std::to_string(account->databaseId));

    // Update the in-memory account details.
    account->balance = newBalance;

    // If the account is a Savings account, update the interest rate.
    if (account->getAccountType() == "Savings") {
        SavingsAccount* sAcc = dynamic_cast<SavingsAccount*>(account);
        if (sAcc) {
            sAcc->interestRate = newInterestRate;
        }
    }
    else {
        // For non-savings accounts, set the interest rate to 0.
        newInterestRate = 0;
    }
	
    // Update the account details in the database.
    if (databaseManager->updateAccount(account->databaseId, newBalance, newInterestRate)) {
        Logger::getInstance()->log("Updated account details for: " + account->accountNumber);
        return true;
    }
    else {
        Logger::getInstance()->log("Failed to update account details in database for: " + account->accountNumber);
        return false;
    }
}

// Apply Interest to a Savings Account
bool Bank::applyInterestToAccount(Customer* customer, const std::string& accountNumber) {
    Account* account = nullptr;

	// If the customer is an admin, directly get the account from the database.
    if (customer->role == Role::Admin) {
        account = databaseManager->getAccountByAccountNumber(accountNumber);
    }
    else {
		// Find the account in the customer's account list.
        auto it = std::find_if(customer->accounts.begin(), customer->accounts.end(),
            [&](Account* a) { return a->accountNumber == accountNumber; });
        if (it != customer->accounts.end()) {
            account = *it;
        }
    }

    if (account) {
		// Check if the account is a Savings account.
        if (account->getAccountType() == "Savings") {
			// Apply interest to the account.
            account->applyInterest();
			// Update the account balance in the database.
            if (databaseManager->updateAccountBalance(account->databaseId, account->balance)) {
                Logger::getInstance()->log("Applied interest to account: " + account->accountNumber);
                return true;
            }
            else {
                Logger::getInstance()->log("Failed to update account balance after applying interest for: " + account->accountNumber);
            }
        }
        else {
            Logger::getInstance()->log("Interest can only be applied to Savings accounts: " + account->accountNumber);
        }
    }
    else {
        Logger::getInstance()->log("Account not found for applying interest: " + accountNumber);
    }
    return false;
}

Customer* Bank::createNewUser(const std::string& username, const std::string& password, const std::string& fullName, const std::string& email, const std::string& phone)
{
    // Directly insert the new user into the database using the plain password.
    if (!databaseManager->insertUser(username, password, fullName, email, phone)) {
        logger->log("Failed to insert new user into database: " + username);
        return nullptr;
    }

    // Create the new Customer object with the Personal role.
    Customer* newCustomer = new Customer(username, fullName, email, phone, Role::Personal);

    // Add the new customer to the Bank's customer list.
    customers.push_back(newCustomer);

    logger->log("New user created: " + username);
    return newCustomer;
}

DatabaseManager* Bank::getDatabaseManager() const {
    return databaseManager;
}

Customer* Bank::findCustomerById(const std::string& id)
{
    for (Customer* cust : customers) {
        if (cust->customerID == id) {
            return cust;
        }
    }
    // Retrieve the customer from the database.
    Customer* dbCustomer = databaseManager->getUser(id);
    if (dbCustomer) {
        // Retrieve and attach accounts to the customer.
        std::vector<Account*> accList = databaseManager->getAccountsForUser(id);
        for (Account* acc : accList) {
            acc->owner = dbCustomer;     // Set the owner pointer
            dbCustomer->addAccount(acc); // Add the account to customer's account vector
        }
        customers.push_back(dbCustomer);
    }
    return dbCustomer;
}

//  still have unfixed bug march-21, 2025
bool Bank::updateTransactionInDB(Transaction* transaction)
{
    // Check if the transaction and its source account are valid
    if (transaction == nullptr || transaction->sourceAccount == nullptr) {
        logger->log("Invalid transaction or source account in updateTransactionInDB.");
        return false;
    }

    // bebug Log the input parameters
    logger->log("updateTransactionInDB called.");
    logger->log("Transaction ID: " + transaction->transactionID);
    logger->log("Amount: " + std::to_string(transaction->amount));
    logger->log("Source Account Number: " + transaction->sourceAccount->accountNumber);
    logger->log("Source Account Database ID: " + std::to_string(transaction->sourceAccount->databaseId));

    int accountId = transaction->sourceAccount->databaseId;
    std::string transactionType;

    // Determine the transaction type using dynamic casting
    if (dynamic_cast<DepositTransaction*>(transaction))
        transactionType = "deposit";
    else if (dynamic_cast<WithdrawalTransaction*>(transaction))
        transactionType = "withdrawal";
    else if (dynamic_cast<TransferTransaction*>(transaction))
        transactionType = "transfer";
    else {
        logger->log("Unknown transaction type in updateTransactionInDB.");
        return false;
    }

    // Insert the transaction record into the database
    if (!databaseManager->insertTransaction(accountId, transactionType, transaction->amount)) {
        logger->log("Failed to insert transaction into database for account: " + transaction->sourceAccount->accountNumber);
        return false;
    }

    // Update the account balance in the database based on the in-memory value
    if (!databaseManager->updateAccountBalance(accountId, transaction->sourceAccount->balance)) {
        logger->log("Failed to update account balance in database for account: " + transaction->sourceAccount->accountNumber);
        return false;
    }

    return true;
}
