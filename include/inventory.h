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
    unsigned int findItem(string _name);
    void addItem(Item _item) { entry.push_back(_item); }
    bool deleteItem(string _name); 
    void printInv();
    void printItem(int index);
    unsigned int getSize() { return entry.size(); }
};


#endif