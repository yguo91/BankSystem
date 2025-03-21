#include "TransactionManager.h"

bool TransactionManager::validateTransaction(Transaction* transaction) {
    // Assume all transactions are valid for demo purposes.
    return true;
}

bool TransactionManager::executeTransaction(Transaction* transaction) {
    if (validateTransaction(transaction)) {
        bool result = transaction->execute();
        transactionLog.push_back(transaction);
        return result;
    }
    return false;   
}

bool TransactionManager::rollbackTransaction(Transaction* transaction) {
    return transaction->rollback();
}
