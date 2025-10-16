#include <iostream>

#include "item.h"
#include "inventory.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    char input = '0';
    vector<Item> v;
    Inventory e(v); 
    Interface i(e);
    
    while(input != 'e'){
        i.helloCommand();
        cin >> input;
    
        i.entrySelect(input);
    }
}
