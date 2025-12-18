#include "mainwindow.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

// ==================== LoginDialog 实现 ====================

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), registerMode(false)
{
    setupUI();
}

void LoginDialog::setupUI() {
    setWindowTitle("用户登录/注册");
    setMinimumSize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("用户登录", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(titleLabel);
    
    formLayout = new QFormLayout();
    
    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    addressEdit = new QLineEdit(this);
    phoneEdit = new QLineEdit(this);
    emailEdit = new QLineEdit(this);
    
    formLayout->addRow("用户名:", usernameEdit);
    formLayout->addRow("密码:", passwordEdit);
    formLayout->addRow("住址:", addressEdit);
    formLayout->addRow("电话:", phoneEdit);
    formLayout->addRow("邮箱:", emailEdit);
    
    addressEdit->setVisible(false);
    phoneEdit->setVisible(false);
    emailEdit->setVisible(false);
    formLayout->labelForField(addressEdit)->setVisible(false);
    formLayout->labelForField(phoneEdit)->setVisible(false);
    formLayout->labelForField(emailEdit)->setVisible(false);
    
    layout->addLayout(formLayout);
    
    switchButton = new QPushButton("切换到注册", this);
    connect(switchButton, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
    layout->addWidget(switchButton);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &LoginDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &LoginDialog::reject);
    layout->addWidget(buttonBox);
}

void LoginDialog::switchToRegister() {
    registerMode = true;
    titleLabel->setText("用户注册");
    switchButton->setText("切换到登录");
    addressEdit->setVisible(true);
    phoneEdit->setVisible(true);
    emailEdit->setVisible(true);
    formLayout->labelForField(addressEdit)->setVisible(true);
    formLayout->labelForField(phoneEdit)->setVisible(true);
    formLayout->labelForField(emailEdit)->setVisible(true);
    disconnect(switchButton, nullptr, nullptr, nullptr);
    connect(switchButton, &QPushButton::clicked, this, &LoginDialog::switchToLogin);
}

void LoginDialog::switchToLogin() {
    registerMode = false;
    titleLabel->setText("用户登录");
    switchButton->setText("切换到注册");
    addressEdit->setVisible(false);
    phoneEdit->setVisible(false);
    emailEdit->setVisible(false);
    formLayout->labelForField(addressEdit)->setVisible(false);
    formLayout->labelForField(phoneEdit)->setVisible(false);
    formLayout->labelForField(emailEdit)->setVisible(false);
    disconnect(switchButton, nullptr, nullptr, nullptr);
    connect(switchButton, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
}

void LoginDialog::accept() {
    if (registerMode) {
        if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty() ||
            addressEdit->text().isEmpty() || phoneEdit->text().isEmpty() || emailEdit->text().isEmpty()) {
            QMessageBox::warning(this, "警告", "请填写所有字段！");
            return;
        }
    } else {
        if (usernameEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
            QMessageBox::warning(this, "警告", "请输入用户名和密码！");
            return;
        }
    }
    QDialog::accept();
}

// ==================== ItemTypeDialog 实现 ====================

ItemTypeDialog::ItemTypeDialog(ItemTypeManager* manager, QWidget *parent)
    : QDialog(parent), itemTypeManager(manager)
{
    setWindowTitle("选择物品类型");
    setMinimumSize(300, 150);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("请选择物品类型:", this));
    
    typeComboBox = new QComboBox(this);
    vector<ItemType>& allTypes = itemTypeManager->getItemTypes();
    for (auto& type : allTypes) {
        typeComboBox->addItem(QString::fromStdString(type.getTypeName()));
        types.push_back(&type);
    }
    layout->addWidget(typeComboBox);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

ItemType* ItemTypeDialog::getSelectedType() const {
    int index = typeComboBox->currentIndex();
    if (index >= 0 && static_cast<unsigned int>(index) < types.size()) {
        return types[index];
    }
    return nullptr;
}

// ==================== AddItemDialog 实现 ====================

AddItemDialog::AddItemDialog(ItemTypeManager* manager, User* currentUser, QWidget *parent)
    : QDialog(parent), itemTypeManager(manager)
{
    setWindowTitle("添加物品");
    setMinimumSize(400, 250);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout();
    
    nameEdit = new QLineEdit(this);
    descEdit = new QLineEdit(this);
    addressEdit = new QLineEdit(this);
    typeComboBox = new QComboBox(this);
    
    formLayout->addRow("物品名称:", nameEdit);
    formLayout->addRow("物品说明:", descEdit);
    formLayout->addRow("物品地址:", addressEdit);
    formLayout->addRow("物品类型:", typeComboBox);
    
    vector<ItemType>& allTypes = itemTypeManager->getItemTypes();
    for (auto& type : allTypes) {
        typeComboBox->addItem(QString::fromStdString(type.getTypeName()));
        types.push_back(&type);
    }
    
    layout->addLayout(formLayout);
    
    if (currentUser) {
        QLabel *contactLabel = new QLabel(
            QString("联系人信息: %1 (%2, %3)")
                .arg(QString::fromStdString(currentUser->getUsername()))
                .arg(QString::fromStdString(currentUser->getPhone()))
                .arg(QString::fromStdString(currentUser->getEmail())), this);
        layout->addWidget(contactLabel);
    }
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

ItemType* AddItemDialog::getItemType() const {
    int index = typeComboBox->currentIndex();
    if (index >= 0 && static_cast<unsigned int>(index) < types.size()) {
        return types[index];
    }
    return nullptr;
}

// ==================== SearchDialog 实现 ====================

SearchDialog::SearchDialog(ItemTypeManager* manager, QWidget *parent)
    : QDialog(parent), itemTypeManager(manager)
{
    setWindowTitle("搜索物品");
    setMinimumSize(350, 150);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QFormLayout *formLayout = new QFormLayout();
    
    keywordEdit = new QLineEdit(this);
    typeComboBox = new QComboBox(this);
    
    formLayout->addRow("搜索关键字:", keywordEdit);
    formLayout->addRow("物品类型:", typeComboBox);
    
    // 添加"全部类型"选项
    typeComboBox->addItem("全部类型");
    types.push_back(nullptr);  // 第一个选项对应 nullptr，表示不限制类型
    
    vector<ItemType>& allTypes = itemTypeManager->getItemTypes();
    for (auto& type : allTypes) {
        typeComboBox->addItem(QString::fromStdString(type.getTypeName()));
        types.push_back(&type);
    }
    
    layout->addLayout(formLayout);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

ItemType* SearchDialog::getItemType() const {
    int index = typeComboBox->currentIndex();
    if (index >= 0 && static_cast<unsigned int>(index) < types.size()) {
        return types[index];  // 可能返回 nullptr 表示不限制类型
    }
    return nullptr;
}

// ==================== AdminDialog 实现 ====================

AdminDialog::AdminDialog(ItemTypeManager* manager, UserManager* userManager, QWidget *parent)
    : QDialog(parent), itemTypeManager(manager), userManager(userManager)
{
    setWindowTitle("管理员功能");
    setMinimumSize(600, 500);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QTabWidget *tabWidget = new QTabWidget(this);
    
    // 物品类型管理标签页
    QWidget *typeTab = new QWidget(this);
    QVBoxLayout *typeLayout = new QVBoxLayout(typeTab);
    
    QHBoxLayout *typeButtonLayout = new QHBoxLayout();
    QPushButton *addTypeButton = new QPushButton("添加类型", this);
    QPushButton *addAttrButton = new QPushButton("添加属性", this);
    QPushButton *modifyAttrButton = new QPushButton("修改属性", this);
    QPushButton *deleteAttrButton = new QPushButton("删除属性", this);
    QPushButton *refreshTypeButton = new QPushButton("刷新", this);
    connect(addTypeButton, &QPushButton::clicked, this, &AdminDialog::addItemType);
    connect(addAttrButton, &QPushButton::clicked, this, &AdminDialog::addAttributeToType);
    connect(modifyAttrButton, &QPushButton::clicked, this, &AdminDialog::modifyAttributeInType);
    connect(deleteAttrButton, &QPushButton::clicked, this, &AdminDialog::deleteAttributeFromType);
    connect(refreshTypeButton, &QPushButton::clicked, this, &AdminDialog::refreshTypeList);
    typeButtonLayout->addWidget(addTypeButton);
    typeButtonLayout->addWidget(addAttrButton);
    typeButtonLayout->addWidget(modifyAttrButton);
    typeButtonLayout->addWidget(deleteAttrButton);
    typeButtonLayout->addWidget(refreshTypeButton);
    typeButtonLayout->addStretch();
    
    typeListWidget = new QListWidget(this);
    typeLayout->addLayout(typeButtonLayout);
    typeLayout->addWidget(typeListWidget);
    
    tabWidget->addTab(typeTab, "物品类型管理");
    
    // 用户审批标签页
    QWidget *userTab = new QWidget(this);
    QVBoxLayout *userLayout = new QVBoxLayout(userTab);
    
    QHBoxLayout *userButtonLayout = new QHBoxLayout();
    QPushButton *approveButton = new QPushButton("批准选中用户", this);
    QPushButton *refreshUserButton = new QPushButton("刷新", this);
    connect(approveButton, &QPushButton::clicked, this, &AdminDialog::approveUsers);
    connect(refreshUserButton, &QPushButton::clicked, this, &AdminDialog::refreshUserList);
    userButtonLayout->addWidget(approveButton);
    userButtonLayout->addWidget(refreshUserButton);
    userButtonLayout->addStretch();
    
    userListWidget = new QListWidget(this);
    userLayout->addLayout(userButtonLayout);
    userLayout->addWidget(userListWidget);
    
    tabWidget->addTab(userTab, "用户审批");
    
    layout->addWidget(tabWidget);
    
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
    
    refreshTypeList();
    refreshUserList();
}

void AdminDialog::refreshTypeList() {
    typeListWidget->clear();
    vector<ItemType>& allTypes = itemTypeManager->getItemTypes();
    for (auto& type : allTypes) {
        QString typeInfo = QString::fromStdString(type.getTypeName());
        const vector<ItemTypeAttribute>& attrs = type.getAttributes();
        if (!attrs.empty()) {
            typeInfo += " [属性: ";
            for (unsigned int i = 0; i < attrs.size(); i++) {
                if (i > 0) typeInfo += ", ";
                typeInfo += QString::fromStdString(attrs[i].getName());
            }
            typeInfo += "]";
        }
        typeListWidget->addItem(typeInfo);
    }
}

void AdminDialog::refreshUserList() {
    userListWidget->clear();
    vector<User>& allUsers = userManager->getUsers();
    for (auto& user : allUsers) {
        QString userInfo = QString("%1 [%2] - %3")
            .arg(QString::fromStdString(user.getUsername()))
            .arg(user.isAdmin() ? "管理员" : "普通用户")
            .arg(user.isApproved() ? "已批准" : "待审批");
        userListWidget->addItem(userInfo);
    }
}

void AdminDialog::addItemType() {
    bool ok;
    QString typeName = QInputDialog::getText(this, "添加物品类型", "请输入类型名称:", 
                                              QLineEdit::Normal, "", &ok);
    if (ok && !typeName.isEmpty()) {
        if (itemTypeManager->addItemType(typeName.toStdString())) {
            QMessageBox::information(this, "成功", "物品类型添加成功！");
            refreshTypeList();
        } else {
            QMessageBox::warning(this, "错误", "物品类型已存在！");
        }
    }
}

void AdminDialog::addAttributeToType() {
    QListWidgetItem *item = typeListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "警告", "请先选择一个物品类型！");
        return;
    }
    
    QString typeName = item->text().split(" [").first();
    ItemType* type = itemTypeManager->findItemType(typeName.toStdString());
    if (!type) {
        QMessageBox::warning(this, "错误", "找不到该物品类型！");
        return;
    }
    
    bool ok;
    QString attrName = QInputDialog::getText(this, "添加属性", "请输入属性名称:", 
                                             QLineEdit::Normal, "", &ok);
    if (ok && !attrName.isEmpty()) {
        QStringList types;
        types << "字符串" << "整数" << "浮点数";
        QString selected = QInputDialog::getItem(this, "选择属性类型", "属性类型:", 
                                                 types, 0, false, &ok);
        if (ok) {
            AttributeType attrType = AttributeType::STRING;
            if (selected == "整数") attrType = AttributeType::INTEGER;
            else if (selected == "浮点数") attrType = AttributeType::FLOAT;
            
            if (itemTypeManager->addAttributeToType(typeName.toStdString(), 
                                                    attrName.toStdString(), attrType)) {
                QMessageBox::information(this, "成功", "属性添加成功！");
                refreshTypeList();
            } else {
                QMessageBox::warning(this, "错误", "属性添加失败！");
            }
        }
    }
}

void AdminDialog::modifyItemType() {
    // 保留向后兼容，重定向到添加属性
    addAttributeToType();
}

void AdminDialog::modifyAttributeInType() {
    QListWidgetItem *item = typeListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "警告", "请先选择一个物品类型！");
        return;
    }
    
    QString typeName = item->text().split(" [").first();
    ItemType* type = itemTypeManager->findItemType(typeName.toStdString());
    if (!type) {
        QMessageBox::warning(this, "错误", "找不到该物品类型！");
        return;
    }
    
    const vector<ItemTypeAttribute>& attrs = type->getAttributes();
    if (attrs.empty()) {
        QMessageBox::warning(this, "警告", "该类型没有属性！");
        return;
    }
    
    // 显示属性列表供选择
    QStringList attrList;
    for (const auto& attr : attrs) {
        QString typeStr;
        switch(attr.getType()) {
            case AttributeType::STRING: typeStr = "字符串"; break;
            case AttributeType::INTEGER: typeStr = "整数"; break;
            case AttributeType::FLOAT: typeStr = "浮点数"; break;
        }
        attrList << QString("%1 (%2)").arg(QString::fromStdString(attr.getName())).arg(typeStr);
    }
    
    bool ok;
    QString selected = QInputDialog::getItem(this, "选择要修改的属性", "属性:", 
                                             attrList, 0, false, &ok);
    if (!ok) return;
    
    int attrIndex = attrList.indexOf(selected);
    if (attrIndex < 0 || static_cast<unsigned int>(attrIndex) >= attrs.size()) {
        QMessageBox::warning(this, "错误", "无效的属性选择！");
        return;
    }
    
    // 获取新属性名称
    QString newAttrName = QInputDialog::getText(this, "修改属性", "请输入新属性名称:", 
                                                QLineEdit::Normal, 
                                                QString::fromStdString(attrs[attrIndex].getName()), 
                                                &ok);
    if (!ok || newAttrName.isEmpty()) return;
    
    // 获取新属性类型
    QStringList types;
    types << "字符串" << "整数" << "浮点数";
    QString selectedType = QInputDialog::getItem(this, "选择新属性类型", "属性类型:", 
                                                 types, 0, false, &ok);
    if (!ok) return;
    
    AttributeType attrType = AttributeType::STRING;
    if (selectedType == "整数") attrType = AttributeType::INTEGER;
    else if (selectedType == "浮点数") attrType = AttributeType::FLOAT;
    
    if (itemTypeManager->modifyAttributeInType(typeName.toStdString(), attrIndex,
                                               newAttrName.toStdString(), attrType)) {
        QMessageBox::information(this, "成功", "属性修改成功！");
        refreshTypeList();
    } else {
        QMessageBox::warning(this, "错误", "属性修改失败！");
    }
}

