#ifndef SAVINGSACCOUNT_H
#define SAVINGSACCOUNT_H

#include "Account.h"

class SavingsAccount : public Account {
public:
    double interestRate;
    SavingsAccount(const std::string& accNum, Customer* cust, double initialBalance, double rate = 0.0225);
    void deposit(double amount) override;
    bool withdraw(double amount) override;
    void applyInterest() override;
    std::string getAccountType() const override;
};

#endif // SAVINGSACCOUNT_H
