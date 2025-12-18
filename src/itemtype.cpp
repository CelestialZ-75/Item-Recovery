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
    // 初始化一些默认的物品类型
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

