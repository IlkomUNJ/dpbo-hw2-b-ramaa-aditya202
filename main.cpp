#include <iostream>
#include <limits>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "store.h"
#include "order.h"

enum PrimaryPrompt{LOGIN, REGISTER, EXIT, ADMIN_LOGIN};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};
enum BuyerMenu{CHECK_ACCOUNT = 1, UPGRADE_TO_SELLER, BANKING_FUNCTIONS, BROWSE_STORE, ORDER_FUNC, PAYMENT_FUNC, LOGOUT, DELETE_ACCOUNT};
enum SellerMenu{CHECK_INVENTORY = 1, ADD_ITEM, REMOVE_ITEM, VIEW_ORDERS, SELLER_LOGOUT};
enum AdminMenu{ACCOUNT_MANAGEMENT = 1, SYSTEM_REPORT, ADMIN_LOGOUT};

using namespace std;

Store store;
int currentBuyerId = -1;
const string DATA_FILE = "store_data.txt";

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void buyerMenu() {
    while (true) {
        cout << "\n=== Buyer Menu ===" << endl;
        cout << "1. Check Account Status" << endl;
        cout << "2. Upgrade Account to Seller" << endl;
        cout << "3. Banking Functions" << endl;
        cout << "4. Browse Store" << endl;
        cout << "5. Order Functionality" << endl;
        cout << "6. Payment Functionality" << endl;
        cout << "7. Logout" << endl;
        cout << "8. Delete Account" << endl;
        cout << "Select option: ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            clearInput();
            cout << "Invalid input!" << endl;
            continue;
        }
        
        Buyer buyer = store.getBuyerById(currentBuyerId);
        if (buyer.getId() == 0) {
            cout << "Error: Buyer not found!" << endl;
            return;
        }
        
        switch (choice) {
            case CHECK_ACCOUNT: {
                cout << "\n=== Account Status ===" << endl;
                buyer.printInfo();
                cout << "\n--- Bank Account ---" << endl;
                BankCustomer account = store.getBank().getAccountById(buyer.getBankAccountId());
                account.printInfo();
                
                seller s = store.getSellerById(currentBuyerId);
                if (s.getId() != 0) {
                    cout << "\n--- Seller Status ---" << endl;
                    cout << "You are also a Seller!" << endl;
                    cout << "Store: " << s.getSellerName() << endl;
                }
                break;
            }
            
            case UPGRADE_TO_SELLER: {
                seller existingSeller = store.getSellerById(currentBuyerId);
                if (existingSeller.getId() != 0) {
                    cout << "You are already a seller!" << endl;
                    break;
                }
                
                cout << "\n=== Upgrade to Seller ===" << endl;
                string storeName, storeAddr, storePhone, storeEmail;
                
                cout << "Store Name: ";
                clearInput();
                getline(cin, storeName);
                cout << "Store Address: ";
                getline(cin, storeAddr);
                cout << "Store Phone: ";
                getline(cin, storePhone);
                cout << "Store Email: ";
                getline(cin, storeEmail);
                
                seller newSeller(buyer, currentBuyerId, storeName, storeAddr, storePhone, storeEmail);
                store.addSeller(newSeller);
                cout << "Successfully upgraded to Seller!" << endl;
                store.saveToFile(DATA_FILE);
                break;
            }
            
            case BANKING_FUNCTIONS: {
                while (true) {
                    cout << "\n=== Banking Functions ===" << endl;
                    cout << "1. Check Balance" << endl;
                    cout << "2. Deposit" << endl;
                    cout << "3. Withdraw" << endl;
                    cout << "4. Back" << endl;
                    cout << "Select option: ";
                    
                    int bankChoice;
                    cin >> bankChoice;
                    
                    if (cin.fail()) {
                        clearInput();
                        continue;
                    }
                    
                    if (bankChoice == 4) break;
                    
                    BankCustomer account = store.getBank().getAccountById(buyer.getBankAccountId());
                    
                    switch (bankChoice) {
                        case 1:
                            cout << "Current Balance: Rp" << account.getBalance() << endl;
                            break;
                        case 2: {
                            double amount;
                            cout << "Enter deposit amount: Rp";
                            cin >> amount;
                            if (amount > 0) {
                                account.addBalance(amount);
                                store.getBank().updateAccount(account);
                                cout << "Deposited Rp" << amount << endl;
                                store.saveToFile(DATA_FILE);
                            }
                            break;
                        }
                        case 3: {
                            double amount;
                            cout << "Enter withdrawal amount: Rp";
                            cin >> amount;
                            if (account.withdrawBalance(amount)) {
                                store.getBank().updateAccount(account);
                                cout << "Withdrawn Rp" << amount << endl;
                                store.saveToFile(DATA_FILE);
                            }
                            break;
                        }
                    }
                }
                break;
            }
            
            case BROWSE_STORE: {
                store.displayAllSellers();
                if (store.getSellers().empty()) {
                    cout << "No sellers available." << endl;
                    break;
                }
                
                cout << "\nEnter Seller ID to browse (0 to cancel): ";
                int sellerId;
                cin >> sellerId;
                if (sellerId == 0) break;
                
                seller s = store.getSellerById(sellerId);
                if (s.getId() == 0) {
                    cout << "Seller not found!" << endl;
                    break;
                }
                
                s.displayInventory();
                break;
            }
            
            case ORDER_FUNC: {
                Order newOrder(store.getNextOrderId(), currentBuyerId);
                
                while (true) {
                    cout << "\n=== Order Menu ===" << endl;
                    cout << "1. Add item to cart" << endl;
                    cout << "2. View cart" << endl;
                    cout << "3. Remove item from cart" << endl;
                    cout << "4. Checkout" << endl;
                    cout << "5. Cancel" << endl;
                    cout << "Select option: ";
                    
                    int orderChoice;
                    cin >> orderChoice;
                    
                    if (cin.fail()) {
                        clearInput();
                        continue;
                    }
                    
                    if (orderChoice == 5) break;
                    
                    switch (orderChoice) {
                        case 1: {
                            store.displayAllSellers();
                            cout << "Enter Seller ID: ";
                            int sellerId;
                            cin >> sellerId;
                            
                            seller s = store.getSellerById(sellerId);
                            if (s.getId() == 0) {
                                cout << "Seller not found!" << endl;
                                break;
                            }
                            
                            s.displayInventory();
                            cout << "Enter Item ID: ";
                            int itemId;
                            cin >> itemId;
                            
                            const vector<Item>& items = s.getItems();
                            bool found = false;
                            for (const auto& item : items) {
                                if (item.getId() == itemId) {
                                    cout << "Enter quantity: ";
                                    int qty;
                                    cin >> qty;
                                    
                                    if (qty > item.getQuantity()) {
                                        cout << "Not enough stock!" << endl;
                                    } else {
                                        OrderItem orderItem(item.getId(), item.getName(), qty, item.getPrice(), sellerId);
                                        newOrder.addItem(orderItem);
                                        cout << "Item added to cart!" << endl;
                                    }
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) cout << "Item not found!" << endl;
                            break;
                        }
                        case 2:
                            newOrder.printOrder();
                            break;
                        case 3: {
                            cout << "Enter Item ID to remove: ";
                            int itemId;
                            cin >> itemId;
                            newOrder.removeItem(itemId);
                            cout << "Item removed!" << endl;
                            break;
                        }
                        case 4:
                            if (newOrder.getItems().empty()) {
                                cout << "Cart is empty!" << endl;
                            } else {
                                store.addOrder(newOrder);
                                cout << "Order placed! Order ID: " << newOrder.getOrderId() << endl;
                                store.saveToFile(DATA_FILE);
                                return;
                            }
                            break;
                    }
                }
                break;
            }
            
            case PAYMENT_FUNC: {
                cout << "\n=== Your Orders ===" << endl;
                bool hasOrders = false;
                for (auto& order : store.getOrders()) {
                    if (order.getBuyerId() == currentBuyerId && order.getStatus() == "pending") {
                        order.printOrder();
                        cout << "---" << endl;
                        hasOrders = true;
                    }
                }
                
                if (!hasOrders) {
                    cout << "No pending orders." << endl;
                    break;
                }
                
                cout << "\nEnter Order ID to pay (0 to cancel): ";
                int orderId;
                cin >> orderId;
                if (orderId == 0) break;
                
                Order order = store.getOrderById(orderId);
                if (order.getOrderId() == 0 || order.getBuyerId() != currentBuyerId) {
                    cout << "Order not found!" << endl;
                    break;
                }
                
                BankCustomer buyerAccount = store.getBank().getAccountById(buyer.getBankAccountId());
                cout << "\nCurrent Balance: Rp" << buyerAccount.getBalance() << endl;
                cout << "Total Amount: Rp" << order.getTotalAmount() << endl;
                cout << "Confirm payment? Enter Order ID to confirm: ";
                int confirmId;
                cin >> confirmId;
                
                if (confirmId == orderId) {
                    if (buyerAccount.withdrawBalance(order.getTotalAmount())) {
                        store.getBank().updateAccount(buyerAccount);
                        order.setStatus("paid");
                        store.updateOrder(order);
                        
                        for (const auto& item : order.getItems()) {
                            seller s = store.getSellerById(item.sellerId);
                            if (s.getId() != 0) {
                                BankCustomer sellerAccount = store.getBank().getAccountById(s.getBankAccountId());
                                sellerAccount.addBalance(item.price * item.quantity);
                                store.getBank().updateAccount(sellerAccount);
                            }
                        }
                        
                        Transaction trans(store.getNextTransactionId(), currentBuyerId, 0, order.getTotalAmount());
                        store.addTransaction(trans);
                        
                        cout << "Payment successful!" << endl;
                        store.saveToFile(DATA_FILE);
                    }
                } else {
                    cout << "Payment cancelled." << endl;
                }
                break;
            }
            
            case LOGOUT: {
                cout << "Confirm logout? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    currentBuyerId = -1;
                    return;
                }
                break;
            }
            
            case DELETE_ACCOUNT: {
                cout << "WARNING: This will delete your account permanently!" << endl;
                cout << "Type 'DELETE' to confirm: ";
                string confirm;
                cin >> confirm;
                if (confirm == "DELETE") {
                    store.removeBuyerById(currentBuyerId);
                    store.removeSellerById(currentBuyerId);
                    store.getBank().removeAccount(currentBuyerId);
                    cout << "Account deleted." << endl;
                    store.saveToFile(DATA_FILE);
                    currentBuyerId = -1;
                    return;
                }
                break;
            }
        }
    }
}

