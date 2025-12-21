#include "itemtype.h"

void ItemTypeAttribute::printAttribute() const {
    string typeStr;
    switch(type) {
        case AttributeType::STRING: typeStr = "字符串"; break;
        case AttributeType::INTEGER: typeStr = "整数"; break;
        case AttributeType::FLOAT: typeStr = "浮点数"; break;
    }
    cout << "  - " << name << " (" << typeStr << ")" << endl;
}

void ItemType::addAttribute(string attrName, AttributeType attrType) {
    attributes.push_back(ItemTypeAttribute(attrName, attrType));
}

bool ItemType::modifyAttribute(int index, string newName, AttributeType newType) {
    if (index >= 0 && static_cast<unsigned int>(index) < attributes.size()) {
        attributes[index] = ItemTypeAttribute(newName, newType);
        return true;
    }
    return false;
}

bool ItemType::removeAttribute(int index) {
    if (index >= 0 && static_cast<unsigned int>(index) < attributes.size()) {
        attributes.erase(attributes.begin() + index);
        return true;
    }
    return false;
}

void ItemType::printType() {
    cout << "类型名称: " << typeName << endl;
    cout << "属性列表:" << endl;
    if (attributes.empty()) {
        cout << "  (无属性)" << endl;
    } else {
        for (const auto& attr : attributes) {
            attr.printAttribute();
        }
    }
}

ItemTypeManager::ItemTypeManager() {
    // 尝试从文件加载物品类型数据
    if (!loadFromFile()) {
        // 如果加载失败（文件不存在），初始化默认的物品类型
        addItemType("食品");
        ItemType* foodType = findItemType("食品");
        if (foodType) {
            foodType->addAttribute("保质期", AttributeType::STRING);
            foodType->addAttribute("数量", AttributeType::INTEGER);
        }
        
        addItemType("书籍");
        ItemType* bookType = findItemType("书籍");
        if (bookType) {
            bookType->addAttribute("作者", AttributeType::STRING);
            bookType->addAttribute("出版社", AttributeType::STRING);
        }
        
        addItemType("工具");
        ItemType* toolType = findItemType("工具");
        if (toolType) {
            toolType->addAttribute("工具状态", AttributeType::STRING);
        }
        
        addItemType("其他");
    }
}

bool ItemTypeManager::addItemType(string typeName) {
    if (findItemType(typeName) != nullptr) {
        return false;  // 类型已存在
    }
    itemTypes.push_back(ItemType(typeName));
    return true;
}

ItemType* ItemTypeManager::findItemType(string typeName) {
    for (auto& type : itemTypes) {
        if (type.getTypeName() == typeName) {
            return &type;
        }
    }
    return nullptr;
}

bool ItemTypeManager::addAttributeToType(string typeName, string attrName, AttributeType attrType) {
    ItemType* type = findItemType(typeName);
    if (type != nullptr) {
        type->addAttribute(attrName, attrType);
        return true;
    }
    return false;
}

bool ItemTypeManager::modifyAttributeInType(string typeName, int attrIndex, 
                                           string newName, AttributeType newType) {
    ItemType* type = findItemType(typeName);
    if (type != nullptr) {
        return type->modifyAttribute(attrIndex, newName, newType);
    }
    return false;
}

void ItemTypeManager::printAllTypes() {
    cout << "===== 物品类型列表 =====" << endl;
    for (unsigned int i = 0; i < itemTypes.size(); i++) {
        cout << "[" << (i + 1) << "] ";
        itemTypes[i].printType();
        cout << endl;
    }
}

ItemType* ItemTypeManager::selectType() {
    printAllTypes();
    cout << "请选择物品类型（输入序号）: ";
    int choice;
    cin >> choice;
    cin.ignore();
    
    if (choice >= 1 && static_cast<unsigned int>(choice) <= itemTypes.size()) {
        return &itemTypes[choice - 1];
    }
    return nullptr;
}

bool ItemTypeManager::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& itemType : itemTypes) {
        file << itemType.getTypeName() << "|";
        
        const vector<ItemTypeAttribute>& attrs = itemType.getAttributes();
        file << attrs.size() << "|";
        
        for (const auto& attr : attrs) {
            string typeStr;
            switch(attr.getType()) {
                case AttributeType::STRING: typeStr = "STRING"; break;
                case AttributeType::INTEGER: typeStr = "INTEGER"; break;
                case AttributeType::FLOAT: typeStr = "FLOAT"; break;
            }
            file << attr.getName() << ":" << typeStr << ";";
        }
        
        file << "\n";
    }
    
    file.close();
    return true;
}

bool ItemTypeManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;  // 文件不存在或无法打开，返回false但不报错（首次运行）
    }
    
    itemTypes.clear();  // 清空现有类型
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string typeName;
        string attrCountStr;
        
        if (getline(ss, typeName, '|') && getline(ss, attrCountStr, '|')) {
            ItemType itemType(typeName);
            
            int attrCount = 0;
            try {
                attrCount = stoi(attrCountStr);
            } catch (...) {
                attrCount = 0;
            }
            
            if (attrCount > 0) {
                string attrsStr;
                if (getline(ss, attrsStr)) {
                    stringstream attrsStream(attrsStr);
                    string attrPair;
                    
                    while (getline(attrsStream, attrPair, ';')) {
                        if (attrPair.empty()) continue;
                        
                        size_t colonPos = attrPair.find(':');
                        if (colonPos != string::npos) {
                            string attrName = attrPair.substr(0, colonPos);
                            string attrTypeStr = attrPair.substr(colonPos + 1);
                            
                            AttributeType attrType = AttributeType::STRING;
                            if (attrTypeStr == "INTEGER") {
                                attrType = AttributeType::INTEGER;
                            } else if (attrTypeStr == "FLOAT") {
                                attrType = AttributeType::FLOAT;
                            }
                            
                            itemType.addAttribute(attrName, attrType);
                        }
                    }
                }
            }
            
            itemTypes.push_back(itemType);
        }
    }
    
    file.close();
    return true;
}

