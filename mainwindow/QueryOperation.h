#pragma once
#pragma once
#include "SearchOperation.h"
#include "querylayer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class QueryOperation : public SearchOperation {
public:
    using SearchOperation::SearchOperation;
    QString name() const override { return "QueryOperation"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_query;
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
        if (query.isEmpty()) {
            windowPtr->ui->textBrowser_results->setText("请输入搜索内容。");
            return;
        }

        QueryLayer q;
        QVector<QueryLayer::SearchResult> results = q.search(query);

        QStringList resultList;
        for (const auto& r : results) {
            resultList << QString("标题: %1\n链接: %2\n摘要: %3")
                .arg(r.title)
                .arg(r.url)
                .arg(r.content);
        }

        windowPtr->ui->textBrowser_results->setText(resultList.join("\n\n"));
    }
};
