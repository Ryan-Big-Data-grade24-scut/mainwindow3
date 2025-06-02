#pragma once
#include <QObject>
#include <QPushButton>
//#include "mainwindow.h"
class MainWindow;
class OperationBase : public QObject {
    Q_OBJECT
public:
    explicit OperationBase(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : m_associatedButton(btn), windowPtr(window) {
    }

    virtual ~OperationBase() = default;

    // 纯虚函数 - 必须由子类实现
    virtual QString name() const = 0;
    virtual void processor() = 0;
    virtual void setButton() = 0;
    virtual void connectTrigger() = 0;

    // 获取关联按钮
    QPushButton* button() const { return m_associatedButton; }

protected:
    QPushButton* m_associatedButton;
    MainWindow* windowPtr;
};