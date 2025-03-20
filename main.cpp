#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
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
        bank.createAccount(currentCustomer, AccountType::Checking, 1000.0);
    }
    Logger::getInstance()->log("User " + userID + " logged in.");
}

void showAdminNewUserRegistration(Bank& bank) {
    cout << "=====================================\n";
    cout << "       NEW USER REGISTRATION         \n";
    cout << "=====================================\n";

    // Since the admin is already logged in, we do not need to re-validate admin credentials.
    string username, password, fullName, email, phone;
    int roleChoice;

    cout << "Enter new user's username: ";
    cin >> username;
    cout << "Enter password for new user: ";
    cin >> password;
    cout << "Enter full name: ";
    cin.ignore(); // Clear the newline character.
    getline(cin, fullName);
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter phone: ";
    cin >> phone;
	//default role is personal
    Role newUserRole = Role::Personal;

    Customer* newCustomer = bank.createNewUser(username, password, fullName, email, phone);

    if (newCustomer) {
        cout << "New user '" << username << "' created successfully.\n";
        Logger::getInstance()->log("Admin created new user: " + username);
    }
    else {
        cout << "Failed to create new user. Please check logs for details.\n";
    }
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
			cout << "5. Create New User\n";  //new ui for creating new user
            cout << "6. Back to Main Menu\n";
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
                    cout << acc->accountNumber << " (" << acc->getAccountType() << ") - $"
                        << fixed << setprecision(2) << acc->balance << "\n";
                }
            }
            else if (choice == 5) {
                // Call the new user registration function directly from the admin dashboard.
                showAdminNewUserRegistration(bank);            
            }
        }
        else {
            cout << "=====================================\n";
            cout << " Welcome, " << customer->name << " (Personal Client)\n";
            cout << "-------------------------------------\n";
            if (!customer->accounts.empty()) {
                cout << "Accounts:\n";
                for (auto acc : customer->accounts) {
                    cout << " - " << acc->accountNumber << " (" << acc->getAccountType()
                        << ") - $" << fixed << setprecision(2) << acc->balance << "\n";
                }
            }
            else {
                cout << "No accounts available.\n";
            }
            cout << "=====================================\n";
            cout << "1. Account Overview\n";
            cout << "2. Deposit Funds\n";
            cout << "3. Withdraw Funds\n";
            cout << "4. Transfer Money\n";
            cout << "5. Transaction History\n";
            cout << "6. Account Settings\n";
            cout << "7. Logout\n";
            cout << "8. Delete Account\n";
            cout << "9. Update Account Details\n";
            cout << "10. Apply Interest (Savings only)\n";
            cout << "Enter choice: ";
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
                break;
            case 8: {
                cout << "Enter account number to delete: ";
                string accNum; cin >> accNum;
                if (bank.deleteAccount(customer, accNum))
                    cout << "Account deleted successfully.\n";
                else
                    cout << "Failed to delete account.\n";
                break;
            }
            case 9: {
                cout << "Enter account number to update: ";
                string accNum; cin >> accNum;
                cout << "Enter new balance: $";
                double newBalance; cin >> newBalance;
                cout << "Enter new interest rate (if applicable, else 0): ";
                double newInterestRate; cin >> newInterestRate;
                if (bank.updateAccountDetails(customer, accNum, newBalance, newInterestRate))
                    cout << "Account updated successfully.\n";
                else
                    cout << "Failed to update account.\n";
                break;
            }
            case 10: {
                cout << "Enter savings account number to apply interest: ";
                string accNum; cin >> accNum;
                if (bank.applyInterestToAccount(customer, accNum))
                    cout << "Interest applied successfully.\n";
                else
                    cout << "Failed to apply interest.\n";
                break;
            }
          }
        }
    } while ((customer->role == Role::Admin && choice != 6) ||
        (customer->role != Role::Admin && choice != 7));
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
