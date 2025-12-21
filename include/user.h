#ifndef USER
#define USER

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// 用户类型枚举
enum class UserType {
    ADMIN,      // 管理员
    NORMAL      // 普通用户
};

// 用户状态枚举
enum class UserStatus {
    PENDING,    // 待审批
    APPROVED    // 已批准
};

class User {
  private:
    string username;        // 用户名
    string password;        // 密码
    UserType userType;      // 用户类型
    UserStatus status;      // 用户状态
    string address;         // 住址
    string phone;           // 联系方式（电话）
    string email;           // 邮箱

  public:
    User(string _username, string _password, UserType _type, 
         string _address, string _phone, string _email)
        : username(_username), password(_password), userType(_type),
          status(_type == UserType::ADMIN ? UserStatus::APPROVED : UserStatus::PENDING),
          address(_address), phone(_phone), email(_email) {}
    
    User() : username(""), password(""), userType(UserType::NORMAL),
             status(UserStatus::PENDING), address(""), phone(""), email("") {}
    
    // Getter方法
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    UserType getUserType() const { return userType; }
    UserStatus getStatus() const { return status; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    
    // Setter方法
    void setStatus(UserStatus _status) { status = _status; }
    void approve() { status = UserStatus::APPROVED; }
    void reject() { status = UserStatus::PENDING; }
    
    bool isAdmin() const { return userType == UserType::ADMIN; }
    bool isApproved() const { return status == UserStatus::APPROVED; }
    
    void printUser();
};

class UserManager {
  private:
    vector<User> users;
    
  public:
    UserManager();
    ~UserManager() {}
    
    // 用户注册
    bool registerUser(string username, string password, string address, 
                     string phone, string email);
    
    // 用户登录
    User* login(string username, string password);
    
    // 查找用户
    User* findUser(string username);
    
    // 获取待审批用户列表
    vector<User*> getPendingUsers();
    
    // 批准用户
    bool approveUser(string username);
    
    // 获取所有用户
    vector<User>& getUsers() { return users; }
    
    // 数据持久化
    bool saveToFile(const string& filename = "data/users.txt");
    bool loadFromFile(const string& filename = "data/users.txt");
};

#endif

