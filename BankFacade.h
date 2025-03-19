#ifndef BANKFACADE_H
#define BANKFACADE_H

#include "Customer.h"
#include <string>

class BankFacade {
public:
    void performDeposit(Customer* customer, double amount);
    void performWithdrawal(Customer* customer, double amount);
    void performTransfer(Customer* customer, const std::string& destAccNum, double amount);
    void checkBalance(Customer* customer);
};

#endif // BANKFACADE_H
