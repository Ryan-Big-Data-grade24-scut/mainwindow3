#pragma once
// SearchOperation.h
#pragma once
#include "operationbase.h"

class SearchOperation : public OperationBase {
    Q_OBJECT
public:
    // 使用基类的构造函数
    using OperationBase::OperationBase;

    // 保持原有的纯虚函数声明
    virtual QString name() const override = 0;
    virtual void processor() override = 0;
    virtual void setButton() override = 0;
    virtual void connectTrigger() override = 0;

    // 析构函数可以省略，因为基类已有默认实现
};