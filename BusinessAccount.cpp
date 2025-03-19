#include "BusinessAccount.h"
#include <iostream>

BusinessAccount::BusinessAccount(const std::string& accNum, Customer* cust, double initialBalance, double limit, double fee)
    : Account(accNum, cust, initialBalance), transactionLimit(limit), businessFees(fee) {}

void BusinessAccount::deposit(double amount) {
    balance += amount;
}

bool BusinessAccount::withdraw(double amount) {
    if (amount + businessFees > balance || amount > transactionLimit) return false;
    balance -= (amount + businessFees);
    return true;
}

void BusinessAccount::applyInterest() {
    // Business accounts may not accrue interest.
}

void BusinessAccount::generateBusinessReport() {
    std::cout << "Generating business report for " << accountNumber << std::endl;
}

std::string BusinessAccount::getAccountType() const {
    return "Business";
}