void AdminDialog::deleteAttributeFromType() {
    QListWidgetItem *item = typeListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "警告", "请先选择一个物品类型！");
        return;
    }
    
    QString typeName = item->text().split(" [").first();
    ItemType* type = itemTypeManager->findItemType(typeName.toStdString());
    if (!type) {
        QMessageBox::warning(this, "错误", "找不到该物品类型！");
        return;
    }
    
    const vector<ItemTypeAttribute>& attrs = type->getAttributes();
    if (attrs.empty()) {
        QMessageBox::warning(this, "警告", "该类型没有属性！");
        return;
    }
    
    // 显示属性列表供选择
    QStringList attrList;
    for (const auto& attr : attrs) {
        QString typeStr;
        switch(attr.getType()) {
            case AttributeType::STRING: typeStr = "字符串"; break;
            case AttributeType::INTEGER: typeStr = "整数"; break;
            case AttributeType::FLOAT: typeStr = "浮点数"; break;
        }
        attrList << QString("%1 (%2)").arg(QString::fromStdString(attr.getName())).arg(typeStr);
    }
    
    bool ok;
    QString selected = QInputDialog::getItem(this, "选择要删除的属性", "属性:", 
                                             attrList, 0, false, &ok);
    if (!ok) return;
    
    int attrIndex = attrList.indexOf(selected);
    if (attrIndex < 0 || static_cast<unsigned int>(attrIndex) >= attrs.size()) {
        QMessageBox::warning(this, "错误", "无效的属性选择！");
        return;
    }
    
    // 确认删除
    int ret = QMessageBox::question(this, "确认删除", 
                                    QString("确定要删除属性 \"%1\" 吗？").arg(selected),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (type->removeAttribute(attrIndex)) {
            QMessageBox::information(this, "成功", "属性删除成功！");
            refreshTypeList();
        } else {
            QMessageBox::warning(this, "错误", "属性删除失败！");
        }
    }
}

