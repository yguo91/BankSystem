#ifndef WITHDRAWALTRANSACTION_H
#define WITHDRAWALTRANSACTION_H

#include "Transaction.h"

class WithdrawalTransaction : public Transaction {
public:
    WithdrawalTransaction(const std::string& txnID, double amt, Account* src);
    bool execute() override;
    bool rollback() override;
};

#endif // WITHDRAWALTRANSACTION_H