void sellerMenu() {
    seller s = store.getSellerById(currentBuyerId);
    if (s.getId() == 0) {
        cout << "Error: You are not a seller!" << endl;
        return;
    }
    
    while (true) {
        cout << "\n=== Seller Menu ===" << endl;
        cout << "1. Check Inventory" << endl;
        cout << "2. Add Item to Inventory" << endl;
        cout << "3. Remove Item from Inventory" << endl;
        cout << "4. View Orders" << endl;
        cout << "5. Logout" << endl;
        cout << "Select option: ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            clearInput();
            continue;
        }
        
        s = store.getSellerById(currentBuyerId);
        
        switch (choice) {
            case CHECK_INVENTORY:
                s.displayInventory();
                break;
                
            case ADD_ITEM: {
                cout << "\n=== Add New Item ===" << endl;
                int id, qty;
                double price;
                string name;
                
                cout << "Item ID: ";
                cin >> id;
                cout << "Item Name: ";
                clearInput();
                getline(cin, name);
                cout << "Quantity: ";
                cin >> qty;
                cout << "Price: Rp";
                cin >> price;
                
                s.addNewItem(id, name, qty, price);
                s.makeItemVisibleToCustomer(id);
                store.updateSeller(s);
                store.saveToFile(DATA_FILE);
                cout << "Item added successfully!" << endl;
                break;
            }
            
            case REMOVE_ITEM: {
                s.displayInventory();
                cout << "Enter Item ID to remove: ";
                int itemId;
                cin >> itemId;
                s.removeItem(itemId);
                store.updateSeller(s);
                store.saveToFile(DATA_FILE);
                cout << "Item removed!" << endl;
                break;
            }
            
            case VIEW_ORDERS: {
                cout << "\n=== Orders for your store ===" << endl;
                bool hasOrders = false;
                for (auto& order : store.getOrders()) {
                    if (order.getStatus() == "paid") {
                        for (const auto& item : order.getItems()) {
                            if (item.sellerId == currentBuyerId) {
                                order.printOrder();
                                cout << "---" << endl;
                                hasOrders = true;
                                break;
                            }
                        }
                    }
                }
                if (!hasOrders) {
                    cout << "No paid orders." << endl;
                }
                break;
            }
            
            case SELLER_LOGOUT:
                return;
        }
    }
}

