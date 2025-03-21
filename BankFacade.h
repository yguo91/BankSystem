#ifndef BANKFACADE_H
#define BANKFACADE_H

#include "Customer.h"
#include <string>

class Bank;

class BankFacade {

private:
    Bank* bank;
public:
    BankFacade(Bank* bankInstance) : bank(bankInstance) {}
    void performDeposit(Customer* customer, const std::string& accNum, double amount);
    void performWithdrawal(Customer* customer, const std::string& accNum, double amount);
    void performTransfer(Customer* customer, const std::string& sourceAccNum, const std::string& destAccNum, double amount);
    void checkBalance(Customer* customer);


    BankFacade() = default;
};

#endif // BANKFACADE_H
