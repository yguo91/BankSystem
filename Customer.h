#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "Account.h"

class Customer {
private:
    std::string customerID;
    std::string name;
    std::string email;
    std::string phone;
    std::vector<Account*> accounts;

public:
    Customer(std::string id, std::string name, std::string email, std::string phone);

    std::string getCustomerID() const;
    std::string getName() const;
    void addAccount(Account* account);
    void removeAccount(const std::string& accountNumber);
    void listAccounts() const;
};

#endif // CUSTOMER_H

