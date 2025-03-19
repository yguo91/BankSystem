#ifndef TRANSFERTRANSACTION_H
#define TRANSFERTRANSACTION_H

#include "Transaction.h"

class TransferTransaction : public Transaction {
public:
    Account* destinationAccount;
    TransferTransaction(const std::string& txnID, double amt, Account* src, Account* dest);
    bool execute() override;
    bool rollback() override;
};

#endif // TRANSFERTRANSACTION_H
