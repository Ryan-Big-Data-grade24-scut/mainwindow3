#pragma once
// imageoperation.h
#pragma once
#include "mainwindow.h"
#include <QObject>
#include <QImage>
#include <QPushButton>
#include <opencv2/opencv.hpp>
#include <QMessageBox>


class ImageOperation : public QObject {
    Q_OBJECT
public:
    // ���캯����ÿ�����߿��Թ���һ����ť
    explicit ImageOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : m_associatedButton(btn), windowPtr(window) {
    }

    // �������ƣ����麯��������������ʵ�֣�
    virtual QString name() const = 0;

    /*
    ��Ҫ1
    */
    // ������Ƭ�ķ��������麯����
    virtual void process() = 0;

    /*
    ��Ҫ2
    */
    // ��ȡ/���ù����İ�ť
    QPushButton* button() const { return m_associatedButton; }
    virtual void setButton() = 0;
    virtual ~ImageOperation() {
        if (!m_associatedButton) {
            QMessageBox::warning(qApp->activeWindow(), "Warning",
                "Destroyed");
        }
    }

signals:
    // �������ʱ�������źţ���"��"��һ����ʾ����
    void processed(const QImage& result);

protected:
    QPushButton* m_associatedButton; // ÿ�����߹�����UI��ť
    MainWindow* windowPtr; // ������ָ��MainWindow��ͼ���ָ��


    // ����������QImageתOpenCV��ʽ
    static cv::Mat qImageToMat(const QImage& image)
    {
    }

    // ����������OpenCVתQImage
    static QImage matToQImage(const cv::Mat& mat) {
        if (mat.type() == CV_8UC1) {
            QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
            image.setColorCount(256);
            for (int i = 0; i < 256; i++) image.setColor(i, qRgb(i, i, i));
            uchar* pSrc = mat.data;
            for (int row = 0; row < mat.rows; row++) {
                uchar* pDest = image.scanLine(row);
                memcpy(pDest, pSrc, mat.cols);
                pSrc += mat.step;
            }
            return image;
        }
        else if (mat.type() == CV_8UC3) {
            const uchar* pSrc = mat.data;
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped();
        }
        else if (mat.type() == CV_8UC4) {
            const uchar* pSrc = mat.data;
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            return image.copy();
        }
        return QImage();
    }
};