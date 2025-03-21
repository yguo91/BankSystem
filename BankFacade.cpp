#include "BankFacade.h"
#include "DepositCommand.h"
#include "WithdrawalCommand.h"
#include "TransferCommand.h"
#include "Logger.h"
#include "TransactionManager.h"
#include "AccountFactory.h"
#include "Account.h"
#include "Customer.h"
#include "Bank.h"
#include <iostream>
#include <algorithm>

// Deposit function now accepts an account number.
void BankFacade::performDeposit(Customer* customer, const std::string& accountNumber, double amount) {
    Account* account = nullptr;
    for (auto acc : customer->accounts) {
        if (acc->accountNumber == accountNumber) {
            account = acc;
            break;
        }
    }
    if (!account) {
        std::cout << "Account " << accountNumber << " not found.\n";
        return;
    }
    TransactionManager tm;
    DepositCommand cmd(account, amount, &tm);
    cmd.execute();
    Logger::getInstance()->log("Deposited $" + std::to_string(amount) + " into account " + account->accountNumber);
}

// Withdrawal function now accepts an account number.
void BankFacade::performWithdrawal(Customer* customer, const std::string& accountNumber, double amount) {
    Account* account = nullptr;
    for (auto acc : customer->accounts) {
        if (acc->accountNumber == accountNumber) {
            account = acc;
            break;
        }
    }
    if (!account) {
        std::cout << "Account " << accountNumber << " not found.\n";
        return;
    }
    TransactionManager tm;
    WithdrawalCommand cmd(account, amount, &tm);
    cmd.execute();
    Logger::getInstance()->log("Withdrew $" + std::to_string(amount) + " from account " + account->accountNumber);
}

// Transfer function now accepts a Bank pointer to look up the destination account.
void BankFacade::performTransfer(Bank* bank, Customer* customer, const std::string& sourceAccNum, const std::string& destAccNum, double amount) {
    // Find source account in customer's accounts.
    Account* sourceAccount = nullptr;
    for (auto acc : customer->accounts) {
        if (acc->accountNumber == sourceAccNum) {
            sourceAccount = acc;
            break;
        }
    }
    if (!sourceAccount) {
        std::cout << "Source account " << sourceAccNum << " not found.\n";
        return;
    }

    // Use bank's function to find the destination account.
    Account* destAccount = bank->findAccountByNumber(destAccNum);
    if (!destAccount) {
        std::cout << "Destination account " << destAccNum << " not found.\n";
        return;
    }

    TransactionManager tm;
    TransferCommand cmd(sourceAccount, destAccount, amount, &tm);
    cmd.execute();
    Logger::getInstance()->log("Transferred $" + std::to_string(amount) + " from account "
        + sourceAccount->accountNumber + " to " + destAccount->accountNumber);
}

// Check balance for all accounts of the customer.
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
