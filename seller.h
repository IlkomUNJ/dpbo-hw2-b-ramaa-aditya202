#ifndef SELLER_H
#define SELLER_H
#include "buyer.h"
#include "item.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class seller : public Buyer {

private:
    int sellerId;
    std::string sellerName;
    std::string storeAddress;
    std::string storePhone;
    std::string storeEmail;
    
    bool idDisplayed(int itemId) const {
        return itemId > 0;
    }

    std::vector<Item> items;

public:
    seller() : Buyer(), sellerId(0), sellerName(""), storeAddress(""), storePhone(""), storeEmail("") {}

    seller(Buyer buyer, int sellerId, const std::string& sellerName, 
           const std::string& storeAddr, const std::string& storePhone, const std::string& storeEmail)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getBankAccountId()), 
          sellerId(sellerId), sellerName(sellerName), storeAddress(storeAddr),
          storePhone(storePhone), storeEmail(storeEmail) {
            setAddress(buyer.getAddress());
            setPhone(buyer.getPhone());
            setEmail(buyer.getEmail());
        }

    virtual ~seller() = default;

    int getSellerId() const { return sellerId; }
    std::string getSellerName() const { return sellerName; }
    std::string getStoreAddress() const { return storeAddress; }
    std::string getStorePhone() const { return storePhone; }
    std::string getStoreEmail() const { return storeEmail; }
    const std::vector<Item>& getItems() const { return items; }

    void addNewItem(int newId, const std::string& newName, int newQuantity, double newPrice) {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
    }

    void updateItem(int itemId, const std::string& newName, int newQuantity, double newPrice) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.alterItemById(itemId, newName, newQuantity, newPrice);
            }
        }
    }

    void removeItem(int itemId) {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].getId() == itemId) {
                items.erase(items.begin() + i);
                break;
            }
        }
    }

    void makeItemVisibleToCustomer(int itemId) {
        for (auto& item : items) {
            if (item.getId() == itemId) {
                item.setDisplay(true);
                break;
            }
        }
    }

    void displayInventory() const {
        std::cout << "=== Inventory for " << sellerName << " ===" << std::endl;
        for (const auto& item : items) {
            std::cout << "ID: " << item.getId() << " | Name: " << item.getName() 
                     << " | Qty: " << item.getQuantity() << " | Price: Rp" << item.getPrice() << std::endl;
        }
    }

    void serialize(std::ofstream& out) const {
        Buyer::serialize(out);
        out << sellerId << std::endl;
        out << sellerName << std::endl;
        out << storeAddress << std::endl;
        out << storePhone << std::endl;
        out << storeEmail << std::endl;
        out << items.size() << std::endl;
        for (const auto& item : items) {
            item.serialize(out);
        }
    }

    static seller deserialize(std::ifstream& in) {
        Buyer buyer = Buyer::deserialize(in);
        int sellerId;
        std::string sellerName, storeAddress, storePhone, storeEmail;
        in >> sellerId;
        in.ignore();
        getline(in, sellerName);
        getline(in, storeAddress);
        getline(in, storePhone);
        getline(in, storeEmail);
        
        seller s(buyer, sellerId, sellerName, storeAddress, storePhone, storeEmail);
        
        size_t itemCount;
        in >> itemCount;
        for (size_t i = 0; i < itemCount; i++) {
            Item item = Item::deserialize(in);
            s.items.push_back(item);
        }
        return s;
    }
};

#endif // SELLER_H