#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Bank.h"
#include "Customer.h"
#include "SavingsAccount.h"
#include "BankFacade.h"
#include "Logger.h"
#include <conio.h>

using namespace std;

std::string getPasswordInput();

void showLoginScreen(Bank& bank, Customer*& currentCustomer) {
    cout << "=====================================\n";
    cout << "          BANK SYSTEM - LOGIN        \n";
    cout << "=====================================\n";
    cout << "Enter User ID: ";
    string userID;
    cin >> userID;
    std::string password = getPasswordInput();

    if (userID == "admin") {
        currentCustomer = new Customer(userID, "Administrator", "admin@bank.com", "000-0000", Role::Admin);
    }
    else {
        //currentCustomer = new Customer(userID, "User " + userID, userID + "@bank.com", "1234567890", Role::Personal);
        // Validate credentials using the DatabaseManager with userID
        if (bank.getDatabaseManager()->validateUser(userID, password)) {
            Customer* userFromDB = bank.findCustomerById(userID);
            if (userFromDB) {
                currentCustomer = userFromDB;
            }
            else {
                std::cout << "User details could not be retrieved from the database." << std::endl;
                currentCustomer = nullptr;
                return;
            }
        }
    }
    //bank.addCustomer(currentCustomer);
    /*if (currentCustomer->role != Role::Admin) {
        bank.createAccount(currentCustomer, AccountType::Checking, 1000.0);
    }*/
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

// Function to securely get password input from the user
std::string getPasswordInput() {
    std::string password;
    char ch;
    std::cout << "Enter Password: ";
    while ((ch = _getch()) != '\r') { // '\r' is carriage return, equivalent to Enter key
        if (ch == '\b') { // Handle backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // Erase character from console
            }
        }
        else {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    return password;
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
            cout << "6. Apply Interest (Savings only)\n";
            cout << "7. Back to Main Menu\n";
            cout << "Enter choice [1-7]: ";
            cin >> choice;
            if (choice == 1) {
                cout << "Enter Account Type [1] Savings [2] Checking [3] Business: ";
                int typeChoice; cin >> typeChoice;
                AccountType type = (typeChoice == 1) ? AccountType::Savings : (typeChoice == 2 ? AccountType::Checking : AccountType::Business);
                cout << "Enter Customer ID: ";
                string custID; 
                cin >> custID;
                Customer* targetCustomer = bank.findCustomerById(custID);
                if (targetCustomer != nullptr) {
                    cout << "Enter Initial Balance: $";
                    double initBal;
                    cin >> initBal;
                    bank.createAccount(targetCustomer, type, initBal, customer);
                    Logger::getInstance()->log("Admin created new account for customer " + custID);
                }
                else {
                    cout << "Customer not found.\n";
                }

                /*cout << "Enter Initial Balance: $";
                double initBal; cin >> initBal;
                bank.createAccount(customer, type, initBal);
                Logger::getInstance()->log("Admin created new account for customer " + custID);*/
            }
            else if (choice == 2) {
                cout << "Enter Account Number to close: ";
                string accNum; cin >> accNum;
                if (bank.deleteAccount(customer, accNum))
                    cout << "Account closed successfully.\n";
                else
                    cout << "Failed to close account.\n";
            }
            else if (choice == 3) {
                cout << "Enter Account Number to modify: ";
                string accNum; cin >> accNum;
                cout << "Enter new balance: $";
                double newBalance; cin >> newBalance;
                cout << "Enter new interest rate (if applicable, else 0): ";
                double newInterestRate; cin >> newInterestRate;
                if (bank.updateAccountDetails(customer, accNum, newBalance, newInterestRate))
                    cout << "Account modified successfully.\n";
                else
                    cout << "Failed to modify account.\n";
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
            else if (choice == 6) {
                cout << "Enter savings account number to apply interest: ";
                string accNum; cin >> accNum;
                if (bank.applyInterestToAccount(customer, accNum))
                    cout << "Interest applied successfully.\n";
                else
                    cout << "Failed to apply interest.\n";
            }
            else if (choice == 7) {
                break;
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
            cout << "7. Logout\n";
            cout << "Enter choice: ";
            cin >> choice;
            BankFacade facade = bank.bankFacade;
            switch (choice) {
            case 1:
                facade.checkBalance(customer);
                break;
            case 2: {
                cout << "\nDEPOSIT FUNDS\n";
                cout << "Enter the account number to deposit into: ";
                string accNum;
                cin >> accNum;
                cout << "Enter deposit amount: $";
                double amt;
                cin >> amt;
                facade.performDeposit(customer, accNum, amt);
                break;
            }
            case 3: {
                cout << "\nWITHDRAW FUNDS\n";
                cout << "Enter the account number to withdraw from: ";
                string accNum;
                cin >> accNum;
                cout << "Enter withdrawal amount: $";
                double amt;
                cin >> amt;
                facade.performWithdrawal(customer, accNum, amt);
                break;
            }
            case 4: {
                cout << "\nTRANSFER FUNDS\n";
                cout << "Enter source account number: ";
                string sourceAcc;
                cin >> sourceAcc;
                cout << "Enter destination account number: ";
                string destAcc;
                cin >> destAcc;
                cout << "Enter transfer amount: $";
                double amt;
                cin >> amt;
                facade.performTransfer(customer, sourceAcc, destAcc, amt);
                break;
            }
            case 5: {
                cout << "TRANSACTION HISTORY\n";
                /*vector<Transaction*> history = customer->getTransactionHistory();
                for (auto txn : history) {
                    cout << txn->timestamp << " | " << txn->transactionID << " | $" << txn->amount << "\n";
                }*/
               /* Account* currentAccount = bank.getDatabaseManager()->getAccountByAccountNumber(customer->accounts[0]->accountNumber);
                if (!currentAccount) {
                    std::cout << "Unable to retrieve account details from database.\n";
                    break;
                }*/

                //int accountId = currentAccount->databaseId;
                int accountId = customer->accounts[0]->databaseId;
				//accountId = 2; //current do not have new transaction, so we need modify the id to eather 1 or 2

                // for debugging
                //cout << "DEBUG Account ID: " << accountId << endl;

                std::vector<TransactionRecord> records = bank.getDatabaseManager()->getTransactionsForAccount(accountId);

                std::cout << "\n=== Database Transaction History for Account "
                    << customer->accounts[0]->accountNumber << " ===\n";
				//formatting the output
                for (const auto& record : records) {
                    std::cout << "Transaction ID: " << record.transactionId
                        << " | Type: " << record.transactionType
                        << " | Amount: $" << record.amount
                        << " | Date: " << record.transactionDate;
                    if (record.recipientAccount != -1)
                        std::cout << " | Recipient Account: " << record.recipientAccount;
                    std::cout << std::endl;
                }
                if (records.empty()) {
                    std::cout << "No transactions found for this account.\n";
                }
                break;
            }
            case 7:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
            }
        }
    } while (choice != 7);
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
