#ifndef WITHDRAWALCOMMAND_H
#define WITHDRAWALCOMMAND_H

#include "ICommand.h"
#include "Account.h"
#include "TransactionManager.h"

class WithdrawalCommand : public ICommand {
    Account* account;
    double amount;
    TransactionManager* txnManager;
public:
    WithdrawalCommand(Account* acc, double amt, TransactionManager* tm);
    void execute() override;
    void undo() override;
};

#endif // WITHDRAWALCOMMAND_H
