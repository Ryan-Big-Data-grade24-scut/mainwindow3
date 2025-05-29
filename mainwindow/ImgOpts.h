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
    // 构造函数：每个工具可以关联一个按钮
    explicit ImageOperation(QPushButton* btn = nullptr, MainWindow* window = nullptr)
        : m_associatedButton(btn), windowPtr(window) {
    }

    // 工具名称（纯虚函数，必须由子类实现）
    virtual QString name() const = 0;

    /*
    重要1
    */
    // 处理照片的方法（纯虚函数）
    virtual void process() = 0;

    /*
    重要2
    */
    // 获取/设置关联的按钮
    QPushButton* button() const { return m_associatedButton; }
    virtual void setButton() = 0;
    virtual ~ImageOperation() {
        if (!m_associatedButton) {
            QMessageBox::warning(qApp->activeWindow(), "Warning",
                "Destroyed");
        }
    }

signals:
    // 处理完成时发出的信号（像"叮"的一声提示音）
    void processed(const QImage& result);

protected:
    QPushButton* m_associatedButton; // 每个工具关联的UI按钮
    MainWindow* windowPtr; // 新增：指向MainWindow中图像的指针


    // 辅助函数：QImage转OpenCV格式
    static cv::Mat qImageToMat(const QImage& image)
    {
    }

    // 辅助函数：OpenCV转QImage
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