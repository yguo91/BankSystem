#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Account;

enum class TransactionStatus { Pending, Completed, Failed };
enum class TransactionType { Deposit, Withdraw, Transfer };

class Transaction {
public:
    std::string transactionID;
    double amount;
    std::string timestamp;
    Account* sourceAccount;
    TransactionStatus status;
    TransactionType type;

    Transaction(const std::string& txnID, double amt, Account* src, TransactionType t);
    virtual ~Transaction() {}
    virtual bool execute() = 0;
    virtual bool rollback() = 0;
};

#endif // TRANSACTION_H
