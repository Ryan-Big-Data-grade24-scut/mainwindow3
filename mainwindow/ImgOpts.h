// ImageOperation.h
#pragma once
#include "operationbase.h"
#include <QImage>
#include <QApplication>  // ��������Խ��qAppδ����������
#include <opencv2/opencv.hpp>
#include <QMessageBox>

class ImageOperation : public OperationBase {
    Q_OBJECT
public:
    // ʹ�û���Ĺ��캯��
    using OperationBase::OperationBase;

    // �������ƣ����ִ��麯����
    virtual QString name() const override = 0;

    // ������Ƭ�ķ�����������processorΪprocess�Ա��ּ��ݣ�
    virtual void process() = 0;

    // ʵ�ֻ���processor�ӿڣ�ת����process��
    void processor() override { process(); }

    // ���ù�����ť�����麯����
    virtual void setButton() override = 0;

    // ���Ӵ����źţ����麯����
    virtual void connectTrigger() override = 0;

    // ���������߼�
    ~ImageOperation() override {
        if (!m_associatedButton) {
            QMessageBox::warning(qApp->activeWindow(), "Warning", "ImageOperation Destroyed");
        }
    }

signals:
    // ��������ź�
    void processed(const QImage& result);

protected:
    // ����������QImageתOpenCV��ʽ
    static cv::Mat qImageToMat(const QImage& image) {
        // ʵ�ִ���...
        cv::Mat mat;
        // ת���߼�...
        return mat;
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