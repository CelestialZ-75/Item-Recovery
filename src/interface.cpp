#include <iostream>

#include "interface.h"

bool isUnvalidN(string n) { return n == "NaN"; }

bool isUnvalidD(string d) { return d == "NaD"; }

void Interface::helloCommand() {
    cout << "Welcome, enter 1 for new item, 2 for item delete, 3 for item list show, 4 "
            "for item search, e for exit"
         << endl;
}

void Interface::entrySelect(char input) {
    string name, de, c_name, c_num;
    while (input != 'e') {
        switch (input) {
            case '1':
                printf("Enter new Item Name: ");
                cin >> name;
                printf("Enter new Item Description: ");
                cin >> de;
                printf("Enter Item Contact Name: ");
                cin >> c_name;
                printf("Enter Item Contact Num: ");
                cin >> c_num;

                if (!isUnvalidN(name) && !isUnvalidD(de)) {
                    if (!isUnvalidN(c_name) && !isUnvalidN(c_num)) {
                        con_v.push_back(Contacts(c_name, c_num));
                        inv.addItem(Item(name, de, con_v.back()));
                    } else {
                        cout
                            << "Unvalid Contact Name or Num, plz re-enter Name or exit: ";
                        cin >> input;
                    }
                }

                if (input == 'e')
                    break;
                else
                    input == 'x';

                break;

            case '2':
                printf("Enter Target Item Name: ");
                cin >> name;
                if(inv.deleteItem(name)) cout << "Delete Successful" << endl;
                else cout << "Delete Unsuccessful" << endl;
                break;

            case '3':
                break;
            
            case '4':
                break;

            case 'x':
                break;

            default:
                break;
        }
    }
}