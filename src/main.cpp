#include <iostream>

#include "item.h"
#include "inventory.h"

int main(int argc, char *argv[])
{
    char input = '0';
    vector<Item> v;
    Inventory e(v); 
    while(input != 'e'){
        helloCommand();
        cin >> input;
    
        entrySelect(input);
    }
}
