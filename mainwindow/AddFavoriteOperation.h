#pragma once
#pragma once
#include "OllamaOperation.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
class AddFavoriteOperation : public OllamaOperation {
public:
    using OllamaOperation::OllamaOperation;
    QString name() const override { return "AddFavorite"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_addFavorite;
    }
    // ShowFavoriteOperation.h
    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override
    {
        int index = windowPtr->ui->listWidget_history->currentRow();
        if (index < 0 || index >= windowPtr->ollamaHistory.size()) {
            QMessageBox::warning(windowPtr, "提示", "请先选择一条历史记录。");
            return;
        }

        auto pair = windowPtr->ollamaHistory[index];
        for (const auto& existing : windowPtr->favoritesHistory) {
            if (existing.first == pair.first && existing.second == pair.second) {
                return;
            }
        }

        windowPtr->favoritesHistory.append(pair);
        auto* item = new QListWidgetItem(pair.first.left(30));
        item->setData(Qt::UserRole, pair.first);
        item->setData(Qt::UserRole + 1, pair.second);
        windowPtr->ui->listWidget_favorites->addItem(item);

        windowPtr->saveFavoritesHistory();
    }
};
