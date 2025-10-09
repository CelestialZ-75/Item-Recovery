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
}

void Contacts::addInfo(string _name, string _num) {
    name = _name;
    num = _num;
}

void Item::printItem() {
    if (name != "NaN")
        cout << "Item Name:" << name << endl;
    else
        unvalidPrint("Item Name");

    if (description != "NaD")
        cout << "Description: " << description << endl;
    else
        unvalidPrint("Description");

    if (con)
        con->printContact();
    else
        unvalidPrint("Contact Name or Num");
}

void Item::addInfo(string _name, string _de) {
        name = _name;
        description = _de;
}