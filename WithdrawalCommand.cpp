#include "WithdrawalCommand.h"
#include "WithdrawalTransaction.h"

WithdrawalCommand::WithdrawalCommand(Account* acc, double amt, TransactionManager* tm)
    : account(acc), amount(amt), txnManager(tm) {}

void WithdrawalCommand::execute() {
    WithdrawalTransaction* txn = new WithdrawalTransaction("WDR_CMD", amount, account);
    txnManager->executeTransaction(txn);
}

void WithdrawalCommand::undo() {
    // Implement undo logic if needed.
}
