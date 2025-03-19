#include "Transaction.h"
#include <ctime>

Transaction::Transaction(const std::string& txnID, double amt, Account* src, TransactionType t)
    : transactionID(txnID), amount(amt), sourceAccount(src), type(t), status(TransactionStatus::Pending) {
    time_t now = time(0);
    char buf[26];
    ctime_s(buf, sizeof(buf), &now);
    timestamp = buf;
}
