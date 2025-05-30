#pragma once
#include "SearchOperation.h"
#include "utils.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class UtilityOperation : public SearchOperation {
public:
    using SearchOperation::SearchOperation;
    QString name() const override { return "UtilityOperation"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_util;
    }

    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override {
        QString content = windowPtr->ui->textBrowser_results->toPlainText().trimmed();
        if (content.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("当前无内容可清洗。");
            return;
        }

        Utils u;
        QString cleaned = u.cleanText(content);
        windowPtr->ui->textBrowser_results->setText("清洗后文本：\n" + cleaned);
    }
};