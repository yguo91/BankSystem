#include "DepositTransaction.h"
#include "Account.h"

DepositTransaction::DepositTransaction(const std::string& txnID, double amt, Account* src)
    : Transaction(txnID, amt, src, TransactionType::Deposit) {}

bool DepositTransaction::execute() {
    sourceAccount->deposit(amount);
    status = TransactionStatus::Completed;
    return true;
}

bool DepositTransaction::rollback() {
    sourceAccount->withdraw(amount);
    status = TransactionStatus::Pending;
    return true;
}
