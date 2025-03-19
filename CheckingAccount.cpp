#include "CheckingAccount.h"

CheckingAccount::CheckingAccount(const std::string& accNum, Customer* cust, double initialBalance, double overdraft)
    : Account(accNum, cust, initialBalance), overdraftLimit(overdraft) {}

void CheckingAccount::deposit(double amount) {
    balance += amount;
}

bool CheckingAccount::withdraw(double amount) {
    if (amount > balance + overdraftLimit) return false;
    balance -= amount;
    return true;
}

void CheckingAccount::applyInterest() {
    // No interest for checking accounts
}

std::string CheckingAccount::getAccountType() const {
    return "Checking";
}
