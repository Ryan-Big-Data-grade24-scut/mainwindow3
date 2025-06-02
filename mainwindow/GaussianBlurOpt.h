// GaussianBlurOpt.h
#pragma once
#include "ImgOpts.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QMessageBox>


class BlurOperation : public ImageOperation {
    Q_OBJECT
public:
    // 构造函数：告诉父类这是个什么工具
    explicit BlurOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : ImageOperation(btn, window) {
        //QMessageBox::warning(window, "Initialize Warning", "BlurOperation Successfully Created");
        this->setButton();

    }
    void setButton() override
    {
        if (windowPtr && windowPtr->ui && windowPtr->ui->pushButton_gaussian) {
            this->m_associatedButton = windowPtr->ui->pushButton_gaussian;
            //QMessageBox::warning(windowPtr, "setButton", m_associatedButton->objectName());
        }
        else {
            qCritical() << "UI not correctly initialized! ";
            // 可选：显示更友好的错误提示
            QMessageBox::warning(
                qApp->activeWindow(),  // 使用当前活动窗口作为父窗口
                "Initializatioin failed",
                "Can't relate to Buttun",
                QMessageBox::Ok
            );
        }
    }

    // 工具名称：高斯模糊
    QString name() const override { return "GaussianBlur"; }

    // 具体处理方法：实现模糊效果
    void process() override {
        //QMessageBox::warning(windowPtr, "Initialize Warning", "BlurOperation Successfully Created");
        //如果没有关联按钮就警告
        if (!m_associatedButton) {
            qWarning() << "UI按钮未连接!";
            return;
        }
        if (windowPtr->image.empty()) {
            QMessageBox::warning(qApp->activeWindow(), "Error", "No image loaded!");
            return;
        }

        //// 实际处理代码（使用OpenCV）
        cv::Mat mat;
        cv::GaussianBlur(windowPtr->image, mat, cv::Size(5, 5), 0);
        //QMessageBox::warning(qApp->activeWindow(), "Process", windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6")->objectName());
        windowPtr->display_MatInQT(windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6"), mat);
        return;
    }
    void connectTrigger() override {
        if (m_associatedButton) {
            connect(m_associatedButton, &QPushButton::clicked, this, &BlurOperation::process);
        }
    }
};
