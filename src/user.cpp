#include "user.h"
#include <algorithm>

void User::printUser() {
    cout << "用户名: " << username << endl;
    cout << "用户类型: " << (userType == UserType::ADMIN ? "管理员" : "普通用户") << endl;
    cout << "状态: " << (status == UserStatus::APPROVED ? "已批准" : "待审批") << endl;
    cout << "住址: " << address << endl;
    cout << "电话: " << phone << endl;
    cout << "邮箱: " << email << endl;
}

UserManager::UserManager() {
    // 尝试从文件加载用户数据
    if (!loadFromFile()) {
        // 如果加载失败（文件不存在），创建默认管理员账户
        users.push_back(User("admin", "admin123", UserType::ADMIN, 
                            "管理员地址", "00000000000", "admin@example.com"));
    }
}

bool UserManager::registerUser(string username, string password, string address,
                               string phone, string email) {
    // 检查用户名是否已存在
    if (findUser(username) != nullptr) {
        return false;
    }
    
    // 创建新用户
    users.push_back(User(username, password, UserType::NORMAL, 
                        address, phone, email));
    return true;
}

User* UserManager::login(string username, string password) {
    User* user = findUser(username);
    if (user != nullptr && user->getPassword() == password) {
        return user;
    }
    return nullptr;
}

User* UserManager::findUser(string username) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            return &user;
        }
    }
    return nullptr;
}

vector<User*> UserManager::getPendingUsers() {
    vector<User*> pending;
    for (auto& user : users) {
        if (user.getStatus() == UserStatus::PENDING) {
            pending.push_back(&user);
        }
    }
    return pending;
}

bool UserManager::approveUser(string username) {
    User* user = findUser(username);
    if (user != nullptr && user->getStatus() == UserStatus::PENDING) {
        user->approve();
        return true;
    }
    return false;
}

bool UserManager::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& user : users) {
        file << user.getUsername() << "|"
             << user.getPassword() << "|"
             << (user.getUserType() == UserType::ADMIN ? "ADMIN" : "NORMAL") << "|"
             << (user.getStatus() == UserStatus::APPROVED ? "APPROVED" : "PENDING") << "|"
             << user.getAddress() << "|"
             << user.getPhone() << "|"
             << user.getEmail() << "\n";
    }
    
    file.close();
    return true;
}

bool UserManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;  // 文件不存在或无法打开，返回false但不报错（首次运行）
    }
    
    users.clear();  // 清空现有用户（除了可能需要保留的默认管理员）
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string username, password, typeStr, statusStr, address, phone, email;
        
        if (getline(ss, username, '|') &&
            getline(ss, password, '|') &&
            getline(ss, typeStr, '|') &&
            getline(ss, statusStr, '|') &&
            getline(ss, address, '|') &&
            getline(ss, phone, '|') &&
            getline(ss, email, '|')) {
            
            UserType type = (typeStr == "ADMIN") ? UserType::ADMIN : UserType::NORMAL;
            User user(username, password, type, address, phone, email);
            
            if (statusStr == "APPROVED") {
                user.approve();
            }
            
            users.push_back(user);
        }
    }
    
    file.close();
    return true;
}

