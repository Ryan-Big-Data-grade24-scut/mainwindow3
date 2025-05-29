#pragma once
#include "OllamaOperation.h"
#include <QListWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

class ShowFavoriteOperation : public OllamaOperation {
public:
    using OllamaOperation::OllamaOperation;

    QString name() const override { return "ShowFavorite"; }

    void setButton() override {
        // 不需要按钮，所以置空
        m_associatedButton = nullptr;
    }

    void connectTrigger() override {
        // 将 listWidget 的 itemClicked 事件连接到 processor()，转发功能
        QObject::connect(windowPtr->ui->listWidget_favorites, &QListWidget::itemClicked,
            [this](QListWidgetItem* item) {
                QString prompt = item->data(Qt::UserRole).toString();
                QString response = item->data(Qt::UserRole + 1).toString();
                windowPtr->ui->textEdit_result->setText("Q: " + prompt + "\n\nA: " + response);
            });
    }

    void processor() override {
        // 此函数此类中不会手动调用，留空或保留调试提示
    }
};


