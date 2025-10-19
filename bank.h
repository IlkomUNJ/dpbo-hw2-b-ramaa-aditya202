#ifndef BANK_H
#define BANK_H

#include "bank_customer.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Bank{
private:
    string name;
    vector<BankCustomer> Accounts;
    int customerCount;

public:
    Bank(const string& name){
        this->name = name;
        this->customerCount = 0;
    }

    void addAccount(const BankCustomer& account) {
        Accounts.push_back(account);
        customerCount++;
    }

    BankCustomer getAccountById(int customerId) {
        for (size_t i = 0; i < Accounts.size(); i++) {
            if (Accounts[i].getId() == customerId) {
                return Accounts[i];
            }
        }
        return BankCustomer(0, "", 0.0);
    }

    bool updateAccount(const BankCustomer& account) {
        for (size_t i = 0; i < Accounts.size(); i++) {
            if (Accounts[i].getId() == account.getId()) {
                Accounts[i] = account;
                return true;
            }
        }
        return false;
    }

    bool removeAccount(int customerId) {
        for (size_t i = 0; i < Accounts.size(); i++) {
            if (Accounts[i].getId() == customerId) {
                Accounts.erase(Accounts.begin() + i);
                customerCount--;
                return true;
            }
        }
        return false;
    }

    int getCustomerCount() const { return customerCount; }

    void displayAllAccounts() const {
        cout << "=== Bank: " << name << " ===" << endl;
        cout << "Total Customers: " << customerCount << endl;
        for (const auto& account : Accounts) {
            account.printInfo();
            cout << "---" << endl;
        }
    }

    void serialize(ofstream& out) const {
        out << name << endl;
        out << customerCount << endl;
        out << Accounts.size() << endl;
        for (const auto& account : Accounts) {
            account.serialize(out);
        }
    }

    void deserialize(ifstream& in) {
        getline(in, name);
        in >> customerCount;
        size_t accountCount;
        in >> accountCount;
        Accounts.clear();
        for (size_t i = 0; i < accountCount; i++) {
            BankCustomer account = BankCustomer::deserialize(in);
            Accounts.push_back(account);
        }
    }
};

#endif // BANK_H