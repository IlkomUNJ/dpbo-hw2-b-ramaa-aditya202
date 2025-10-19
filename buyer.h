#ifndef BUYER_H
#define BUYER_H

#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Buyer {
private:
    int id;
    string name;
    string address;
    string phone;
    string email;
    int bankAccountId;

public:
    Buyer() : id(0), name(""), address(""), phone(""), email(""), bankAccountId(0) {}

    Buyer(int id, const string& name, int accountId)
        : id(id), name(name), address(""), phone(""), email(""), bankAccountId(accountId) {}

    Buyer(int id, const string& name, const string& addr, const string& ph, const string& em, int accountId)
        : id(id), name(name), address(addr), phone(ph), email(em), bankAccountId(accountId) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    int getBankAccountId() const { return bankAccountId; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setAddress(const string& addr) { address = addr; }
    void setPhone(const string& ph) { phone = ph; }
    void setEmail(const string& em) { email = em; }

    void printInfo() const {
        cout << "Buyer ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
        cout << "Email: " << email << endl;
    }

    void serialize(ofstream& out) const {
        out << id << endl;
        out << name << endl;
        out << address << endl;
        out << phone << endl;
        out << email << endl;
        out << bankAccountId << endl;
    }

    static Buyer deserialize(ifstream& in) {
        int id, bankAccountId;
        string name, address, phone, email;
        in >> id;
        in.ignore();
        getline(in, name);
        getline(in, address);
        getline(in, phone);
        getline(in, email);
        in >> bankAccountId;
        return Buyer(id, name, address, phone, email, bankAccountId);
    }
};

#endif // BUYER_H