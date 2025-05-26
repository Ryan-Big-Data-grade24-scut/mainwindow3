// GaussianBlurOpt.h
#pragma once
#include "ImgOpts.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QMessageBox>


class GrayOperation : public ImageOperation {
    Q_OBJECT
public:
    // ���캯�������߸������Ǹ�ʲô����
    explicit GrayOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : ImageOperation(btn, window) {
        //QMessageBox::warning(window, "Initialize Warning", "GrayOperation Successfully Created");
        this->setButton();

    }
    void setButton() override
    {
        if (windowPtr && windowPtr->ui && windowPtr->ui->pushButton_gaussian) {
            this->m_associatedButton = windowPtr->ui->pushButton_gray;
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


    // �������ƣ��Ҷ�ͼ
    QString name() const override { return "Gray"; }

    // ���崦�����������
    void process() override {
        //QMessageBox::warning(windowPtr, "Initialize Warning", "GrayOperation Successfully Created");
        // ���û�й�����ť�;���
        if (!m_associatedButton) {
            qWarning() << "UI��ťδ����!";
            return;
        }

        // ʵ�ʴ�����루ʹ��OpenCV��
        cv::Mat mat;
        cv::cvtColor(windowPtr->image, mat, COLOR_BGR2GRAY);
        //QMessageBox::warning(qApp->activeWindow(), "Process", windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6")->objectName());
        windowPtr->display_MatInQT(windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6"), mat);
        return;
    }
};
