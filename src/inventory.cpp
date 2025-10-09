#include "inventory.h"

/// @brief 
/// @param _name 
/// @return -1 for not found
int Inventory::traverseFindItem(string _name) {
    for(int i = 0; i<entry.size() ; i++) {
        if(entry[i].getName() == _name)
            return i;
    }
    return -1;
}

bool Inventory::traverseDeleteItem(string _name){
    int index = traverseFindItem(_name);
    if(index=-1) return false;
    else{
        entry.erase(entry.begin() + index);
        return true;
    }
}
