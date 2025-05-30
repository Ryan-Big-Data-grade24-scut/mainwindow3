#pragma once
#include <QObject>
#include <QPushButton>
//#include "mainwindow.h"
class MainWindow;
class SearchOperation : public QObject {
    Q_OBJECT
public:
    explicit SearchOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : m_associatedButton(btn), windowPtr(window) {
    }

    virtual QString name() const = 0;
    virtual void processor() = 0;
    virtual void setButton() = 0;
    virtual void connectTrigger() = 0;
    virtual ~SearchOperation() = default;

protected:
    QPushButton* m_associatedButton;
    MainWindow* windowPtr;
};