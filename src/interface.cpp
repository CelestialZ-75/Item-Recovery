#include <iostream>
#include <string>

#include "interface.h"

bool isUnvalidN(string n) { return n == "NaN" || n.empty(); }

bool isUnvalidD(string d) { return d == "NaD" || d.empty(); }

void Interface::helloCommand() {
    cout << "Welcome, enter 1 for new item, 2 for item delete, 3 for item list show, 4 "
            "for item search, e for exit"
         << endl;
}

ItemCategory Interface::selectCategory() {
    int cat;
    cout << "Select item category: 1-Food(食品), 2-Book(书籍), 3-Tool(工具), 4-Other(其他): ";
    cin >> cat;
    cin.ignore();  // 清除输入缓冲区中的换行符
    
    switch(cat) {
        case 1: return ItemCategory::FOOD;
        case 2: return ItemCategory::BOOK;
        case 3: return ItemCategory::TOOL;
        default: return ItemCategory::OTHER;
    }
}

void Interface::entrySelect(char input) {
    string name, de, addr, c_name, c_num, c_email;
    switch (input) {
        case '1': {
            // 输入公共信息
            cout << "Enter new Item Name: ";
            cin.ignore();  // 清除输入缓冲区
            getline(cin, name);
            cout << "Enter new Item Description: ";
            getline(cin, de);
            cout << "Enter Item Address: ";
            getline(cin, addr);
            cout << "Enter Contact Name: ";
            getline(cin, c_name);
            cout << "Enter Contact Phone: ";
            getline(cin, c_num);
            cout << "Enter Contact Email: ";
            getline(cin, c_email);

            if (!isUnvalidN(name) && !isUnvalidD(de)) {
                if (!isUnvalidN(c_name) && !isUnvalidN(c_num) && !isUnvalidN(c_email)) {
                    con_v.push_back(Contacts(c_name, c_num, c_email));
                    
                    // 选择类别并输入特定属性
                    ItemCategory cat = selectCategory();
                    
                    if (cat == ItemCategory::FOOD) {
                        string expiry;
                        int qty;
                        cout << "Enter Expiry Date: ";
                        getline(cin, expiry);
                        cout << "Enter Quantity: ";
                        cin >> qty;
                        cin.ignore();  // 清除输入缓冲区
                        inv.addItem(new FoodItem(name, de, addr, con_v.back(), expiry, qty));
                    }
                    else if (cat == ItemCategory::BOOK) {
                        string author, publisher;
                        cout << "Enter Author: ";
                        getline(cin, author);
                        cout << "Enter Publisher: ";
                        getline(cin, publisher);
                        inv.addItem(new BookItem(name, de, addr, con_v.back(), author, publisher));
                    }
                    else if (cat == ItemCategory::TOOL) {
                        string condition;
                        cout << "Enter Condition: ";
                        getline(cin, condition);
                        inv.addItem(new ToolItem(name, de, addr, con_v.back(), condition));
                    }
                    else {
                        inv.addItem(new OtherItem(name, de, addr, con_v.back()));
                    }
                    
                    cout << "Item added successfully!" << endl;
                } else {
                    cout << "Invalid Contact Information, please try again." << endl;
                }
            } else {
                cout << "Invalid Item Name or Description, please try again." << endl;
            }
            break;
        }

        case '2':
            cout << "Enter Target Item Name: ";
            cin.ignore();
            getline(cin, name);
            if (inv.deleteItem(name))
                cout << "Delete Successful" << endl;
            else
                cout << "Delete Unsuccessful" << endl;
            break;

        case '3':
            if (inv.getSize() == 0) {
                cout << "Inventory is empty." << endl;
            } else {
                inv.printInv();
            }
            break;

        case '4':
            cout << "Enter Target Item Name: ";
            cin.ignore();
            getline(cin, name);
            unsigned int index;
            if ((index = inv.findItem(name)) != inv.getSize()) {
                inv.printItem(index);
            }
            else {
                cout << "Target not Found" << endl;
            }
            break;

        default:
            break;
    }
}
