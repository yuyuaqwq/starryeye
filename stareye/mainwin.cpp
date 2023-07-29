#include "stdafx.h"
#include "mainwin.h"

MainWin::MainWin(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    AppendModule("System", "Process", 0x114514);
    AppendModule("我是傻逼", "Process", 0x1919810);
    AppendModule("每秒运行114514亿次", "Thread", 0x6666666);
}

MainWin::~MainWin()
{}


void MainWin::AppendModule(const QString & module_name, const QString & module_type, uint64_t address)
{
    auto row_count = ui.table_modules->rowCount();
    ui.table_modules->setRowCount(row_count + 1);

    auto header_item = new QTableWidgetItem(QString::number(row_count + 1));
    header_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui.table_modules->setVerticalHeaderItem(row_count, header_item);

    auto module_name_item = new QTableWidgetItem(module_name);
    module_name_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui.table_modules->setItem(row_count, 0, module_name_item);

    auto module_type_item = new QTableWidgetItem(module_type);
    module_type_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui.table_modules->setItem(row_count, 1, module_type_item);

    auto address_item = new QTableWidgetItem(QString::number(address, 16));
    address_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui.table_modules->setItem(row_count, 2, address_item);
}


void MainWin::on_table_modules_customContextMenuRequested(const QPoint& pos)
{

}
