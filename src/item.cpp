#include "item.h"

void unvalidPrint(string _type) { cout << "No Valid " << _type << endl; }

void Contacts::printContact() {
    if (name != "NaN")
        cout << "Contact Name: " << name << endl;
    else
        unvalidPrint("Contact Name");

    if (num != "NaN")
        cout << "Contact Num: " << num << endl;
    else
        unvalidPrint("Contact Num");

    if (email != "NaN")
        cout << "Contact Email: " << email << endl;
    else
        unvalidPrint("Contact Email");
}

void Contacts::addInfo(string _name, string _num, string _email) {
    name = _name;
    num = _num;
    email = _email;
}

void Item::printItem() {
    if (name != "NaN")
        cout << "Item Name: " << name << endl;
    else
        unvalidPrint("Item Name");

    if (description != "NaD")
        cout << "Description: " << description << endl;
    else
        unvalidPrint("Description");

    if (address != "NaN")
        cout << "Address: " << address << endl;
    else
        unvalidPrint("Address");

    // 显示类别
    string catStr;
    switch(category) {
        case ItemCategory::FOOD: catStr = "食品"; break;
        case ItemCategory::BOOK: catStr = "书籍"; break;
        case ItemCategory::TOOL: catStr = "工具"; break;
        default: catStr = "其他"; break;
    }
    cout << "Category: " << catStr << endl;

    if (con.getName() != "NaN")
        con.printContact();
    else
        unvalidPrint("Contact Information");
}

void Item::addInfo(string _name, string _de, string _addr) {
    name = _name;
    description = _de;
    address = _addr;
}

void FoodItem::printItem() {
    Item::printItem();
    if (expiryDate != "NaN")
        cout << "Expiry Date: " << expiryDate << endl;
    else
        unvalidPrint("Expiry Date");
    
    if (quantity > 0)
        cout << "Quantity: " << quantity << endl;
    else
        unvalidPrint("Quantity");
}

void BookItem::printItem() {
    Item::printItem();
    if (author != "NaN")
        cout << "Author: " << author << endl;
    else
        unvalidPrint("Author");
    
    if (publisher != "NaN")
        cout << "Publisher: " << publisher << endl;
    else
        unvalidPrint("Publisher");
}

void ToolItem::printItem() {
    Item::printItem();
    if (condition != "NaN")
        cout << "Condition: " << condition << endl;
    else
        unvalidPrint("Condition");
}