void AdminDialog::approveUsers() {
    QListWidgetItem *item = userListWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "警告", "请先选择一个用户！");
        return;
    }
    
    QString userInfo = item->text();
    QString username = userInfo.split(" [").first();
    
    if (userManager->approveUser(username.toStdString())) {
        QMessageBox::information(this, "成功", "用户批准成功！");
        refreshUserList();
    } else {
        QMessageBox::warning(this, "错误", "批准失败！用户可能不存在或已被批准。");
    }
}

// ==================== MainWindow 实现 ====================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentUser(nullptr)
{
    setupUI();
    refreshItemList();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("物品复活系统 - 库存管理系统");
    setMinimumSize(1000, 700);
    
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create widgets
    itemListWidget = new QListWidget(this);
    itemListWidget->setMinimumWidth(300);
    detailsTextEdit = new QTextBrowser(this);
    detailsTextEdit->setReadOnly(true);
    
    // Buttons
    loginButton = new QPushButton("登录/注册", this);
    logoutButton = new QPushButton("退出登录", this);
    addButton = new QPushButton("添加物品", this);
    deleteButton = new QPushButton("删除物品", this);
    searchButton = new QPushButton("搜索物品", this);
    refreshButton = new QPushButton("刷新列表", this);
    adminButton = new QPushButton("管理员功能", this);
    
    // Labels
    userInfoLabel = new QLabel("未登录", this);
    statusLabel = new QLabel("状态: 未登录", this);
    
    // Layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    // Left side - Item list and operations
    leftLayout->addWidget(new QLabel("物品列表:", this));
    leftLayout->addWidget(itemListWidget);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(refreshButton);
    leftLayout->addLayout(buttonLayout);
    
    // Right side - Item details and user info
    rightLayout->addWidget(userInfoLabel);
    rightLayout->addWidget(statusLabel);
    rightLayout->addWidget(new QLabel("物品详情:", this));
    rightLayout->addWidget(detailsTextEdit);
    
    QHBoxLayout *userButtonLayout = new QHBoxLayout();
    userButtonLayout->addWidget(loginButton);
    userButtonLayout->addWidget(logoutButton);
    userButtonLayout->addWidget(adminButton);
    rightLayout->addLayout(userButtonLayout);
    
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addLayout(rightLayout, 1);
    
    // Connect signals and slots
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::showLoginDialog);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::logout);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addItem);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteItem);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchItem);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshItemList);
    connect(adminButton, &QPushButton::clicked, this, &MainWindow::showAdminDialog);
    connect(itemListWidget, &QListWidget::itemClicked, this, &MainWindow::displayItemDetails);
    
    updateUIForUser();
}

