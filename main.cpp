#include <iostream>
#include "Bank.h"
#include "Customer.h"
#include "SavingsAccount.h"
#include "BankFacade.h"
#include "Logger.h"

using namespace std;

void showLoginScreen(Bank& bank, Customer*& currentCustomer) {
    cout << "=====================================\n";
    cout << "          BANK SYSTEM - LOGIN        \n";
    cout << "=====================================\n";
    cout << "Enter User ID: ";
    string userID;
    cin >> userID;
    cout << "Enter Password: ";
    string password;
    // For demo, we simulate password masking.
    cin >> password;

    if (userID == "admin") {
        currentCustomer = new Customer(userID, "Administrator", "admin@bank.com", "000-0000", Role::Admin);
    }
    else {
        currentCustomer = new Customer(userID, "User " + userID, userID + "@bank.com", "1234567890", Role::Personal);
    }
    bank.addCustomer(currentCustomer);
    if (currentCustomer->role != Role::Admin) {
        bank.createAccount(currentCustomer, AccountType::Savings, 5000.0);
    }
    Logger::getInstance()->log("User " + userID + " logged in.");
}

void showDashboard(Bank& bank, Customer* customer) {
    int choice;
    do {
        cout << "\n=====================================\n";
        if (customer->role == Role::Admin) {
            cout << "        ADMIN ACCOUNT MANAGEMENT       \n";
            cout << "=====================================\n";
            cout << "1. Create New Account\n";
            cout << "2. Close Account\n";
            cout << "3. Modify Account Details\n";
            cout << "4. View All Accounts\n";
            cout << "5. Back to Main Menu\n";
            cout << "Enter choice [1-5]: ";
            cin >> choice;
            if (choice == 1) {
                cout << "Enter Account Type [1] Savings [2] Checking [3] Business: ";
                int typeChoice; cin >> typeChoice;
                AccountType type = (typeChoice == 1) ? AccountType::Savings : (typeChoice == 2 ? AccountType::Checking : AccountType::Business);
                cout << "Enter Customer ID: ";
                string custID; cin >> custID;
                cout << "Enter Initial Balance: $";
                double initBal; cin >> initBal;
                bank.createAccount(customer, type, initBal);
                Logger::getInstance()->log("Admin created new account for customer " + custID);
            }
            else if (choice == 4) {
                cout << "Listing all accounts:\n";
                for (auto acc : bank.accounts) {
                    cout << acc->accountNumber << " (" << acc->getAccountType() << ") - $" << acc->balance << "\n";
                }
            }
        }
        else {
            cout << "=====================================\n";
            cout << " Welcome, " << customer->name << " (Personal Client)\n";
            cout << "-------------------------------------\n";
            if (!customer->accounts.empty()) {
                Account* acc = customer->accounts[0];
                cout << "Account: " << acc->accountNumber << " (" << acc->getAccountType() << ")\n";
                cout << "Balance: $" << acc->balance << "\n";
                cout << "Last Login: " << "2025-01-20 14:30\n";
            }
            cout << "=====================================\n";
            cout << "1. Account Overview\n";
            cout << "2. Deposit Funds\n";
            cout << "3. Withdraw Funds\n";
            cout << "4. Transfer Money\n";
            cout << "5. Transaction History\n";
            cout << "6. Account Settings\n";
            cout << "7. Logout\n";
            cout << "Enter choice [1-7]: ";
            cin >> choice;
            BankFacade facade = bank.bankFacade;
            switch (choice) {
            case 1:
                facade.checkBalance(customer);
                break;
            case 2: {
                cout << "DEPOSIT FUNDS\nEnter deposit amount: $";
                double amt; cin >> amt;
                facade.performDeposit(customer, amt);
                break;
            }
            case 3: {
                cout << "WITHDRAW FUNDS\nEnter withdrawal amount: $";
                double amt; cin >> amt;
                facade.performWithdrawal(customer, amt);
                break;
            }
            case 4: {
                cout << "TRANSFER MONEY\nEnter destination account number: ";
                string dest; cin >> dest;
                cout << "Enter transfer amount: $";
                double amt; cin >> amt;
                facade.performTransfer(customer, dest, amt);
                break;
            }
            case 5: {
                cout << "TRANSACTION HISTORY\n";
                vector<Transaction*> history = customer->getTransactionHistory();
                for (auto txn : history) {
                    cout << txn->timestamp << " | " << txn->transactionID << " | $" << txn->amount << "\n";
                }
                break;
            }
            case 6: {
                cout << "Account Settings (Not fully implemented in demo)\n";
                break;
            }
            case 7:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        }
    } while (choice != 5 && choice != 7);
}

int main() {
    Bank bank;
    Customer* currentCustomer = nullptr;
    int mainChoice;
    do {
        cout << "\n=====================================\n";
        cout << "         BANK SYSTEM - MAIN          \n";
        cout << "=====================================\n";
        cout << "1. Login/Authentication\n";
        cout << "2. Exit\n";
        cout << "Enter choice: ";
        cin >> mainChoice;
        if (mainChoice == 1) {
            showLoginScreen(bank, currentCustomer);
            if (currentCustomer) {
                showDashboard(bank, currentCustomer);
            }
        }
    } while (mainChoice != 2);
    cout << "Thank you for using the bank system. Goodbye!\n";
    return 0;
}
