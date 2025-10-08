#include "item.h"

void unvalidPrint(string _type) { cout << "No Valid" << _type << endl; }

void Contacts::printContact() {
    if (name != "NaN")
        cout << "Contact Name: " << name << endl;
    else
        unvalidPrint("Contact Name");

    if (num != "NaN")
        cout << "Contact Num: " << num << endl;
    else
        unvalidPrint("Contact Name");
}

void Item::printItem() {
    if (name != "NaN")
        cout << "Item Name:" << name << endl;
    else
        unvalidPrint("Item Name");

    if (name != "NaN")
        cout << "Description: " << description << endl;
    else
        unvalidPrint("Description");

    contacts.printContact();
}