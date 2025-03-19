#include "WithdrawalTransaction.h"
#include "Account.h"

WithdrawalTransaction::WithdrawalTransaction(const std::string& txnID, double amt, Account* src)
    : Transaction(txnID, amt, src, TransactionType::Withdraw) {}

bool WithdrawalTransaction::execute() {
    if (sourceAccount->withdraw(amount)) {
        status = TransactionStatus::Completed;
        return true;
    }
    status = TransactionStatus::Failed;
    return false;
}

bool WithdrawalTransaction::rollback() {
    sourceAccount->deposit(amount);
    status = TransactionStatus::Pending;
    return true;
}
