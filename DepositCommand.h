#ifndef DEPOSITCOMMAND_H
#define DEPOSITCOMMAND_H

#include "ICommand.h"
#include "Account.h"
#include "TransactionManager.h"
#include "Bank.h"

class DepositCommand : public ICommand {
    Account* account;
    double amount;
    TransactionManager* txnManager;
    Bank* bank; // Added pointer to Bank for database updates

public:
    // Modified constructor to accept a valid Bank pointer
    DepositCommand(Account* acc, double amt, TransactionManager* tm, Bank* bankInstance)
        : account(acc), amount(amt), txnManager(tm), bank(bankInstance) {}
    void execute() override;
    void undo() override;
};

#endif // DEPOSITCOMMAND_H
