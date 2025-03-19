#ifndef TRANSFERCOMMAND_H
#define TRANSFERCOMMAND_H

#include "ICommand.h"
#include "Account.h"
#include "TransactionManager.h"

class TransferCommand : public ICommand {
    Account* sourceAccount;
    Account* destinationAccount;
    double amount;
    TransactionManager* txnManager;
public:
    TransferCommand(Account* src, Account* dest, double amt, TransactionManager* tm);
    void execute() override;
    void undo() override;
};

#endif // TRANSFERCOMMAND_H
