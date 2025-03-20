#include "DepositTransaction.h"
#include "DatabaseManager.h"
#include "Account.h"

DepositTransaction::DepositTransaction(const std::string& txnID, double amt, Account* src)
    : Transaction(txnID, amt, src, TransactionType::Deposit) {}

bool DepositTransaction::execute() {
    if (!sourceAccount || amount <= 0) {
        status = TransactionStatus::Failed;
        return false;
    }

    // 1. Update in-memory balance
    sourceAccount->deposit(amount);

    // 2. Insert transaction + update balance in DB (if dbManager is available)
    if (dbManager) {
        // Insert the deposit transaction
        if (!dbManager->insertTransaction(sourceAccount->databaseId, "deposit", amount)) {
            // If insertion fails, revert the in-memory deposit or handle the error
            sourceAccount->withdraw(amount);
            status = TransactionStatus::Failed;
            return false;
        }
        // Update the account balance in the DB
        if (!dbManager->updateAccountBalance(sourceAccount->databaseId, sourceAccount->balance)) {
            // If the balance update fails, revert the in-memory deposit or handle the error
            sourceAccount->withdraw(amount);
            status = TransactionStatus::Failed;
            return false;
        }
    }

    status = TransactionStatus::Completed;
    return true;
}

bool DepositTransaction::rollback() {
    if (sourceAccount->withdraw(amount)) {
        // Also remove or log a reversal transaction in DB
        // if dbManager is available.
        status = TransactionStatus::Pending;
        return true;
    }
    return false;
}
