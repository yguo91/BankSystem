#ifndef CHECKINGACCOUNT_H
#define CHECKINGACCOUNT_H

#include "Account.h"

class CheckingAccount : public Account {
public:
    double overdraftLimit;
    CheckingAccount(const std::string& accNum, Customer* cust, double initialBalance, double overdraft = 500);
    void deposit(double amount) override;
    bool withdraw(double amount) override;
    void applyInterest() override;
    std::string getAccountType() const override;
};

#endif // CHECKINGACCOUNT_H
