#pragma once
#include "SearchOperation.h"
#include "contentlayer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class ContentOperation : public SearchOperation {
public:
    using SearchOperation::SearchOperation;
    QString name() const override { return "ContentOperation"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_content;
    }

    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override {
        QString inputText = windowPtr->ui->textBrowser_results->toPlainText().trimmed();
        if (inputText.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("请先执行搜索操作以获取内容。");
            return;
        }

        // 构造一个假数据（在没有原始 QueryLayer::SearchResult 时使用）
        QueryLayer::SearchResult fake;
        fake.title = "本地内容";
        fake.url = "无";
        fake.content = inputText;

        QVector<QueryLayer::SearchResult> list{ fake };
        ContentLayer c;
        QVector<ContentLayer::ProcessedContent> processed = c.processResults(list);

        if (processed.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("摘要提取失败。");
            return;
        }

        const auto& result = processed[0];
        QString output = QString("摘要：\n%1\n\n关键点：\n%2\n\n来源：%3")
            .arg(result.summary)
            .arg(result.keyPoints)
            .arg(result.sourceUrl);

        windowPtr->ui->textBrowser_results->setText(output);
    }
};