void MainWindow::updateUIForUser() {
    if (currentUser == nullptr) {
        userInfoLabel->setText("未登录");
        statusLabel->setText("状态: 未登录");
        loginButton->setEnabled(true);
        logoutButton->setEnabled(false);
        addButton->setEnabled(false);
        deleteButton->setEnabled(false);
        searchButton->setEnabled(false);
        adminButton->setEnabled(false);
    } else {
        userInfoLabel->setText(QString("当前用户: %1 [%2]")
            .arg(QString::fromStdString(currentUser->getUsername()))
            .arg(currentUser->isAdmin() ? "管理员" : "普通用户"));
        statusLabel->setText("状态: 已登录");
        loginButton->setEnabled(false);
        logoutButton->setEnabled(true);
        addButton->setEnabled(true);
        deleteButton->setEnabled(true);
        searchButton->setEnabled(true);
        adminButton->setEnabled(currentUser->isAdmin());
    }
}

void MainWindow::showLoginDialog() {
    LoginDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.isRegisterMode()) {
            // 注册
            if (interface.getUserManager().registerUser(
                    dialog.getUsername().toStdString(),
                    dialog.getPassword().toStdString(),
                    dialog.getAddress().toStdString(),
                    dialog.getPhone().toStdString(),
                    dialog.getEmail().toStdString())) {
                QMessageBox::information(this, "成功", "注册成功！请等待管理员审批。");
            } else {
                QMessageBox::warning(this, "错误", "注册失败！用户名可能已存在。");
            }
        } else {
            // 登录
            interface.currentUser = interface.getUserManager().login(
                dialog.getUsername().toStdString(),
                dialog.getPassword().toStdString());
            
            if (interface.currentUser != nullptr) {
                if (interface.currentUser->isApproved()) {
                    currentUser = interface.currentUser;
                    QMessageBox::information(this, "成功", "登录成功！");
                    onLoginSuccess();
                } else {
                    QMessageBox::warning(this, "警告", "您的账户尚未被管理员批准，请等待审批。");
                    interface.currentUser = nullptr;
                }
            } else {
                QMessageBox::warning(this, "错误", "用户名或密码错误！");
            }
        }
    }
}

