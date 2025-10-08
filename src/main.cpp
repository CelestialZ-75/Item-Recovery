#include <iostream>

#include "item.h"

int main(int argc, char *argv[])
{
    Contacts ZHOU;
    Item mouse;

    ZHOU.addInfo("CZ","12121");
    mouse.addInfo("mouse","a gift");

    mouse.printItem(); 
}
