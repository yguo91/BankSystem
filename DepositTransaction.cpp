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

    //Update in-memory balance
    sourceAccount->deposit(amount);   

    status = TransactionStatus::Completed;
    return true;
}

bool DepositTransaction::rollback() {
    if (sourceAccount && status == TransactionStatus::Completed) {
        // Attempt to withdraw the deposited amount
        if (sourceAccount->withdraw(amount)) {
            status = TransactionStatus::Pending;
            return true;
        }
    }
    return false;
}
