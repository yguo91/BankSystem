#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <vector>
#include "Transaction.h"

class TransactionManager {
public:
    std::vector<Transaction*> transactionLog;
    bool validateTransaction(Transaction* transaction);
    bool executeTransaction(Transaction* transaction);
    bool rollbackTransaction(Transaction* transaction);
};

#endif // TRANSACTIONMANAGER_H