void adminMenu() {
    while (true) {
        cout << "\n=== Admin Menu ===" << endl;
        cout << "1. Account Management" << endl;
        cout << "2. System Report" << endl;
        cout << "3. Logout" << endl;
        cout << "Select option: ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            clearInput();
            continue;
        }
        
        switch (choice) {
            case ACCOUNT_MANAGEMENT: {
                while (true) {
                    cout << "\n=== Account Management ===" << endl;
                    cout << "1. View All Buyers" << endl;
                    cout << "2. View All Sellers" << endl;
                    cout << "3. View Buyer Details" << endl;
                    cout << "4. View Seller Details" << endl;
                    cout << "5. Search Account" << endl;
                    cout << "6. Remove Buyer" << endl;
                    cout << "7. Remove Seller" << endl;
                    cout << "8. Back" << endl;
                    cout << "Select option: ";
                    
                    int mgmtChoice;
                    cin >> mgmtChoice;
                    
                    if (cin.fail()) {
                        clearInput();
                        continue;
                    }
                    
                    if (mgmtChoice == 8) break;
                    
                    switch (mgmtChoice) {
                        case 1:
                            store.displayAllBuyers();
                            break;
                        case 2:
                            store.displayAllSellers();
                            break;
                        case 3: {
                            cout << "Enter Buyer ID: ";
                            int id;
                            cin >> id;
                            Buyer b = store.getBuyerById(id);
                            if (b.getId() != 0) {
                                b.printInfo();
                                BankCustomer account = store.getBank().getAccountById(b.getBankAccountId());
                                account.printInfo();
                            } else {
                                cout << "Buyer not found!" << endl;
                            }
                            break;
                        }
                        case 4: {
                            cout << "Enter Seller ID: ";
                            int id;
                            cin >> id;
                            seller s = store.getSellerById(id);
                            if (s.getId() != 0) {
                                cout << "Seller Name: " << s.getSellerName() << endl;
                                cout << "Store Address: " << s.getStoreAddress() << endl;
                                cout << "Store Phone: " << s.getStorePhone() << endl;
                                cout << "Store Email: " << s.getStoreEmail() << endl;
                            } else {
                                cout << "Seller not found!" << endl;
                            }
                            break;
                        }
                        case 5: {
                            cout << "Search by: 1.Name 2.ID : ";
                            int searchType;
                            cin >> searchType;
                            
                            if (searchType == 1) {
                                string name;
                                cout << "Enter name: ";
                                clearInput();
                                getline(cin, name);
                                
                                for (auto& b : store.getBuyers()) {
                                    if (b.getName().find(name) != string::npos) {
                                        b.printInfo();
                                    }
                                }
                            } else {
                                int id;
                                cout << "Enter ID: ";
                                cin >> id;
                                Buyer b = store.getBuyerById(id);
                                if (b.getId() != 0) b.printInfo();
                            }
                            break;
                        }
                        case 6: {
                            cout << "Enter Buyer ID to remove: ";
                            int id;
                            cin >> id;
                            if (store.removeBuyerById(id)) {
                                store.getBank().removeAccount(id);
                                cout << "Buyer removed!" << endl;
                            } else {
                                cout << "Buyer not found!" << endl;
                            }
                            break;
                        }
                        case 7: {
                            cout << "Enter Seller ID to remove: ";
                            int id;
                            cin >> id;
                            if (store.removeSellerById(id)) {
                                cout << "Seller removed!" << endl;
                            } else {
                                cout << "Seller not found!" << endl;
                            }
                            break;
                        }
                    }
                }
                break;
            }
            
            case SYSTEM_REPORT: {
                cout << "\n=== System Report ===" << endl;
                cout << "Total Buyers: " << store.getBuyers().size() << endl;
                cout << "Total Sellers: " << store.getSellers().size() << endl;
                cout << "Total Bank Accounts: " << store.getBank().getCustomerCount() << endl;
                cout << "Total Orders: " << store.getOrders().size() << endl;
                cout << "Total Transactions: " << store.getTransactions().size() << endl;
                break;
            }
            
            case ADMIN_LOGOUT:
                return;
        }
    }
}

