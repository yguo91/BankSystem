#ifndef BUSINESSACCOUNT_H
#define BUSINESSACCOUNT_H

#include "Account.h"

class BusinessAccount : public Account {
public:
    double transactionLimit;
    double businessFees;
    BusinessAccount(const std::string& accNum, Customer* cust, double initialBalance, double limit = 10000, double fee = 5);
    void deposit(double amount) override;
    bool withdraw(double amount) override;
    void applyInterest() override;
    void generateBusinessReport();
    std::string getAccountType() const override;
};

#endif // BUSINESSACCOUNT_H
