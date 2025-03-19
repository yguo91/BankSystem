#include "TransactionFactory.h"

Transaction* TransactionFactory::createTransaction(TransactionType type, Account* src, Account* dest, double amount) {
    switch (type) {
    case TransactionType::Deposit:
        return new DepositTransaction("TXN_DEP", amount, src);
    case TransactionType::Withdraw:
        return new WithdrawalTransaction("TXN_WDR", amount, src);
    case TransactionType::Transfer:
        return new TransferTransaction("TXN_TRF", amount, src, dest);
    default:
        return nullptr;
    }
}
