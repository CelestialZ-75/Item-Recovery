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
    // 默认创建一个管理员账户
    users.push_back(User("admin", "admin123", UserType::ADMIN, 
                        "管理员地址", "00000000000", "admin@example.com"));
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

