#include "DepositCommand.h"
#include "DepositTransaction.h"


//DepositCommand::DepositCommand(Account* acc, double amt, TransactionManager* tm)
//    : account(acc), amount(amt), txnManager(tm) {}

void DepositCommand::execute() {
    DepositTransaction* txn = new DepositTransaction("DEP_CMD", amount, account);

    //// Log in the command before executing the transaction
    //Logger::getInstance()->log("DepositCommand: Before executing transaction.");
    //Logger::getInstance()->log("Account Number: " + account->accountNumber);
    //Logger::getInstance()->log("Account Database ID: " + std::to_string(account->databaseId));

    txnManager->executeTransaction(txn);

    // Log before calling updateTransactionInDB
    /*Logger::getInstance()->log("DepositCommand: Before calling updateTransactionInDB.");

    Logger::getInstance()->log("updateTransactionInDB called.");
    Logger::getInstance()->log("Transaction ID: " + txn->transactionID);
    Logger::getInstance()->log("Amount: " + std::to_string(txn->amount));
    Logger::getInstance()->log("Source Account Number: " + txn->sourceAccount->accountNumber);
    Logger::getInstance()->log("Source Account Database ID: " + std::to_string(txn->sourceAccount->databaseId));*/

    // Update the database records based on the transaction
    //bank->updateTransactionInDB(txn);
}

void DepositCommand::undo() {
    // Implement undo logic if needed.
}
