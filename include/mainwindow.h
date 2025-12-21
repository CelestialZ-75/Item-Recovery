#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QTextBrowser>
#include <QTabWidget>
#include <QInputDialog>
#include <QTextStream>
#include <QCloseEvent>
#include "inventory.h"
#include "interface.h"
#include "itemtype.h"
#include "user.h"

// 登录对话框
class LoginDialog : public QDialog {
    Q_OBJECT
public:
    LoginDialog(QWidget *parent = nullptr);
    QString getUsername() const { return usernameEdit->text(); }
    QString getPassword() const { return passwordEdit->text(); }
    QString getAddress() const { return addressEdit->text(); }
    QString getPhone() const { return phoneEdit->text(); }
    QString getEmail() const { return emailEdit->text(); }
    bool isRegisterMode() const { return registerMode; }
    
private slots:
    void switchToRegister();
    void switchToLogin();
    void accept() override;
    
private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *addressEdit;
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
    QPushButton *switchButton;
    QLabel *titleLabel;
    QFormLayout *formLayout;
    bool registerMode;
    void setupUI();
};

// 物品类型选择对话框
class ItemTypeDialog : public QDialog {
    Q_OBJECT
public:
    ItemTypeDialog(ItemTypeManager* manager, QWidget *parent = nullptr);
    ItemType* getSelectedType() const;
    
private:
    QComboBox *typeComboBox;
    ItemTypeManager* itemTypeManager;
    vector<ItemType*> types;
};

// 添加物品对话框
class AddItemDialog : public QDialog {
    Q_OBJECT
public:
    AddItemDialog(ItemTypeManager* manager, User* currentUser, QWidget *parent = nullptr);
    QString getName() const { return nameEdit->text(); }
    QString getDescription() const { return descEdit->text(); }
    QString getAddress() const { return addressEdit->text(); }
    ItemType* getItemType() const;
    
private:
    QLineEdit *nameEdit;
    QLineEdit *descEdit;
    QLineEdit *addressEdit;
    QComboBox *typeComboBox;
    ItemTypeManager* itemTypeManager;
    vector<ItemType*> types;
};

// 搜索对话框
class SearchDialog : public QDialog {
    Q_OBJECT
public:
    SearchDialog(ItemTypeManager* manager, QWidget *parent = nullptr);
    QString getKeyword() const { return keywordEdit->text(); }
    ItemType* getItemType() const;
    
private:
    QLineEdit *keywordEdit;
    QComboBox *typeComboBox;
    ItemTypeManager* itemTypeManager;
    vector<ItemType*> types;
};

// 管理员功能对话框
class AdminDialog : public QDialog {
    Q_OBJECT
public:
    AdminDialog(ItemTypeManager* manager, UserManager* userManager, Interface* interface, QWidget *parent = nullptr);
    
private slots:
    void approveUsers();
    void addItemType();
    void modifyItemType();
    void addAttributeToType();
    void modifyAttributeInType();
    void deleteAttributeFromType();
    
private:
    ItemTypeManager* itemTypeManager;
    UserManager* userManager;
    Interface* interface;
    QListWidget *typeListWidget;
    QListWidget *userListWidget;
    void refreshTypeList();
    void refreshUserList();
};

// 主窗口
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event) override;

private slots:
    void showLoginDialog();
    void onLoginSuccess();
    void addItem();
    void deleteItem();
    void searchItem();
    void displayItemDetails(QListWidgetItem *item);
    void refreshItemList();
    void logout();
    void showAdminDialog();

private:
    void setupUI();
    void updateUIForUser();
    QString formatItemDetails(Item* item);
    
    Interface interface;
    User* currentUser;
    
    // UI 组件
    QListWidget *itemListWidget;
    QTextBrowser *detailsTextEdit;
    
    QPushButton *loginButton;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *refreshButton;
    QPushButton *logoutButton;
    QPushButton *adminButton;
    
    QLabel *userInfoLabel;
    QLabel *statusLabel;
};

#endif

