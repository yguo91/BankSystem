#ifndef TRANSACTIONFACTORY_H
#define TRANSACTIONFACTORY_H

#include "Transaction.h"
#include "DepositTransaction.h"
#include "WithdrawalTransaction.h"
#include "TransferTransaction.h"

class TransactionFactory {
public:
    static Transaction* createTransaction(TransactionType type, Account* src, Account* dest, double amount);
};

#endif // TRANSACTIONFACTORY_H
