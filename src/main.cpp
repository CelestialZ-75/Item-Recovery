#include <iostream>

#include "item.h"
#include "inventory.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    (void)argc;  // 避免未使用参数警告
    (void)argv;
    
    char input = '0';
    Interface i;  // 使用默认构造函数
    
    while(input != 'e'){
        i.helloCommand();
        cin >> input;
    
        i.entrySelect(input);
    }
    
    return 0;
}
