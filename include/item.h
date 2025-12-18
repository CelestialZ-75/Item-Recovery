#ifndef ITEM
#define ITEM

#include <iostream>
#include <string>
using namespace std;

// 物品类别枚举
enum class ItemCategory {
    FOOD,    // 食品
    BOOK,    // 书籍
    TOOL,    // 工具
    OTHER    // 其他
};

class Contacts {
  private:
    string name;
    string num;
    string email;

  public:
    Contacts(string _name, string _num, string _email) 
        : name(_name), num(_num), email(_email) {}
    Contacts()
        : Contacts("NaN", "NaN", "NaN") {} // default: unvalid contact
    string getName() { return name; }
    string getNum() { return num; }
    string getEmail() { return email; }
    void addInfo(string _name, string _num, string _email);
    void deleteInfo() {addInfo("NaN","NaN","NaN");}
    void printContact();
};

class Item {
  protected:
    string name;           // 物品名称
    string description;    // 物品说明
    string address;        // 物品所在地址
    Contacts con;          // 联系人信息
    ItemCategory category; // 物品类别

  public:
    Item(string _name, string _de, string _addr, Contacts &_con, ItemCategory _cat)
        : name(_name), description(_de), address(_addr), con(_con), category(_cat) {}
    Item() : name("NaN"), description("NaD"), address("NaN"), con(), category(ItemCategory::OTHER) {}
    virtual ~Item() {}
    void addInfo(string _name, string _de, string _addr); 
    void deleteInfo() {addInfo("NaN", "NaD", "NaN");}
    virtual void printItem();
    string getName() { return name; }
    string getDescription() { return description; }
    string getAddress() { return address; }
    ItemCategory getCategory() { return category; }
    Contacts& getContact() { return con; }
};

// 食品类
class FoodItem : public Item {
  private:
    string expiryDate;  // 保质期
    int quantity;      // 数量

  public:
    FoodItem(string _name, string _de, string _addr, Contacts &_con, 
             string _expiry, int _qty)
        : Item(_name, _de, _addr, _con, ItemCategory::FOOD),
          expiryDate(_expiry), quantity(_qty) {}
    FoodItem() : Item(), expiryDate("NaN"), quantity(0) {}
    void setExpiryDate(string _expiry) { expiryDate = _expiry; }
    void setQuantity(int _qty) { quantity = _qty; }
    string getExpiryDate() { return expiryDate; }
    int getQuantity() { return quantity; }
    void printItem() override;
};

// 书籍类
class BookItem : public Item {
  private:
    string author;      // 作者
    string publisher;   // 出版社

  public:
    BookItem(string _name, string _de, string _addr, Contacts &_con,
             string _author, string _publisher)
        : Item(_name, _de, _addr, _con, ItemCategory::BOOK),
          author(_author), publisher(_publisher) {}
    BookItem() : Item(), author("NaN"), publisher("NaN") {}
    void setAuthor(string _author) { author = _author; }
    void setPublisher(string _publisher) { publisher = _publisher; }
    string getAuthor() { return author; }
    string getPublisher() { return publisher; }
    void printItem() override;
};

// 工具类
class ToolItem : public Item {
  private:
    string condition;  // 工具状态（如：全新、良好、需维修等）

  public:
    ToolItem(string _name, string _de, string _addr, Contacts &_con,
             string _condition)
        : Item(_name, _de, _addr, _con, ItemCategory::TOOL),
          condition(_condition) {}
    ToolItem() : Item(), condition("NaN") {}
    void setCondition(string _condition) { condition = _condition; }
    string getCondition() { return condition; }
    void printItem() override;
};

// 其他类别物品
class OtherItem : public Item {
  public:
    OtherItem(string _name, string _de, string _addr, Contacts &_con)
        : Item(_name, _de, _addr, _con, ItemCategory::OTHER) {}
    OtherItem() : Item() {}
};

#endif