#ifndef DEPOSITCOMMAND_H
#define DEPOSITCOMMAND_H

#include "ICommand.h"
#include "Account.h"
#include "TransactionManager.h"

class DepositCommand : public ICommand {
    Account* account;
    double amount;
    TransactionManager* txnManager;
public:
    DepositCommand(Account* acc, double amt, TransactionManager* tm);
    void execute() override;
    void undo() override;
};

#endif // DEPOSITCOMMAND_H
