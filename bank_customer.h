#ifndef BANK_CUSTOMER_H
#define BANK_CUSTOMER_H

#include <string>
#include <fstream>

using namespace std;

class BankCustomer {
private:
    int id;
    string name;
    double balance;

public:
    BankCustomer(int id, const string& name, double balance) : id(id), name(name), balance(balance) {
        this->id = id;
        this->name = name;
        this->balance = balance;
    }

    int getId() const;
    string getName() const;
    double getBalance() const;

    void printInfo() const;
    void setName(const string& name);
    void setBalance(double balance);
    void addBalance(double amout);
    bool withdrawBalance(double amout);

    void serialize(ofstream& out) const {
        out << id << endl;
        out << name << endl;
        out << balance << endl;
    }

    static BankCustomer deserialize(ifstream& in) {
        int id;
        string name;
        double balance;
        in >> id;
        in.ignore();
        getline(in, name);
        in >> balance;
        return BankCustomer(id, name, balance);
    }
};

#endif // BANK_CUSTOMER_H