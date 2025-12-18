#ifndef INVENTORY
#define INVENTORY

#include <vector>
#include <memory>

#include "item.h"

using namespace std;

class Inventory{
private:
    vector<Item*> entry;  // 使用指针支持多态

public:
    Inventory() {}  // 默认构造函数
    Inventory(vector<Item*> &_e): entry(_e) {};
    ~Inventory();  // 析构函数，释放动态分配的内存
    unsigned int findItem(string _name);
    void addItem(Item* _item) { entry.push_back(_item); }
    bool deleteItem(string _name); 
    void printInv();
    void printItem(int index);
    unsigned int getSize() { return entry.size(); }
};


#endif