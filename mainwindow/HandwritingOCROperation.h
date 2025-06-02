#pragma once
#include "OCROperation.h"
#include "PaddleOCREngine.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextEdit>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

class HandwritingOCROperation : public OCROperation {
public:
    using OCROperation::OCROperation;
    PaddleOCREngine* engine = nullptr;
    HandwritingOCROperation(QPushButton* btn, MainWindow* window): OCROperation(btn, window)
    {
        engine = new PaddleOCREngine();
    }

    QString name() const override { return "HandwritingOCR"; }

    void setButton() override {
        m_associatedButton = windowPtr->ui->pushButton_ocr;
    }

    void connectTrigger() override {
        if (m_associatedButton) {
            QObject::connect(m_associatedButton, &QPushButton::clicked, [this]() {
                this->processor();
                });
        }
    }

    void processor() override {
        try {
            if (!windowPtr->image.data) {
                QMessageBox::warning(windowPtr, "警告", "请先上传图像！");
                return;
            }

            QString tempPath = "temp_ocr.jpg";
            cv::imwrite(tempPath.toStdString(), windowPtr->image);

            
            auto results = engine->process(tempPath.toStdString());

            QString output;
            for (const auto& line : results) {
                output += QString::fromStdString(line) + "\n";
            }

            windowPtr->ui->textEdit->setPlainText(output);

            QFile::remove(tempPath);  // 可选：删除临时文件
        }
        catch (const std::exception& e) {
            QMessageBox::critical(windowPtr, "OCR 错误", e.what());
        }
    }
};
