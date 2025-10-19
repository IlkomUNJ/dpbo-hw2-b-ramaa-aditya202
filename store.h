#ifndef STORE_H
#define STORE_H

#include "buyer.h"
#include "seller.h"
#include "order.h"
#include "transaction.h"
#include "bank.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Store {
private:
    vector<Buyer> buyers;
    vector<seller> sellers;
    vector<Order> orders;
    vector<Transaction> transactions;
    Bank bank;
    int nextBuyerId;
    int nextSellerId;
    int nextOrderId;
    int nextTransactionId;

public:
    Store() : bank("Main Bank"), nextBuyerId(1), nextSellerId(1), nextOrderId(1), nextTransactionId(1) {}

    void addBuyer(const Buyer& buyer) {
        buyers.push_back(buyer);
    }

    void addSeller(const seller& s) {
        sellers.push_back(s);
    }

    void addOrder(const Order& order) {
        orders.push_back(order);
    }

    void addTransaction(const Transaction& trans) {
        transactions.push_back(trans);
    }

    Buyer getBuyerById(int id) {
        for (size_t i = 0; i < buyers.size(); i++) {
            if (buyers[i].getId() == id) {
                return buyers[i];
            }
        }
        return Buyer();
    }

    seller getSellerById(int id) {
        for (size_t i = 0; i < sellers.size(); i++) {
            if (sellers[i].getId() == id) {
                return sellers[i];
            }
        }
        return seller();
    }

    Order getOrderById(int id) {
        for (size_t i = 0; i < orders.size(); i++) {
            if (orders[i].getOrderId() == id) {
                return orders[i];
            }
        }
        return Order(0, 0);
    }

    bool updateBuyer(const Buyer& buyer) {
        for (size_t i = 0; i < buyers.size(); i++) {
            if (buyers[i].getId() == buyer.getId()) {
                buyers[i] = buyer;
                return true;
            }
        }
        return false;
    }

    bool updateSeller(const seller& s) {
        for (size_t i = 0; i < sellers.size(); i++) {
            if (sellers[i].getId() == s.getId()) {
                sellers[i] = s;
                return true;
            }
        }
        return false;
    }

    bool updateOrder(const Order& order) {
        for (size_t i = 0; i < orders.size(); i++) {
            if (orders[i].getOrderId() == order.getOrderId()) {
                orders[i] = order;
                return true;
            }
        }
        return false;
    }

    int getNextBuyerId() { return nextBuyerId++; }
    int getNextSellerId() { return nextSellerId++; }
    int getNextOrderId() { return nextOrderId++; }
    int getNextTransactionId() { return nextTransactionId++; }

    Bank& getBank() { return bank; }
    vector<Buyer>& getBuyers() { return buyers; }
    vector<seller>& getSellers() { return sellers; }
    vector<Order>& getOrders() { return orders; }
    vector<Transaction>& getTransactions() { return transactions; }

    void displayAllSellers() {
        cout << "\n=== All Sellers ===" << endl;
        for (const auto& s : sellers) {
            cout << "Seller ID: " << s.getSellerId() << " | Name: " << s.getSellerName() << endl;
        }
    }

    void displayAllBuyers() {
        cout << "\n=== All Buyers ===" << endl;
        for (const auto& b : buyers) {
            cout << "Buyer ID: " << b.getId() << " | Name: " << b.getName() << endl;
        }
    }

    bool removeBuyerById(int id) {
        for (size_t i = 0; i < buyers.size(); i++) {
            if (buyers[i].getId() == id) {
                buyers.erase(buyers.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool removeSellerById(int id) {
        for (size_t i = 0; i < sellers.size(); i++) {
            if (sellers[i].getSellerId() == id) {
                sellers.erase(sellers.begin() + i);
                return true;
            }
        }
        return false;
    }

    void saveToFile(const string& filename) {
        ofstream out(filename);
        if (!out) {
            cout << "Error: Cannot open file for writing!" << endl;
            return;
        }

        out << nextBuyerId << endl;
        out << nextSellerId << endl;
        out << nextOrderId << endl;
        out << nextTransactionId << endl;

        bank.serialize(out);

        out << buyers.size() << endl;
        for (const auto& buyer : buyers) {
            buyer.serialize(out);
        }

        out << sellers.size() << endl;
        for (const auto& s : sellers) {
            s.serialize(out);
        }

        out << orders.size() << endl;
        for (const auto& order : orders) {
            order.serialize(out);
        }

        out << transactions.size() << endl;
        for (const auto& trans : transactions) {
            trans.serialize(out);
        }

        out.close();
        cout << "Data saved successfully!" << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) {
            cout << "No saved data found. Starting fresh." << endl;
            return;
        }

        in >> nextBuyerId >> nextSellerId >> nextOrderId >> nextTransactionId;
        in.ignore();

        bank.deserialize(in);

        size_t buyerCount;
        in >> buyerCount;
        buyers.clear();
        for (size_t i = 0; i < buyerCount; i++) {
            Buyer buyer = Buyer::deserialize(in);
            buyers.push_back(buyer);
        }

        size_t sellerCount;
        in >> sellerCount;
        sellers.clear();
        for (size_t i = 0; i < sellerCount; i++) {
            seller s = seller::deserialize(in);
            sellers.push_back(s);
        }

        size_t orderCount;
        in >> orderCount;
        orders.clear();
        for (size_t i = 0; i < orderCount; i++) {
            Order order = Order::deserialize(in);
            orders.push_back(order);
        }

        size_t transCount;
        in >> transCount;
        transactions.clear();
        for (size_t i = 0; i < transCount; i++) {
            Transaction trans = Transaction::deserialize(in);
            transactions.push_back(trans);
        }

        in.close();
        cout << "Data loaded successfully!" << endl;
    }
};

#endif // STORE_H
