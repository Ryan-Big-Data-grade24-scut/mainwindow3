#pragma once
#pragma once
#include "OllamaOperation.h"

class ShowHistoryOperation : public OllamaOperation {
public:
    using OllamaOperation::OllamaOperation;
    QString name() const override { return "ShowHistory"; }

    void setButton() override {
        m_associatedButton = nullptr;
    }

    void connectTrigger() override {
        QObject::connect(windowPtr->ui->listWidget_history, &QListWidget::itemClicked,
            [this](QListWidgetItem* item) {
                int index = windowPtr->ui->listWidget_history->row(item);
                if (index >= 0 && index < windowPtr->ollamaHistory.size()) {
                    QString q = windowPtr->ollamaHistory[index].first;
                    QString a = windowPtr->ollamaHistory[index].second;
                    windowPtr->ui->textEdit_result->setText("Q: " + q + "\n\nA: " + a);
                }
            });
    }

    void processor() override {}  // ø’ µœ÷º¥ø…
};



