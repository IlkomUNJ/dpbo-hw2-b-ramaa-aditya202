#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

class Transaction {
private:
    int transactionId;
    int buyerId;
    int sellerId;
    double amount;
    time_t date;
    string status; 

public:
    Transaction(int transactionId, int buyerId, int sellerId, double amount)
        : transactionId(transactionId), buyerId(buyerId), sellerId(sellerId), amount(amount) {
        date = time(0);
        status = "paid";
    }

    int getTransactionId() const { return transactionId; }
    int getBuyerId() const { return buyerId; }
    int getSellerId() const { return sellerId; }
    double getAmount() const { return amount; }
    time_t getDate() const { return date; }
    string getStatus() const { return status; }

    void setStatus(const string& newStatus) { status = newStatus; }

    void printInfo() const {
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Buyer ID: " << buyerId << endl;
        cout << "Seller ID: " << sellerId << endl;
        cout << "Amount: Rp" << amount << endl;
        cout << "Date: " << ctime(&date);
        cout << "Status: " << status << endl;
    }

    void serialize(ofstream& out) const {
        out << transactionId << endl;
        out << buyerId << endl;
        out << sellerId << endl;
        out << amount << endl;
        out << date << endl;
        out << status << endl;
    }

    static Transaction deserialize(ifstream& in) {
        int transactionId, buyerId, sellerId;
        double amount;
        time_t date;
        string status;
        in >> transactionId >> buyerId >> sellerId >> amount >> date;
        in.ignore();
        getline(in, status);
        
        Transaction trans(transactionId, buyerId, sellerId, amount);
        trans.date = date;
        trans.status = status;
        return trans;
    }
};

#endif // TRANSACTION_H
