#pragma once
#include "OllamaOperation.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
class ShowImageOnPage4Operation : public OllamaOperation {
public:
    using OllamaOperation::OllamaOperation;
    QString name() const override { return "ShowImageOnPage4"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_toPage4;
    }
    // ShowFavoriteOperation.h
    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override {
        QLabel* label = windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6");
        if (label && windowPtr->image.data) {
            windowPtr->display_MatInQT(label, windowPtr->image);
        }
        windowPtr->ui->stackedWidget->setCurrentIndex(3);
    }
};