void MainWindow::onLoginSuccess() {
    updateUIForUser();
    refreshItemList();
}

void MainWindow::logout() {
    currentUser = nullptr;
    interface.currentUser = nullptr;
    updateUIForUser();
    detailsTextEdit->clear();
    refreshItemList();
    QMessageBox::information(this, "提示", "已退出登录。");
}

void MainWindow::addItem()
{
    if (currentUser == nullptr) {
        QMessageBox::warning(this, "警告", "请先登录！");
        return;
    }
    
    AddItemDialog dialog(&interface.getItemTypeManager(), currentUser, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getName();
        QString desc = dialog.getDescription();
        QString addr = dialog.getAddress();
        ItemType* type = dialog.getItemType();
        
        if (name.isEmpty()) {
            QMessageBox::warning(this, "警告", "物品名称不能为空！");
            return;
        }
        
        if (type == nullptr) {
            QMessageBox::warning(this, "警告", "请选择物品类型！");
            return;
        }
        
        Contacts contact(currentUser->getUsername(), currentUser->getPhone(), currentUser->getEmail());
        Item* item = new Item(name.toStdString(), desc.toStdString(), 
                             addr.toStdString(), contact, type->getTypeName());
        interface.getInventory().addItem(item);
        
        refreshItemList();
        QMessageBox::information(this, "成功", "物品添加成功！");
    }
}

