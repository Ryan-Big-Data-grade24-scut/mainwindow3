#pragma once
#include "SearchOperation.h"
#include "QueryOperation.h"
#include "ContentOperation.h"
#include "GenerationOperation.h"
#include "UtilityOperation.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class FullSearchOperation : public SearchOperation {
public:
    using SearchOperation::SearchOperation;
    QString name() const override { return "FullSearchOperation"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_fullSearch;
    }

    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override {
        // 一、联网查询
        QueryLayer q;
        QString query = windowPtr->ui->lineEdit_search->text().trimmed();
        QVector<QueryLayer::SearchResult> rawResults = q.search(query);

        if (rawResults.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("未查询到任何结果。");
            return;
        }

        // 二、摘要提取
        ContentLayer c;
        QVector<ContentLayer::ProcessedContent> processed = c.processResults(rawResults);

        if (processed.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("内容摘要失败。");
            return;
        }

        // 三、AI生成
        GenerationLayer g;
        QString answer = g.generateResponse(query, processed);

        // 四、清洗文本
        Utils u;
        QString cleanedAnswer = u.cleanText(answer);

        // 展示最终结果
        QStringList lines;
        for (const auto& p : processed) {
            lines << QString("来源: %1\n摘要: %2\n关键点: %3")
                .arg(p.sourceUrl)
                .arg(p.summary)
                .arg(p.keyPoints);
        }

        lines << "\n【AI综合生成结果】\n" + cleanedAnswer;
        windowPtr->ui->textBrowser_results->setText(lines.join("\n\n"));
    }
};