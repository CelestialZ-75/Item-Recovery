#ifndef INVENTORY
#define INVENTORY

#include <vector>

#include "item.h"

using namespace std;

class Inventory{
private:
    vector<Item> entry;

public:
    Inventory(vector<Item> &_e): entry(_e) {};
    int traverseFindItem(string _name);
    void addItem(Item _item) { entry.push_back(_item); }
    bool traverseDeleteItem(string _name); 
};


#endif