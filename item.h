#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <fstream>

using namespace std;

class Item {
private:
    int id;
    std::string name;
    int quantity;
    double price;
    bool idDisplay;

public:
    Item(int id, const std::string& name, int quantity, double price)
        : id(id), name(name), quantity(quantity), price(price) {
            idDisplay = false;
        }

    int getId() const { return id; }
    const std::string& getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    Item *getItem() {
        return this;
    }

    void setId(int newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setQuantity(int newQuantity) { quantity = newQuantity; }
    void setPrice(double newPrice) { price = newPrice; }
    void setDisplay(bool display) { idDisplay = display; }

    void alterItemById(int itemId, const std::string& newName, int newQuantity, double newPrice) {
        if (id == itemId) {
            name = newName;
            quantity = newQuantity;
            price = newPrice;
        }
    }

    void updatePriceQuantity(int itemId, double newPrice, int newQuantity) {
        if (id == itemId) {
            price = newPrice;
            quantity = newQuantity;
        }
    }

    void serialize(ofstream& out) const {
        out << id << endl;
        out << name << endl;
        out << quantity << endl;
        out << price << endl;
        out << idDisplay << endl;
    }

    static Item deserialize(ifstream& in) {
        int id, quantity;
        string name;
        double price;
        bool idDisplay;
        in >> id;
        in.ignore();
        getline(in, name);
        in >> quantity >> price >> idDisplay;
        Item item(id, name, quantity, price);
        item.setDisplay(idDisplay);
        return item;
    }
};

#endif // ITEM_H