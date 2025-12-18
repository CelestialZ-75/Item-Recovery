#include <iostream>
#include <string>

#include "item.h"
#include "inventory.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    (void)argc;  // 避免未使用参数警告
    (void)argv;
    
    Interface i;  // 使用默认构造函数
    char input;
    
    while(true) {
        if (i.currentUser == nullptr) {
            // 未登录，显示登录/注册菜单
            cout << "\n===== 物品复活系统 =====" << endl;
            cout << "1. 登录" << endl;
            cout << "2. 注册" << endl;
            cout << "e. 退出程序" << endl;
            cout << "请选择操作: ";
            cin >> input;
            cin.ignore();
            
            switch(input) {
                case '1':
                    if (i.login()) {
                        // 登录成功，进入主菜单循环
                        while (i.currentUser != nullptr) {
                            i.showMainMenu();
                            cin >> input;
                            cin.ignore();
                            i.entrySelect(input);
                        }
                    }
                    break;
                case '2':
                    i.registerUser();
                    break;
                case 'e':
                    cout << "再见！" << endl;
                    return 0;
                default:
                    cout << "无效选择！" << endl;
                    break;
            }
        }
    }
    
    return 0;
}
