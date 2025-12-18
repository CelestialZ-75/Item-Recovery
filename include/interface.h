#ifndef TOOL_FUNCTIONS
#define TOOL_FUNCTIONS

#include "inventory.h"
#include "item.h"
#include "user.h"
#include "itemtype.h"

bool isUnvalidN(string n);
bool isUnvalidD(string d);

class Interface {
  private:
    Inventory inv;
    vector<Contacts> con_v;
    UserManager userManager;
    ItemTypeManager itemTypeManager;

  public:
    User* currentUser;  // 当前登录的用户
    Interface();  // 默认构造函数
    Interface(Inventory &_inv) : inv(_inv), currentUser(nullptr) {};
    
    // 用户相关方法
    bool login();
    void registerUser();
    void showMainMenu();
    void showAdminMenu();
    void showNormalUserMenu();
    
    // 管理员功能
    void adminManageItemTypes();
    void adminAddItemType();
    void adminModifyItemType();
    void adminApproveUsers();
    
    // 普通用户功能
    void normalUserAddItem();
    void normalUserSearchItem();
    
    // 通用功能
    void entrySelect(char input);
    ItemCategory selectCategory();  // 选择物品类别（向后兼容）
};

#endif