void MainWindow::deleteItem()
{
    if (currentUser == nullptr) {
        QMessageBox::warning(this, "警告", "请先登录！");
        return;
    }
    
    QListWidgetItem *currentItem = itemListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "警告", "请选择要删除的物品！");
        return;
    }
    
    QString itemName = currentItem->text().split(" - ").first();
    if (interface.getInventory().deleteItem(itemName.toStdString())) {
        refreshItemList();
        detailsTextEdit->clear();
        QMessageBox::information(this, "成功", "物品删除成功！");
    } else {
        QMessageBox::warning(this, "错误", "删除失败！物品不存在。");
    }
}

void MainWindow::searchItem()
{
    if (currentUser == nullptr) {
        QMessageBox::warning(this, "警告", "请先登录！");
        return;
    }
    
    SearchDialog dialog(&interface.getItemTypeManager(), this);
    if (dialog.exec() == QDialog::Accepted) {
        QString keyword = dialog.getKeyword();
        ItemType* type = dialog.getItemType();
        
        if (keyword.isEmpty()) {
            QMessageBox::warning(this, "警告", "请输入搜索关键字！");
            return;
        }
        
        vector<int> results;
        if (type == nullptr) {
            // 搜索所有类型
            const vector<Item*>& items = interface.getInventory().getItems();
            for (unsigned int i = 0; i < items.size(); i++) {
                if (items[i] != nullptr) {
                    string itemName = items[i]->getName();
                    string description = items[i]->getDescription();
                    string contactName = items[i]->getContact().getName();
                    
                    // 转换为小写进行不区分大小写的匹配
                    string lowerKeyword = keyword.toStdString();
                    string lowerItemName = itemName;
                    string lowerDescription = description;
                    string lowerContactName = contactName;
                    
                    for (char& c : lowerKeyword) c = tolower(c);
                    for (char& c : lowerItemName) c = tolower(c);
                    for (char& c : lowerDescription) c = tolower(c);
                    for (char& c : lowerContactName) c = tolower(c);
                    
                    if (lowerItemName.find(lowerKeyword) != string::npos ||
                        lowerDescription.find(lowerKeyword) != string::npos ||
                        lowerContactName.find(lowerKeyword) != string::npos) {
                        results.push_back(i);
                    }
                }
            }
        } else {
            // 按类型搜索
            results = interface.getInventory().searchByTypeAndKeyword(
                type->getTypeName(), keyword.toStdString());
        }
        
        if (results.empty()) {
            QMessageBox::information(this, "搜索结果", "未找到匹配的物品。");
        } else {
            // 高亮显示搜索结果
            itemListWidget->clearSelection();
            for (int index : results) {
                if (index >= 0 && static_cast<unsigned int>(index) < interface.getInventory().getSize()) {
                    const vector<Item*>& items = interface.getInventory().getItems();
                    QString itemName = QString::fromStdString(items[index]->getName());
                    for (int i = 0; i < itemListWidget->count(); ++i) {
                        QListWidgetItem *item = itemListWidget->item(i);
                        if (item->text().startsWith(itemName)) {
                            item->setSelected(true);
                            itemListWidget->scrollToItem(item);
                            displayItemDetails(item);
                            break;
                        }
                    }
                }
            }
            QMessageBox::information(this, "搜索结果", 
                QString("找到 %1 个匹配的物品。").arg(results.size()));
        }
    }
}

