#include "TransferCommand.h"
#include "TransferTransaction.h"

TransferCommand::TransferCommand(Account* src, Account* dest, double amt, TransactionManager* tm)
    : sourceAccount(src), destinationAccount(dest), amount(amt), txnManager(tm) {}

void TransferCommand::execute() {
    TransferTransaction* txn = new TransferTransaction("TRF_CMD", amount, sourceAccount, destinationAccount);
    txnManager->executeTransaction(txn);
}

void TransferCommand::undo() {
    // Implement undo logic if needed.
}
