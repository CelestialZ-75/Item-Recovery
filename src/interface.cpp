#include <iostream>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include "interface.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

bool isUnvalidN(string n) { return n == "NaN" || n.empty(); }

bool isUnvalidD(string d) { return d == "NaD" || d.empty(); }

Interface::Interface() : currentUser(nullptr) {
    // 加载所有数据
    loadAllData();
}

bool Interface::login() {
    string username, password;
    cout << "===== 用户登录 =====" << endl;
    cout << "用户名: ";
    cin >> username;
    cout << "密码: ";
    cin >> password;
    cin.ignore();
    
    currentUser = userManager.login(username, password);
    if (currentUser != nullptr) {
        if (currentUser->isApproved()) {
            cout << "登录成功！" << endl;
            return true;
        } else {
            cout << "您的账户尚未被管理员批准，请等待审批。" << endl;
            currentUser = nullptr;
            return false;
        }
    } else {
        cout << "用户名或密码错误！" << endl;
        return false;
    }
}

void Interface::registerUser() {
    string username, password, address, phone, email;
    cout << "===== 用户注册 =====" << endl;
    cout << "用户名: ";
    cin >> username;
    cout << "密码: ";
    cin >> password;
    cin.ignore();
    cout << "住址: ";
    getline(cin, address);
    cout << "联系电话: ";
    getline(cin, phone);
    cout << "邮箱: ";
    getline(cin, email);
    
    if (userManager.registerUser(username, password, address, phone, email)) {
        cout << "注册成功！请等待管理员审批后即可登录使用。" << endl;
        saveAllData();  // 保存数据
    } else {
        cout << "注册失败！用户名可能已存在。" << endl;
    }
}

void Interface::showMainMenu() {
    if (currentUser == nullptr) {
        cout << "请先登录或注册！" << endl;
        return;
    }
    
    if (currentUser->isAdmin()) {
        showAdminMenu();
    } else {
        showNormalUserMenu();
    }
}

void Interface::showAdminMenu() {
    cout << "\n===== 管理员菜单 =====" << endl;
    cout << "1. 添加物品" << endl;
    cout << "2. 删除物品" << endl;
    cout << "3. 查看所有物品" << endl;
    cout << "4. 搜索物品" << endl;
    cout << "5. 管理物品类型" << endl;
    cout << "6. 审批用户" << endl;
    cout << "e. 退出登录" << endl;
    cout << "请选择操作: ";
}

void Interface::showNormalUserMenu() {
    cout << "\n===== 普通用户菜单 =====" << endl;
    cout << "1. 添加物品" << endl;
    cout << "2. 删除物品" << endl;
    cout << "3. 查看所有物品" << endl;
    cout << "4. 搜索物品" << endl;
    cout << "e. 退出登录" << endl;
    cout << "请选择操作: ";
}

void Interface::adminManageItemTypes() {
    char choice;
    while (true) {
        cout << "\n===== 物品类型管理 =====" << endl;
        cout << "1. 查看所有物品类型" << endl;
        cout << "2. 添加新物品类型" << endl;
        cout << "3. 修改物品类型属性" << endl;
        cout << "b. 返回" << endl;
        cout << "请选择操作: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case '1':
                itemTypeManager.printAllTypes();
                break;
            case '2':
                adminAddItemType();
                break;
            case '3':
                adminModifyItemType();
                break;
            case 'b':
                return;
            default:
                cout << "无效选择！" << endl;
                break;
        }
    }
}

void Interface::adminAddItemType() {
    string typeName;
    cout << "请输入新物品类型名称: ";
    getline(cin, typeName);
    
    if (itemTypeManager.addItemType(typeName)) {
        cout << "物品类型添加成功！" << endl;
        // 添加属性
        char addAttr;
        bool attrAdded = false;
        do {
            cout << "是否添加属性？(y/n): ";
            cin >> addAttr;
            cin.ignore();
            if (addAttr == 'y' || addAttr == 'Y') {
                string attrName;
                int attrType;
                cout << "属性名称: ";
                getline(cin, attrName);
                cout << "属性类型 (1-字符串, 2-整数, 3-浮点数): ";
                cin >> attrType;
                cin.ignore();
                
                AttributeType type = AttributeType::STRING;
                if (attrType == 2) type = AttributeType::INTEGER;
                else if (attrType == 3) type = AttributeType::FLOAT;
                
                if (itemTypeManager.addAttributeToType(typeName, attrName, type)) {
                    cout << "属性添加成功！" << endl;
                    attrAdded = true;
                }
            }
        } while (addAttr == 'y' || addAttr == 'Y');
        
        if (attrAdded || addAttr != 'y') {
            saveAllData();  // 保存数据
        }
    } else {
        cout << "物品类型已存在！" << endl;
    }
}

