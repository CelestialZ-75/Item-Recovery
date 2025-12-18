#include "inventory.h"

Inventory::~Inventory() {
    // 释放所有动态分配的物品对象
    for(auto* item : entry) {
        delete item;
    }
    entry.clear();
}

/// @brief 查找物品
/// @param _name 物品名称
/// @return 物品索引，如果未找到返回entry.size()
unsigned int Inventory::findItem(string _name) {
    unsigned int i;
    for(i = 0; i<entry.size() ; i++) {
        if(entry[i]->getName() == _name)
            break;
    }
    return i;
}

bool Inventory::deleteItem(string _name){
    unsigned int index = findItem(_name);
    if(index==entry.size()) return false;
    else{
        delete entry[index];  // 释放内存
        entry.erase(entry.begin() + index);
        return true;
    }
}

void Inventory::printInv() {
    for(auto* item : entry) {
        if(item != nullptr) {
            item->printItem();
            cout << "-------------------" << endl;
        }
    }
}

void Inventory::printItem(int index) {
    if(index >= 0 && static_cast<unsigned int>(index) < entry.size() && entry[index] != nullptr) {
        entry[index]->printItem();
    }
}