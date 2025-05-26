//ImgProcessor.h
#include <vector>
#include <memory>
#include <utility>
#include "ImgOpts.h"
#include "Opt_types.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

class ImageProcessor : public QObject {
    Q_OBJECT
public:
    // ����ʱ�Զ�װ�����й���
    explicit ImageProcessor(QObject* parent = nullptr, MainWindow* window = nullptr);

    // ������Ա���ֲ���...
    void connectAll();

signals:
    void operationCompleted(const QImage& result);

public:
    /*
    1. ģ���������ʽ��
        templete<>
        ����/��

    2. ����ģ�壺I...
        ��ϼ���
            make_index_sequence<size_t>():
                ����һ�� index_sequence����(I...�� 0, 1, 2, 3, ... �е��������)
            index_sequence<I...>���ܲ�����������ģ���Ӧ��ֵ
            (���ʽ(I), ...)���ʽ�� = (���ʽ(0), ���ʽ(1), ���ʽ(2), ...)

    3. ʹ��OperationTypes:
        tuple_element_t<���, OperationTypes>

    4. make_shared: ����ָ��
        (this)���ڹ�����������
        ��processorʵ�����ţ���Щָ��ͻ���
    */
    template<size_t... I>
    void initOperations(std::index_sequence<I...>) {
        ///if (!windowPtr->ui->pushButton_gaussian) {
           // QMessageBox::warning(windowPtr, "Initialize Warning", "ImageProcessor Successfully Created");
       // }
        (m_operations.push_back(
            std::make_shared<std::tuple_element_t<I, OperationTypes>>(nullptr, windowPtr)
        ), ...);
    }
    MainWindow* windowPtr;
    std::vector<std::shared_ptr<ImageOperation>> m_operations;
};