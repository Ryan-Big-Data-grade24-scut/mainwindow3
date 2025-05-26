//ImgProcessor.cpp
#include "ImgProcessor.h"
#include "mainwindow.h"
#include <QPushButton>


// 构造函数
ImageProcessor::ImageProcessor(QObject* parent, MainWindow* window)
    : QObject(parent), windowPtr(window)
{
    //QMessageBox::warning(window, "Initialize Warning", "ImageProcessor Successfully Created");
    //cout << 1 << endl;
    // 使用辅助函数展开所有工具类型
    initOperations(std::make_index_sequence<std::tuple_size_v<OperationTypes>>());
}

// 连接所有工具的按钮和信号
void ImageProcessor::connectAll()
{
    //QMessageBox::warning(qApp->activeWindow(), "ConnectAll Check", "111");
    for (auto& op : m_operations) {
        //QMessageBox::warning(qApp->activeWindow(), "Connecting Check", op->name()+op->button()->objectName());
        if (op->button()) {
            // 点击按钮 → 执行处理
            QObject::connect(op->button(), &QPushButton::clicked, [this, op]() {
                    
                    op->process();
                });
        }

        // 处理完成 → 通知主窗口
        connect(op.get(), &ImageOperation::processed,
            this, &ImageProcessor::operationCompleted);
    }
}