int main() {
    cout << "=== Loading saved data ===" << endl;
    store.loadFromFile(DATA_FILE);
    
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;

    while (prompt != EXIT) {
        cout << "\n=== Main Menu ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "4. Admin Login" << endl;
        cout << "Select option: ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            clearInput();
            cout << "Invalid input!" << endl;
            continue;
        }
        
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        
        switch (prompt) {
            case LOGIN: {
                cout << "\n=== Login ===" << endl;
                string loginName, loginPhone;
                cout << "Enter Username: ";
                clearInput();
                getline(cin, loginName);
                cout << "Enter Phone Number: ";
                getline(cin, loginPhone);
                
                Buyer buyer;
                bool found = false;
                for (auto& b : store.getBuyers()) {
                    if (b.getName() == loginName && b.getPhone() == loginPhone) {
                        buyer = b;
                        found = true;
                        break;
                    }
                }
                
                if (found) {
                    currentBuyerId = buyer.getId();
                    cout << "Welcome, " << buyer.getName() << "!" << endl;
                    
                    seller s = store.getSellerById(currentBuyerId);
                    if (s.getId() != 0) {
                        cout << "Login as: 1.Buyer 2.Seller : ";
                        int loginType;
                        cin >> loginType;
                        if (loginType == 2) {
                            sellerMenu();
                        } else {
                            buyerMenu();
                        }
                    } else {
                        buyerMenu();
                    }
                } else {
                    cout << "Invalid username or phone number!" << endl;
                }
                break;
            }
            
            case REGISTER: {
                regPrompt = CREATE_BUYER;
                while (regPrompt != BACK) {
                    cout << "\n=== Register ===" << endl;
                    cout << "1. Create Buyer Account" << endl;
                    cout << "2. Create Seller Account" << endl;
                    cout << "3. Back" << endl;
                    cout << "Select option: ";
                    
                    int regChoice;
                    cin >> regChoice;
                    
                    if (cin.fail()) {
                        clearInput();
                        continue;
                    }
                    
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);
                    
                    switch (regPrompt) {
                        case CREATE_BUYER: {
                            cout << "\n=== Create Buyer Account ===" << endl;
                            string name, address, phone, email;
                            double initialDeposit;
                            
                            cout << "Name: ";
                            clearInput();
                            getline(cin, name);
                            cout << "Address: ";
                            getline(cin, address);
                            cout << "Phone: ";
                            getline(cin, phone);
                            cout << "Email: ";
                            getline(cin, email);
                            cout << "Initial Deposit: Rp";
                            cin >> initialDeposit;
                            
                            int newId = store.getNextBuyerId();
                            BankCustomer bankAcc(newId, name, initialDeposit);
                            store.getBank().addAccount(bankAcc);
                            
                            Buyer newBuyer(newId, name, address, phone, email, newId);
                            store.addBuyer(newBuyer);
                            
                            cout << "Account created! Your Buyer ID: " << newId << endl;
                            
                            cout << "Upgrade to Seller? (y/n): ";
                            char upgrade;
                            cin >> upgrade;
                            if (upgrade == 'y' || upgrade == 'Y') {
                                regPrompt = CREATE_SELLER;
                                currentBuyerId = newId;
                                continue;
                            } else {
                                currentBuyerId = newId;
                                buyerMenu();
                                regPrompt = BACK;
                            }
                            break;
                        }
                        
                        case CREATE_SELLER: {
                            if (currentBuyerId == -1) {
                                cout << "Please create a buyer account first!" << endl;
                                break;
                            }
                            
                            Buyer buyer = store.getBuyerById(currentBuyerId);
                            if (buyer.getId() == 0) {
                                cout << "Buyer not found!" << endl;
                                break;
                            }
                            
                            cout << "\n=== Create Seller Account ===" << endl;
                            string storeName, storeAddr, storePhone, storeEmail;
                            
                            cout << "Store Name: ";
                            clearInput();
                            getline(cin, storeName);
                            cout << "Store Address: ";
                            getline(cin, storeAddr);
                            cout << "Store Phone: ";
                            getline(cin, storePhone);
                            cout << "Store Email: ";
                            getline(cin, storeEmail);
                            
                            seller newSeller(buyer, currentBuyerId, storeName, storeAddr, storePhone, storeEmail);
                            store.addSeller(newSeller);
                            
                            cout << "Seller account created!" << endl;
                            cout << "Login as: 1.Buyer 2.Seller : ";
                            int loginType;
                            cin >> loginType;
                            if (loginType == 2) {
                                sellerMenu();
                            } else {
                                buyerMenu();
                            }
                            regPrompt = BACK;
                            break;
                        }
                        
                        case BACK:
                            cout << "Returning to main menu..." << endl;
                            break;
                            
                        default:
                            cout << "Invalid option." << endl;
                            break;
                    }
                }
                break;
            }
            
            case EXIT:
                cout << "\n=== Saving data ===" << endl;
                store.saveToFile(DATA_FILE);
                cout << "Thank you for using our system!" << endl;
                break;
                
            case ADMIN_LOGIN: {
                cout << "\n=== Admin Login ===" << endl;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                
                if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
                    cout << "Admin login successful!" << endl;
                    adminMenu();
                } else {
                    cout << "Invalid credentials!" << endl;
                }
                break;
            }
            
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }

    return 0;
}