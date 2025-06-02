#pragma once
// effectCOpt.h
#pragma once
#include "ImgOpts.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

#include <QMessageBox>


class effectCOperation : public ImageOperation {
    Q_OBJECT
public:
    // ���캯�������߸������Ǹ�ʲô����
    explicit effectCOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : ImageOperation(btn, window) {
        //QMessageBox::warning(window, "Initialize Warning", "BlurOperation Successfully Created");
        this->setButton();

    }
    void setButton() override
    {
        if (windowPtr && windowPtr->ui && windowPtr->ui->pushButton_effectC) {
            this->m_associatedButton = windowPtr->ui->pushButton_effectC;
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
    QString name() const override { return "effectC"; }

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
        cv::Mat gray;
        cvtColor(windowPtr->image, gray, COLOR_BGR2GRAY);
        cv::Mat padded;
        int m = getOptimalDFTSize(gray.rows);
        int n = getOptimalDFTSize(gray.cols);
        copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, BORDER_CONSTANT, Scalar::all(0));

        cv::Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
        cv::Mat complexI;
        merge(planes, 2, complexI);

        dft(complexI, complexI);

        int cx = complexI.cols / 2;
        int cy = complexI.rows / 2;
        int radius = 30;

        Mat mask = Mat::zeros(complexI.size(), CV_32F);
        circle(mask, Point(cx, cy), radius, Scalar(1), -1);

        Mat channels[2];
        split(complexI, channels);
        channels[0] = channels[0].mul(mask);
        channels[1] = channels[1].mul(mask);
        merge(channels, 2, complexI);

        idft(complexI, complexI);
        split(complexI, channels);
        magnitude(channels[0], channels[1], channels[0]);

        normalize(channels[0], channels[0], 0, 255, NORM_MINMAX);
        channels[0].convertTo(channels[0], CV_8U);

        Mat result;
        cvtColor(channels[0], result, COLOR_GRAY2BGR);

        windowPtr->display_MatInQT(windowPtr->ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6"), result);
        return;
    }
    void connectTrigger() override {
        if (m_associatedButton) {
            connect(m_associatedButton, &QPushButton::clicked, this, &effectCOperation::process);
        }
    }
};