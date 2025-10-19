#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "item.h"

using namespace std;

class OrderItem {
public:
    int itemId;
    string itemName;
    int quantity;
    double price;
    int sellerId;

    OrderItem(int id, const string& name, int qty, double pr, int sId)
        : itemId(id), itemName(name), quantity(qty), price(pr), sellerId(sId) {}

    void serialize(ofstream& out) const {
        out << itemId << endl;
        out << itemName << endl;
        out << quantity << endl;
        out << price << endl;
        out << sellerId << endl;
    }

    static OrderItem deserialize(ifstream& in) {
        int itemId, quantity, sellerId;
        string itemName;
        double price;
        in >> itemId;
        in.ignore();
        getline(in, itemName);
        in >> quantity >> price >> sellerId;
        return OrderItem(itemId, itemName, quantity, price, sellerId);
    }
};

class Order {
private:
    int orderId;
    int buyerId;
    vector<OrderItem> items;
    double totalAmount;
    string status;

public:
    Order(int orderId, int buyerId)
        : orderId(orderId), buyerId(buyerId), totalAmount(0.0), status("pending") {}

    void addItem(const OrderItem& item) {
        items.push_back(item);
        totalAmount += item.price * item.quantity;
    }

    void removeItem(int itemId) {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].itemId == itemId) {
                totalAmount -= items[i].price * items[i].quantity;
                items.erase(items.begin() + i);
                break;
            }
        }
    }

    int getOrderId() const { return orderId; }
    int getBuyerId() const { return buyerId; }
    double getTotalAmount() const { return totalAmount; }
    string getStatus() const { return status; }
    const vector<OrderItem>& getItems() const { return items; }

    void setStatus(const string& newStatus) { status = newStatus; }

    void printOrder() const {
        cout << "Order ID: " << orderId << endl;
        cout << "Buyer ID: " << buyerId << endl;
        cout << "Status: " << status << endl;
        cout << "Items:" << endl;
        for (const auto& item : items) {
            cout << "  - " << item.itemName << " (x" << item.quantity << ") - Rp" 
                 << item.price << " each" << endl;
        }
        cout << "Total Amount: Rp" << totalAmount << endl;
    }

    void serialize(ofstream& out) const {
        out << orderId << endl;
        out << buyerId << endl;
        out << totalAmount << endl;
        out << status << endl;
        out << items.size() << endl;
        for (const auto& item : items) {
            item.serialize(out);
        }
    }

    static Order deserialize(ifstream& in) {
        int orderId, buyerId;
        double totalAmount;
        string status;
        in >> orderId >> buyerId >> totalAmount;
        in.ignore();
        getline(in, status);
        
        Order order(orderId, buyerId);
        order.totalAmount = totalAmount;
        order.status = status;
        
        size_t itemCount;
        in >> itemCount;
        for (size_t i = 0; i < itemCount; i++) {
            OrderItem item = OrderItem::deserialize(in);
            order.items.push_back(item);
        }
        return order;
    }
};

#endif // ORDER_H
