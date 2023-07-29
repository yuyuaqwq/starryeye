#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwin.h"

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);
    ~MainWin();

    /// <summary>
    /// 往表格里添加项
    /// </summary>
    /// <param name="module_name">模块名称</param>
    /// <param name="module_type">模块类型</param>
    /// <param name="address">地址</param>
    void AppendModule(const QString& module_name, const QString& module_type, uint64_t address);

protected slots:
    void on_table_modules_customContextMenuRequested(const QPoint& pos);

private:
    Ui::MainWin ui;
};
