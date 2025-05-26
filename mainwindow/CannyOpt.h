// CannyOpt.h
#pragma once
#include "ImgOpts.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QMessageBox>


class CannyOperation : public ImageOperation {
    Q_OBJECT
public:
    // ���캯�������߸������Ǹ�ʲô����
    explicit CannyOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : ImageOperation(btn, window) {
        //QMessageBox::warning(window, "Initialize Warning", "BlurOperation Successfully Created");
        this->setButton();

    }
    void setButton() override
    {
        if (windowPtr && windowPtr->ui && windowPtr->ui->pushButton_canny) {
            this->m_associatedButton = windowPtr->ui->pushButton_canny;
            //QMessageBox::warning(windowPtr, "setButton", m_associatedButton->objectName());
        }
        else {
            qCritical() << "UI not correctly initialized! ";
            // ��ѡ����ʾ���ѺõĴ�����ʾ
            QMessageBox::warning(
                qApp->activeWindow(),  // ʹ�õ�ǰ�������Ϊ������
                "Initializatioin failed",
                "Can't relate to Buttun",
                QMessageBox::Ok
            );
        }
    }

    // �������ƣ���˹ģ��
    QString name() const override { return "GaussianBlur"; }

    // ���崦������ʵ��ģ��Ч��
    void process() override {
        //QMessageBox::warning(windowPtr, "Initialize Warning", "BlurOperation Successfully Created");
        //���û�й�����ť�;���
        if (!m_associatedButton) {
            qWarning() << "UI��ťδ����!";
            return;
        }
        if (windowPtr->image.empty()) {
            QMessageBox::warning(qApp->activeWindow(), "Error", "No image loaded!");
            return;
        }

        //// ʵ�ʴ�����루ʹ��OpenCV��
        cv::Mat mat;
        cv::Canny(windowPtr->image, mat, 100, 150, 3);
        QMessageBox::warning(qApp->activeWindow(), "Process", windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6")->objectName());
        windowPtr->display_MatInQT(windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6"), mat);
    }
};
#pragma once
