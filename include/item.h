#ifndef ITEM
#define ITEM

#include <iostream>
#include <string>
using namespace std;

class Contacts {
  private:
    string name;
    string num;

  public:
    Contacts(string _name, string _num) : name(_name), num(_num) {}
    Contacts()
        : Contacts("NaN", "NaN") {} // default: unvalid contact (not any name/number)
    string getName() { return name; }
    string getNum() { return num; }
    void addInfo(string _name, string _num);
    void printContact();
};

class Item {
  private:
    string name;
    string description;
    Contacts contacts;

  public:
    Item(string _name, string _de, Contacts _con)
        : name(_name), description(_de), contacts(_con) {}
    Item() : Item("NaN", "NaD", Contacts()) {} // default: unvalid item
    ~Item() {}
    void addInfo(string _name, string _de, Contacts _con); 
    void printItem();
};

#endif