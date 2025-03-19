#include "SavingsAccount.h"
#include "Transaction.h"
#include <ctime>
#include <string>

SavingsAccount::SavingsAccount(const std::string& accNum, Customer* cust, double initialBalance, double rate)
    : Account(accNum, cust, initialBalance), interestRate(rate) {}

void SavingsAccount::deposit(double amount) {
    balance += amount;
    // (For simplicity, transaction recording is omitted)
}

bool SavingsAccount::withdraw(double amount) {
    if (amount > balance) return false;
    balance -= amount;
    return true;
}

void SavingsAccount::applyInterest() {
    double interest = balance * interestRate;
    balance += interest;
}

std::string SavingsAccount::getAccountType() const {
    return "Savings";
}
