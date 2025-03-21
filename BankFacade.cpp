#include "BankFacade.h"
#include "Bank.h"
#include "DepositCommand.h"
#include "WithdrawalCommand.h"
#include "TransferCommand.h"
#include "Logger.h"
#include "TransactionManager.h"
#include "AccountFactory.h"
#include "Account.h"
#include "Customer.h"
#include <iostream>

void BankFacade::performDeposit(Customer* customer, double amount) {
    if (customer->accounts.empty()) {
        std::cout << "No accounts available.\n";
        return;
    }
    Account* account = customer->accounts[0];
    TransactionManager tm;
    DepositCommand cmd(account, amount, &tm, bank);
    cmd.execute();
    Logger::getInstance()->log("Deposited $" + std::to_string(amount) + " into account " + account->accountNumber);
}

void BankFacade::performWithdrawal(Customer* customer, double amount) {
    if (customer->accounts.empty()) {
        std::cout << "No accounts available.\n";
        return;
    }
    Account* account = customer->accounts[0];
    TransactionManager tm;
    WithdrawalCommand cmd(account, amount, &tm);
    cmd.execute();
    Logger::getInstance()->log("Withdrew $" + std::to_string(amount) + " from account " + account->accountNumber);
}

void BankFacade::performTransfer(Customer* customer, const std::string& destAccNum, double amount) {
    if (customer->accounts.empty()) {
        std::cout << "No accounts available.\n";
        return;
    }
    Account* sourceAccount = customer->accounts[0];
    // For demonstration, we create a dummy destination account.
    Customer dummy("C002", "Dummy", "dummy@bank.com", "000-0000", Role::Personal);
    Account* destAccount = AccountFactory::createAccount(AccountType::Checking, &dummy, 0);
    TransactionManager tm;
    TransferCommand cmd(sourceAccount, destAccount, amount, &tm);
    cmd.execute();
    Logger::getInstance()->log("Transferred $" + std::to_string(amount) + " from account " + sourceAccount->accountNumber + " to " + destAccount->accountNumber);
}

void BankFacade::checkBalance(Customer* customer) {
    if (customer->accounts.empty()) {
        std::cout << "No accounts available.\n";
        return;
    }
    for (auto account : customer->accounts) {
        std::cout << "Account: " << account->accountNumber 
                  << "\nType: " << account->getAccountType()
                  << "\nBalance: $" << account->balance << "\n";
    }
}
