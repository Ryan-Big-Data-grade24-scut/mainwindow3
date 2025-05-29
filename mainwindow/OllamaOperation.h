#pragma once
#include <QObject>
#include <QPushButton>
#include "mainwindow.h"

class OllamaOperation : public QObject {
    Q_OBJECT
public:
    explicit OllamaOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : m_associatedButton(btn), windowPtr(window) {
    }

    virtual void processor() = 0;
    virtual QString name() const = 0;
    virtual void setButton() = 0;

    virtual void connectTrigger() = 0;
    QPushButton* button() const { return m_associatedButton; }
    virtual ~OllamaOperation() = default;

protected:
    QPushButton* m_associatedButton;
    MainWindow* windowPtr;
};