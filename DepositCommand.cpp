#include "DepositCommand.h"
#include "DepositTransaction.h"

DepositCommand::DepositCommand(Account* acc, double amt, TransactionManager* tm)
    : account(acc), amount(amt), txnManager(tm) {}

void DepositCommand::execute() {
    DepositTransaction* txn = new DepositTransaction("DEP_CMD", amount, account);
    txnManager->executeTransaction(txn);
}

void DepositCommand::undo() {
    // Implement undo logic if needed.
}
