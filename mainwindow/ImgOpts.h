// ImageOperation.h
#pragma once
#include "operationbase.h"
#include <QImage>
#include <QApplication>  // 添加这行以解决qApp未声明的问题
#include <opencv2/opencv.hpp>
#include <QMessageBox>

class ImageOperation : public OperationBase {
    Q_OBJECT
public:
    // 使用基类的构造函数
    using OperationBase::OperationBase;

    // 工具名称（保持纯虚函数）
    virtual QString name() const override = 0;

    // 处理照片的方法（重命名processor为process以保持兼容）
    virtual void process() = 0;

    // 实现基类processor接口（转发到process）
    void processor() override { process(); }

    // 设置关联按钮（纯虚函数）
    virtual void setButton() override = 0;

    // 连接触发信号（纯虚函数）
    virtual void connectTrigger() override = 0;

    // 特殊析构逻辑
    ~ImageOperation() override {
        if (!m_associatedButton) {
            QMessageBox::warning(qApp->activeWindow(), "Warning", "ImageOperation Destroyed");
        }
    }

signals:
    // 处理完成信号
    void processed(const QImage& result);

protected:
    // 辅助函数：QImage转OpenCV格式
    static cv::Mat qImageToMat(const QImage& image) {
        // 实现代码...
        cv::Mat mat;
        // 转换逻辑...
        return mat;
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