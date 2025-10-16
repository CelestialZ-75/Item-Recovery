#include "mainwindow.h"
#include "inventory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    refreshItemList();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("Inventory Management System");
    setMinimumSize(800, 600);
    
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create widgets
    itemListWidget = new QListWidget(this);
    nameEdit = new QLineEdit(this);
    descEdit = new QLineEdit(this);
    contactNameEdit = new QLineEdit(this);
    contactNumEdit = new QLineEdit(this);
    detailsTextEdit = new QTextEdit(this);
    detailsTextEdit->setReadOnly(true);
    
    addButton = new QPushButton("Add Item", this);
    deleteButton = new QPushButton("Delete Item", this);
    searchButton = new QPushButton("Search", this);
    refreshButton = new QPushButton("Refresh", this);
    
    // Layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    // Left side - Item list and operations
    leftLayout->addWidget(new QLabel("Item List:"));
    leftLayout->addWidget(itemListWidget);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(refreshButton);
    leftLayout->addLayout(buttonLayout);
    
    // Right side - Item details and input
    rightLayout->addWidget(new QLabel("Item Details:"));
    rightLayout->addWidget(detailsTextEdit);
    
    rightLayout->addWidget(new QLabel("Add/Edit Item:"));
    rightLayout->addWidget(new QLabel("Name:"));
    rightLayout->addWidget(nameEdit);
    rightLayout->addWidget(new QLabel("Description:"));
    rightLayout->addWidget(descEdit);
    rightLayout->addWidget(new QLabel("Contact Name:"));
    rightLayout->addWidget(contactNameEdit);
    rightLayout->addWidget(new QLabel("Contact Number:"));
    rightLayout->addWidget(contactNumEdit);
    
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addLayout(rightLayout, 1);
    
    // Connect signals and slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addItem);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteItem);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchItem);
    connect(refreshButton, &QPushButton::clicked, this, &MainWindow::refreshItemList);
    connect(itemListWidget, &QListWidget::itemClicked, this, &MainWindow::displayItemDetails);
}

void MainWindow::addItem()
{
    QString name = nameEdit->text();
    QString desc = descEdit->text();
    QString contactName = contactNameEdit->text();
    QString contactNum = contactNumEdit->text();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Item name cannot be empty!");
        return;
    }
    
    Contacts contact(contactName.toStdString(), contactNum.toStdString());
    Item item(name.toStdString(), desc.toStdString(), contact);
    inventory.addItem(item);
    
    // Clear input fields
    nameEdit->clear();
    descEdit->clear();
    contactNameEdit->clear();
    contactNumEdit->clear();
    
    refreshItemList();
    QMessageBox::information(this, "Success", "Item added successfully!");
}

void MainWindow::deleteItem()
{
    QListWidgetItem *currentItem = itemListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warning", "Please select an item to delete!");
        return;
    }
    
    QString itemName = currentItem->text();
    if (inventory.deleteItem(itemName.toStdString())) {
        refreshItemList();
        detailsTextEdit->clear();
        QMessageBox::information(this, "Success", "Item deleted successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to delete item!");
    }
}

void MainWindow::searchItem()
{
    QString searchName = nameEdit->text();
    if (searchName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please enter item name to search!");
        return;
    }
    
    int index = inventory.findItem(searchName.toStdString());
    if (index != -1) {
        // Highlight the found item
        for (int i = 0; i < itemListWidget->count(); ++i) {
            QListWidgetItem *item = itemListWidget->item(i);
            if (item->text() == searchName) {
                itemListWidget->setCurrentItem(item);
                displayItemDetails(item);
                break;
            }
        }
    } else {
        QMessageBox::information(this, "Not Found", "Item not found!");
    }
}

void MainWindow::displayItemDetails(QListWidgetItem *item)
{
    if (!item) return;
    
    QString itemName = item->text();
    unsigned int index = inventory.findItem(itemName.toStdString());
    if (index != inventory.size()) {
        // 这里需要修改 Inventory 类以支持通过索引获取项目
        // inventory.printItem(index); // 需要实现类似功能
        detailsTextEdit->setPlainText(QString("Item: %1\nDetails displayed here...").arg(itemName));
    }
}

void MainWindow::refreshItemList()
{
    itemListWidget->clear();
    // 这里需要修改 Inventory 类以支持遍历项目
    // 暂时用示例数据
    // for (const auto& item : inventory.getItems()) {
    //     itemListWidget->addItem(QString::fromStdString(item.getName()));
    // }
}