void Interface::adminModifyItemType() {
    itemTypeManager.printAllTypes();
    cout << "请输入要修改的物品类型名称: ";
    string typeName;
    getline(cin, typeName);
    
    ItemType* type = itemTypeManager.findItemType(typeName);
    if (type == nullptr) {
        cout << "物品类型不存在！" << endl;
        return;
    }
    
    cout << "\n当前类型信息：" << endl;
    type->printType();
    
    cout << "\n1. 添加属性" << endl;
    cout << "2. 修改属性" << endl;
    cout << "请选择操作: ";
    char choice;
    cin >> choice;
    cin.ignore();
    
    if (choice == '1') {
        string attrName;
        int attrType;
        cout << "属性名称: ";
        getline(cin, attrName);
        cout << "属性类型 (1-字符串, 2-整数, 3-浮点数): ";
        cin >> attrType;
        cin.ignore();
        
        AttributeType typeEnum = AttributeType::STRING;
        if (attrType == 2) typeEnum = AttributeType::INTEGER;
        else if (attrType == 3) typeEnum = AttributeType::FLOAT;
        
        if (itemTypeManager.addAttributeToType(typeName, attrName, typeEnum)) {
            cout << "属性添加成功！" << endl;
            saveAllData();  // 保存数据
        }
    } else if (choice == '2') {
        // 显示属性列表
        vector<ItemTypeAttribute>& attrs = type->getAttributes();
        if (attrs.empty()) {
            cout << "该类型没有属性！" << endl;
            return;
        }
        
        for (unsigned int i = 0; i < attrs.size(); i++) {
            cout << "[" << (i + 1) << "] " << attrs[i].getName() << endl;
        }
        cout << "请选择要修改的属性序号: ";
        int index;
        cin >> index;
        cin.ignore();
        index--;  // 转换为0-based索引
        
        if (index >= 0 && static_cast<unsigned int>(index) < attrs.size()) {
            string newName;
            int newType;
            cout << "新属性名称: ";
            getline(cin, newName);
            cout << "新属性类型 (1-字符串, 2-整数, 3-浮点数): ";
            cin >> newType;
            cin.ignore();
            
            AttributeType typeEnum = AttributeType::STRING;
            if (newType == 2) typeEnum = AttributeType::INTEGER;
            else if (newType == 3) typeEnum = AttributeType::FLOAT;
            
            if (itemTypeManager.modifyAttributeInType(typeName, index, newName, typeEnum)) {
                cout << "属性修改成功！" << endl;
                saveAllData();  // 保存数据
            }
        } else {
            cout << "无效的序号！" << endl;
        }
    }
}

void Interface::adminApproveUsers() {
    vector<User*> pendingUsers = userManager.getPendingUsers();
    if (pendingUsers.empty()) {
        cout << "没有待审批的用户。" << endl;
        return;
    }
    
    cout << "\n===== 待审批用户列表 =====" << endl;
    for (unsigned int i = 0; i < pendingUsers.size(); i++) {
        cout << "[" << (i + 1) << "] " << endl;
        pendingUsers[i]->printUser();
        cout << endl;
    }
    
    cout << "请输入要批准的用户名（输入 'b' 返回）: ";
    string username;
    getline(cin, username);
    
    if (username != "b") {
        if (userManager.approveUser(username)) {
            cout << "用户批准成功！" << endl;
            saveAllData();  // 保存数据
        } else {
            cout << "批准失败！用户名不存在或已被批准。" << endl;
        }
    }
}

void Interface::normalUserAddItem() {
    // 先选择物品类型
    ItemType* selectedType = itemTypeManager.selectType();
    if (selectedType == nullptr) {
        cout << "无效的物品类型选择！" << endl;
        return;
    }
    
    string name, de, addr;
    cout << "物品名称: ";
    getline(cin, name);
    cout << "物品说明: ";
    getline(cin, de);
    cout << "物品所在地址: ";
    getline(cin, addr);
    
    // 使用当前用户的联系信息
    Contacts con(currentUser->getUsername(), currentUser->getPhone(), currentUser->getEmail());
    con_v.push_back(con);
    
    // 创建物品（使用类型名称）
    Item* newItem = new Item(name, de, addr, con_v.back(), selectedType->getTypeName());
    inv.addItem(newItem);
    
    cout << "物品添加成功！" << endl;
    saveAllData();  // 保存数据
}

