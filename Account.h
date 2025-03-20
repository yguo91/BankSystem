#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

class Transaction;
class Customer;

class Account {
public:
    std::string accountNumber;
    double balance;
    Customer* owner;
    std::vector<Transaction*> transactions;
    int databaseId;  // Added: stores the primary key from the database

    Account(const std::string& accNum, Customer* cust, double initialBalance = 0);
    virtual ~Account() {}
    virtual void deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;
    virtual void applyInterest() = 0;
    virtual std::string getAccountType() const = 0;
};

#endif // ACCOUNT_H
