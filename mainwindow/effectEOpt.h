#pragma once
// effectEOpt.h
#pragma once
#include "ImgOpts.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QMessageBox>


class effectEOperation : public ImageOperation {
    Q_OBJECT
public:
    // ���캯�������߸������Ǹ�ʲô����
    explicit effectEOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : ImageOperation(btn, window) {
        //QMessageBox::warning(window, "Initialize Warning", "BlurOperation Successfully Created");
        this->setButton();

    }
    void setButton() override
    {
        if (windowPtr && windowPtr->ui && windowPtr->ui->pushButton_effectE) {
            this->m_associatedButton = windowPtr->ui->pushButton_effectE;
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
    QString name() const override { return "effectE"; }

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
        cv::Rect rect(windowPtr->image.cols / 4, windowPtr->image.rows / 4, windowPtr->image.cols / 2, windowPtr->image.rows / 2);
        cv::Mat mask = Mat::zeros(windowPtr->image.size(), CV_8UC1);
        cv::Mat bgModel, fgModel;

        grabCut(windowPtr->image, mask, rect, bgModel, fgModel, 5, GC_INIT_WITH_RECT);
        compare(mask, GC_PR_FGD, mask, CMP_EQ);
        cv::Mat foreground(windowPtr->image.size(), CV_8UC3, Scalar(0, 0, 0));
        windowPtr->image.copyTo(foreground, mask);

        cv::Mat blended;
        addWeighted(windowPtr->image, 0.3, foreground, 0.7, 0, blended);

        windowPtr->display_MatInQT(windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6"), blended);
        return;
    }
};