void Interface::normalUserSearchItem() {
    // 先选择物品类型
    ItemType* selectedType = itemTypeManager.selectType();
    if (selectedType == nullptr) {
        cout << "无效的物品类型选择！" << endl;
        return;
    }
    
    string keyword;
    cout << "请输入搜索关键字（将在物品名称、说明和联系人名称中搜索）: ";
    getline(cin, keyword);
    
    vector<int> results = inv.searchByTypeAndKeyword(selectedType->getTypeName(), keyword);
    
    if (results.empty()) {
        cout << "未找到匹配的物品。" << endl;
    } else {
        cout << "\n找到 " << results.size() << " 个匹配的物品：" << endl;
        for (unsigned int i = 0; i < results.size(); i++) {
            cout << "--- 物品 " << (i + 1) << " ---" << endl;
            inv.printItem(results[i]);
            cout << endl;
        }
    }
}

void Interface::entrySelect(char input) {
    if (currentUser == nullptr) {
        return;
    }
    
    string name, de, addr;
    
    if (currentUser->isAdmin()) {
        // 管理员菜单处理
        switch (input) {
            case '1': {
                // 添加物品（管理员也可以使用新系统）
                normalUserAddItem();
                break;
            }
            case '2': {
                cout << "请输入要删除的物品名称: ";
                cin.ignore();
                getline(cin, name);
                if (inv.deleteItem(name)) {
                    cout << "删除成功！" << endl;
                    saveAllData();  // 保存数据
                } else {
                    cout << "删除失败！物品不存在。" << endl;
                }
                break;
            }
            case '3': {
                if (inv.getSize() == 0) {
                    cout << "库存为空。" << endl;
                } else {
                    inv.printInv();
                }
                break;
            }
            case '4': {
                normalUserSearchItem();
                break;
            }
            case '5': {
                adminManageItemTypes();
                break;
            }
            case '6': {
                adminApproveUsers();
                break;
            }
            case 'e': {
                currentUser = nullptr;
                cout << "已退出登录。" << endl;
                break;
            }
            default:
                cout << "无效选择！" << endl;
                break;
        }
    } else {
        // 普通用户菜单处理
        switch (input) {
            case '1': {
                normalUserAddItem();
                break;
            }
            case '2': {
                cout << "请输入要删除的物品名称: ";
                cin.ignore();
                getline(cin, name);
                if (inv.deleteItem(name)) {
                    cout << "删除成功！" << endl;
                    saveAllData();  // 保存数据
                } else {
                    cout << "删除失败！物品不存在。" << endl;
                }
                break;
            }
            case '3': {
                if (inv.getSize() == 0) {
                    cout << "库存为空。" << endl;
                } else {
                    inv.printInv();
                }
                break;
            }
            case '4': {
                normalUserSearchItem();
                break;
            }
            case 'e': {
                currentUser = nullptr;
                cout << "已退出登录。" << endl;
                break;
            }
            default:
                cout << "无效选择！" << endl;
                break;
        }
    }
}

ItemCategory Interface::selectCategory() {
    int cat;
    cout << "Select item category: 1-Food(食品), 2-Book(书籍), 3-Tool(工具), 4-Other(其他): ";
    cin >> cat;
    cin.ignore();
    
    switch(cat) {
        case 1: return ItemCategory::FOOD;
        case 2: return ItemCategory::BOOK;
        case 3: return ItemCategory::TOOL;
        default: return ItemCategory::OTHER;
    }
}

void Interface::saveAllData() {
    // 确保data目录存在（如果目录已存在，mkdir会失败，但可以忽略）
#ifdef _WIN32
    CreateDirectoryA("data", NULL);
#else
    mkdir("data", 0755);  // 如果目录已存在，会返回-1，但我们可以忽略
#endif
    
    // 保存所有数据
    userManager.saveToFile();
    inv.saveToFile();
    itemTypeManager.saveToFile();
}

void Interface::loadAllData() {
    // 加载物品数据（用户和物品类型数据已在各自管理器的构造函数中加载）
    inv.loadFromFile();
}
