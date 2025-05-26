//ImgProcessor.cpp
#include "ImgProcessor.h"
#include "mainwindow.h"
#include <QPushButton>


// ���캯��
ImageProcessor::ImageProcessor(QObject* parent, MainWindow* window)
    : QObject(parent), windowPtr(window)
{
    //QMessageBox::warning(window, "Initialize Warning", "ImageProcessor Successfully Created");
    //cout << 1 << endl;
    // ʹ�ø�������չ�����й�������
    initOperations(std::make_index_sequence<std::tuple_size_v<OperationTypes>>());
}

// �������й��ߵİ�ť���ź�
void ImageProcessor::connectAll()
{
    //QMessageBox::warning(qApp->activeWindow(), "ConnectAll Check", "111");
    for (auto& op : m_operations) {
        //QMessageBox::warning(qApp->activeWindow(), "Connecting Check", op->name()+op->button()->objectName());
        if (op->button()) {
            // �����ť �� ִ�д���
            QObject::connect(op->button(), &QPushButton::clicked, [this, op]() {
                    
                    op->process();
                });
        }

        // ������� �� ֪ͨ������
        connect(op.get(), &ImageOperation::processed,
            this, &ImageProcessor::operationCompleted);
    }
}