#pragma once
#include <QMessageBox>
#include <QListWidgetItem>
#include "OllamaOperation.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

class RemoveFavoriteOperation : public OllamaOperation {
public:
    using OllamaOperation::OllamaOperation;
    QString name() const override { return "RemoveFavorite"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_removeFavorite;
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
        int index = windowPtr->ui->listWidget_favorites->currentRow();
        if (index < 0 || index >= windowPtr->ui->listWidget_favorites->count()) return;

        QListWidgetItem* item = windowPtr->ui->listWidget_favorites->item(index);
        QString prompt = item->data(Qt::UserRole).toString();
        QString response = item->data(Qt::UserRole + 1).toString();

        auto confirm = QMessageBox::question(
            windowPtr,
            tr("确认移除收藏"),
            tr("你确定要移除这条收藏记录吗？\n\nPrompt:\n%1\n\nResponse:\n%2")  // 确保字符串无直接换行
            .arg(prompt.left(100))
            .arg(response.left(100)),
            QMessageBox::Yes | QMessageBox::No
        );

        if (confirm != QMessageBox::Yes)
            return;




        if (confirm != QMessageBox::Yes) return;

        auto& fav = windowPtr->favoritesHistory;
        fav.erase(std::remove_if(fav.begin(), fav.end(), [&](const QPair<QString, QString>& pair) {
            return pair.first == prompt && pair.second == response;
            }), fav.end());

        delete windowPtr->ui->listWidget_favorites->takeItem(index);
        windowPtr->saveFavoritesHistory();
    }
};
