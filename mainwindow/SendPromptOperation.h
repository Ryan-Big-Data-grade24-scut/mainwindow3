#pragma once
#pragma once
#include "OllamaOperation.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include<ui_mainwindow.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ImgProcessor.h"
#include <QMessageBox>
#include <QScrollBar>

class SendPromptOperation : public OllamaOperation {
public:
    using OllamaOperation::OllamaOperation;
    QString name() const override { return "SendPrompt"; }

    void setButton() override {
        //QMessageBox::warning(windowPtr, "setButton", "sendPrompt Successfully Created");
        m_associatedButton = windowPtr->ui->pushButton_ollama;
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
        //QMessageBox::warning(windowPtr, "process", "sendPrompt start process");
        QString prompt = windowPtr->ui->lineEdit_prompt->text().trimmed();
        if (prompt.isEmpty()) {
            QMessageBox::warning(windowPtr, "警告", "提示词不能为空！");
            return;
        }

        QUrl url("http://localhost:11434/api/generate");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json.insert("model", "deepseek-r1:1.5b");
        json.insert("prompt", prompt);
        json.insert("max_tokens", 150);

        QNetworkReply* reply = windowPtr->networkManager->post(request, QJsonDocument(json).toJson());

        QObject::connect(reply, &QNetworkReply::finished, windowPtr, [this, reply, prompt]() {
            QString result;
            if (reply->error() == QNetworkReply::NoError) {
                result = windowPtr->parseOllamaResponse(reply->readAll());
            }
            else {
                windowPtr->ui->textEdit_result->setText("网络错误: " + reply->errorString());
                return;
            }

            QStringList lines = result.split("\n");
            QString finalResult;
            for (const QString& line : lines) {
                QJsonParseError error;
                QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &error);
                if (error.error == QJsonParseError::NoError && doc.isObject()) {
                    auto obj = doc.object();
                    if (obj.contains("response") && obj.contains("done")) {
                        finalResult += obj["response"].toString();
                        if (obj["done"].toBool()) break;
                    }
                }
            }

            windowPtr->ui->textEdit_result->setText(finalResult);
            windowPtr->ollamaHistory.append(qMakePair(prompt, finalResult));
            windowPtr->ui->listWidget_history->addItem(prompt.left(30));
            windowPtr->ui->lineEdit_prompt->clear();
            });
        //QMessageBox::warning(windowPtr, "process", "sendPrompt Successfully Created");
    }
};
