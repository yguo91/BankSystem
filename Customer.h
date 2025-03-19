#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "Account.h"
#include "Transaction.h"
#include "CustomerNotification.h"

enum class Role { Personal, Business, AccountManager, Admin };

class Customer {
public:
    std::string customerID;
    std::string name;
    std::string email;
    std::string phone;
    Role role;
    std::vector<Account*> accounts;
    CustomerNotification notifier;

    Customer(const std::string& id, const std::string& n, const std::string& email, const std::string& phone, Role r = Role::Personal);
    void addAccount(Account* account);
    void removeAccount(const std::string& accountNumber);
    std::vector<Transaction*> getTransactionHistory();
    void receiveNotification(const std::string& message);
};

#endif // CUSTOMER_H
