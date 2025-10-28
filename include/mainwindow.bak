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
#include "inventory.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addItem();
    void deleteItem();
    void searchItem();
    void displayItemDetails(QListWidgetItem *item);
    void refreshItemList();

private:
    void setupUI();
    
    Inventory inventory;
    
    QListWidget *itemListWidget;
    QLineEdit *nameEdit;
    QLineEdit *descEdit;
    QLineEdit *contactNameEdit;
    QLineEdit *contactNumEdit;
    QTextEdit *detailsTextEdit;
    
    QPushButton *addButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *refreshButton;
};
#endif