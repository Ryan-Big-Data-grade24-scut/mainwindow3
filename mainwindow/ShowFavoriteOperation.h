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
        // ����Ҫ��ť�������ÿ�
        m_associatedButton = nullptr;
    }

    void connectTrigger() override {
        // �� listWidget �� itemClicked �¼����ӵ� processor()��ת������
        QObject::connect(windowPtr->ui->listWidget_favorites, &QListWidget::itemClicked,
            [this](QListWidgetItem* item) {
                QString prompt = item->data(Qt::UserRole).toString();
                QString response = item->data(Qt::UserRole + 1).toString();
                windowPtr->ui->textEdit_result->setText("Q: " + prompt + "\n\nA: " + response);
            });
    }

    void processor() override {
        // �˺��������в����ֶ����ã����ջ���������ʾ
    }
};


