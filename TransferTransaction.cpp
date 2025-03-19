#include "TransferTransaction.h"
#include "Account.h"

TransferTransaction::TransferTransaction(const std::string& txnID, double amt, Account* src, Account* dest)
    : Transaction(txnID, amt, src, TransactionType::Transfer), destinationAccount(dest) {}

bool TransferTransaction::execute() {
    if (sourceAccount->withdraw(amount)) {
        destinationAccount->deposit(amount);
        status = TransactionStatus::Completed;
        return true;
    }
    status = TransactionStatus::Failed;
    return false;
}

bool TransferTransaction::rollback() {
    if (destinationAccount->withdraw(amount)) {
        sourceAccount->deposit(amount);
        status = TransactionStatus::Pending;
        return true;
    }
    return false;
}
