#ifndef BANKFACADE_H
#define BANKFACADE_H

#include "Customer.h"
#include <string>

class Bank; // Forward declaration

class BankFacade {
public:
    // Deposit into a specified account.
    void performDeposit(Customer* customer, const std::string& accountNumber, double amount);

    // Withdraw from a specified account.
    void performWithdrawal(Customer* customer, const std::string& accountNumber, double amount);

    // Transfer funds: requires a pointer to the Bank so we can find the destination account.
    void performTransfer(Bank* bank, Customer* customer, const std::string& sourceAccNum, const std::string& destAccNum, double amount);

    // Check balance for all customer's accounts.
    void checkBalance(Customer* customer);
};

#endif // BANKFACADE_H
