#ifndef DEPOSITTRANSACTION_H
#define DEPOSITTRANSACTION_H

#include "Transaction.h"

class DepositTransaction : public Transaction {
public:
    DepositTransaction(const std::string& txnID, double amt, Account* src);
    bool execute() override;
    bool rollback() override;
};

#endif // DEPOSITTRANSACTION_H
