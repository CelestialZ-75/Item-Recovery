#include "inventory.h"

/// @brief 
/// @param _name 
/// @return entry size for not found
unsigned int Inventory::findItem(string _name) {
    unsigned int i;
    for(i = 0; i<entry.size() ; i++) {
        if(entry[i].getName() == _name)
            break;
    }
    return i;
}

bool Inventory::deleteItem(string _name){
    unsigned int index = findItem(_name);
    if(index==entry.size()) return false;
    else{
        entry.erase(entry.begin() + index);
        return true;
    }
}

void Inventory::printInv() {
    for(auto& item : entry) {
        item.printItem();
    }
}

void Inventory::printItem(int index) {
    (entry.begin() + index) -> printItem();
}