#include "inventory.h"
#include <cctype>
#include <algorithm>

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

vector<int> Inventory::searchByTypeAndKeyword(string typeName, string keyword) {
    vector<int> results;
    for (unsigned int i = 0; i < entry.size(); i++) {
        if (entry[i] != nullptr) {
            // 检查类型是否匹配
            if (entry[i]->getTypeName() == typeName || 
                (entry[i]->getTypeName().empty() && typeName == "其他")) {
                // 检查关键字是否匹配物品名称、说明或联系人名称
                string itemName = entry[i]->getName();
                string description = entry[i]->getDescription();
                string contactName = entry[i]->getContact().getName();
                
                // 转换为小写进行不区分大小写的匹配
                string lowerKeyword = keyword;
                string lowerItemName = itemName;
                string lowerDescription = description;
                string lowerContactName = contactName;
                
                // 简单的转换（实际应该使用更完善的函数）
                for (char& c : lowerKeyword) c = tolower(c);
                for (char& c : lowerItemName) c = tolower(c);
                for (char& c : lowerDescription) c = tolower(c);
                for (char& c : lowerContactName) c = tolower(c);
                
                if (lowerItemName.find(lowerKeyword) != string::npos ||
                    lowerDescription.find(lowerKeyword) != string::npos ||
                    lowerContactName.find(lowerKeyword) != string::npos) {
                    results.push_back(i);
                }
            }
        }
    }
    return results;
}

bool Inventory::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (auto* item : entry) {
        if (item != nullptr) {
            // 保存物品基本信息
            file << item->getName() << "|"
                 << item->getDescription() << "|"
                 << item->getAddress() << "|"
                 << item->getTypeName() << "|"
                 << item->getContact().getName() << "|"
                 << item->getContact().getNum() << "|"
                 << item->getContact().getEmail() << "\n";
        }
    }
    
    file.close();
    return true;
}

bool Inventory::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;  // 文件不存在或无法打开，返回false但不报错（首次运行）
    }
    
    // 清空现有物品
    for(auto* item : entry) {
        delete item;
    }
    entry.clear();
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string name, description, address, typeName, contactName, contactNum, contactEmail;
        
        if (getline(ss, name, '|') &&
            getline(ss, description, '|') &&
            getline(ss, address, '|') &&
            getline(ss, typeName, '|') &&
            getline(ss, contactName, '|') &&
            getline(ss, contactNum, '|') &&
            getline(ss, contactEmail, '|')) {
            
            Contacts con(contactName, contactNum, contactEmail);
            Item* newItem = new Item(name, description, address, con, typeName);
            entry.push_back(newItem);
        }
    }
    
    file.close();
    return true;
}