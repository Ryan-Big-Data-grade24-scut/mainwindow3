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
    // 构造时自动装入所有工具
    explicit ImageProcessor(QObject* parent = nullptr, MainWindow* window = nullptr);

    // 其他成员保持不变...
    void connectAll();

signals:
    void operationCompleted(const QImage& result);

public:
    /*
    1. 模板的声明方式：
        templete<>
        函数/类

    2. 特殊模板：I...
        组合技：
            make_index_sequence<size_t>():
                生成一个 index_sequence对象(I...≈ 0, 1, 2, 3, ... 有点像迭代器)
            index_sequence<I...>接受参数，并赋予模板对应的值
            (表达式(I), ...)这个式子 = (表达式(0), 表达式(1), 表达式(2), ...)

    3. 使用OperationTypes:
        tuple_element_t<序号, OperationTypes>

    4. make_shared: 智能指针
        (this)用于管理生命周期
        即processor实例活着，这些指针就活着
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