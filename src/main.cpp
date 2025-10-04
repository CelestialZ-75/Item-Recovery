#include <iostream>

#include "item.h"

int main(int argc, char *argv[])
{
    Contacts ZHOU("Zhou", "13661716426");
    Item mouse("mouse","a gift to my mom",ZHOU);

    mouse.printItem(); 
}
