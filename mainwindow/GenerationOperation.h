#pragma once
#include "SearchOperation.h"
#include "generationlayer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class GenerationOperation : public SearchOperation {
public:
    using SearchOperation::SearchOperation;
    QString name() const override { return "GenerationOperation"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_generate;
    }

    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override {
        QString query = windowPtr->ui->lineEdit_search->text().trimmed();
        QString content = windowPtr->ui->textBrowser_results->toPlainText().trimmed();

        if (query.isEmpty() || content.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("请先输入查询内容并提取摘要。");
            return;
        }

        ContentLayer::ProcessedContent processed;
        processed.summary = content;
        processed.keyPoints = "N/A";      // 可选
        processed.sourceUrl = "N/A";      // 可选

        QVector<ContentLayer::ProcessedContent> contextVector{ processed };

        GenerationLayer g;
        QString answer = g.generateResponse(query, contextVector);

        windowPtr->ui->textBrowser_results->setText("AI生成结果：\n" + answer);
    }
};