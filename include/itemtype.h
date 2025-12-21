#ifndef ITEMTYPE
#define ITEMTYPE

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

// 属性类型枚举
enum class AttributeType {
    STRING,     // 字符串类型
    INTEGER,    // 整数类型
    FLOAT       // 浮点数类型
};

class ItemTypeAttribute {
  private:
    string name;            // 属性名称
    AttributeType type;     // 属性类型
    
  public:
    ItemTypeAttribute(string _name, AttributeType _type)
        : name(_name), type(_type) {}
    
    string getName() const { return name; }
    AttributeType getType() const { return type; }
    
    void printAttribute() const;
};

class ItemType {
  private:
    string typeName;                                    // 类型名称
    vector<ItemTypeAttribute> attributes;              // 属性列表
    
  public:
    ItemType(string _typeName) : typeName(_typeName) {}
    
    string getTypeName() const { return typeName; }
    vector<ItemTypeAttribute>& getAttributes() { return attributes; }
    const vector<ItemTypeAttribute>& getAttributes() const { return attributes; }
    
    // 添加属性
    void addAttribute(string attrName, AttributeType attrType);
    
    // 修改属性（通过索引）
    bool modifyAttribute(int index, string newName, AttributeType newType);
    
    // 删除属性
    bool removeAttribute(int index);
    
    // 打印类型信息
    void printType();
};

class ItemTypeManager {
  private:
    vector<ItemType> itemTypes;
    
  public:
    ItemTypeManager();
    ~ItemTypeManager() {}
    
    // 添加新的物品类型
    bool addItemType(string typeName);
    
    // 查找物品类型
    ItemType* findItemType(string typeName);
    
    // 修改物品类型（添加属性）
    bool addAttributeToType(string typeName, string attrName, AttributeType attrType);
    
    // 修改物品类型（修改属性）
    bool modifyAttributeInType(string typeName, int attrIndex, string newName, AttributeType newType);
    
    // 获取所有类型
    vector<ItemType>& getItemTypes() { return itemTypes; }
    
    // 打印所有类型
    void printAllTypes();
    
    // 选择类型（用于界面交互）
    ItemType* selectType();
    
    // 数据持久化
    bool saveToFile(const string& filename = "data/itemtypes.txt");
    bool loadFromFile(const string& filename = "data/itemtypes.txt");
};

#endif

