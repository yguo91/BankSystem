#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
protected:
    std::string accountNumber;
    double balance;

public:
    Account(std::string accNum, double bal) : accountNumber(accNum), balance(bal) {}

    virtual void deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;

    std::string getAccountNumber() const;
    double getBalance() const;
};

#endif // ACCOUNT_H

