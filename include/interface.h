#ifndef TOOL_FUNCTIONS
#define TOOL_FUNCTIONS

#include "inventory.h"
#include "item.h"

bool isUnvalidN(string n);
bool isUnvalidD(string d);

class Interface {
  private:
    Inventory inv;
    vector<Contacts> con_v;

  public:
    Interface() {};  // 默认构造函数
    Interface(Inventory &_inv) : inv(_inv) {};
    void helloCommand();
    void entrySelect(char input);
    ItemCategory selectCategory();  // 选择物品类别
};

#endif