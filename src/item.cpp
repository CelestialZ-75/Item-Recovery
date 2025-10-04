#include "item.h"

void Contacts::printContact() {
    cout << "Contact Name: " << name << endl;
    cout << "Contact Num: " << num << endl;
}

void Item::printItem() {
    cout << "Item Name:" << name << endl;
    cout << "Description: " << description << endl;
    contacts.printContact();
}