void MainWindow::displayItemDetails(QListWidgetItem *item)
{
    if (!item || currentUser == nullptr) return;
    
    QString itemName = item->text().split(" - ").first();
    unsigned int index = interface.getInventory().findItem(itemName.toStdString());
    if (index != interface.getInventory().getSize()) {
        const vector<Item*>& items = interface.getInventory().getItems();
        Item* selectedItem = items[index];
        detailsTextEdit->setPlainText(formatItemDetails(selectedItem));
    }
}

QString MainWindow::formatItemDetails(Item* item) {
    if (!item) return "";
    
    QString details;
    QTextStream stream(&details);
    
    stream << "物品名称: " << QString::fromStdString(item->getName()) << "\n";
    stream << "物品说明: " << QString::fromStdString(item->getDescription()) << "\n";
    stream << "物品地址: " << QString::fromStdString(item->getAddress()) << "\n";
    
    QString typeName = QString::fromStdString(item->getTypeName());
    if (typeName.isEmpty()) {
        // 使用旧的类别系统
        switch(item->getCategory()) {
            case ItemCategory::FOOD: typeName = "食品"; break;
            case ItemCategory::BOOK: typeName = "书籍"; break;
            case ItemCategory::TOOL: typeName = "工具"; break;
            default: typeName = "其他"; break;
        }
    }
    stream << "物品类型: " << typeName << "\n";
    
    // 显示物品类型的属性信息
    if (!typeName.isEmpty() && !item->getTypeName().empty()) {
        ItemType* type = interface.getItemTypeManager().findItemType(item->getTypeName());
        if (type != nullptr) {
            const vector<ItemTypeAttribute>& attrs = type->getAttributes();
            if (!attrs.empty()) {
                stream << "\n类型属性:\n";
                for (const auto& attr : attrs) {
                    QString attrTypeStr;
                    switch(attr.getType()) {
                        case AttributeType::STRING: attrTypeStr = "字符串"; break;
                        case AttributeType::INTEGER: attrTypeStr = "整数"; break;
                        case AttributeType::FLOAT: attrTypeStr = "浮点数"; break;
                    }
                    stream << "  - " << QString::fromStdString(attr.getName()) 
                           << " (" << attrTypeStr << ")\n";
                }
            }
        }
    }
    
    Contacts& contact = item->getContact();
    stream << "\n联系人信息:\n";
    stream << "  姓名: " << QString::fromStdString(contact.getName()) << "\n";
    stream << "  电话: " << QString::fromStdString(contact.getNum()) << "\n";
    stream << "  邮箱: " << QString::fromStdString(contact.getEmail()) << "\n";
    
    return details;
}

void MainWindow::refreshItemList()
{
    itemListWidget->clear();
    const vector<Item*>& items = interface.getInventory().getItems();
    for (Item* item : items) {
        if (item != nullptr) {
            QString itemName = QString::fromStdString(item->getName());
            QString typeName = QString::fromStdString(item->getTypeName());
            if (typeName.isEmpty()) {
                switch(item->getCategory()) {
                    case ItemCategory::FOOD: typeName = "食品"; break;
                    case ItemCategory::BOOK: typeName = "书籍"; break;
                    case ItemCategory::TOOL: typeName = "工具"; break;
                    default: typeName = "其他"; break;
                }
            }
            itemListWidget->addItem(QString("%1 - [%2]").arg(itemName).arg(typeName));
        }
    }
}

void MainWindow::showAdminDialog()
{
    if (currentUser == nullptr || !currentUser->isAdmin()) {
        QMessageBox::warning(this, "警告", "您没有管理员权限！");
        return;
    }
    
    AdminDialog dialog(&interface.getItemTypeManager(), &interface.getUserManager(), this);
    dialog.exec();
}
