// OllamaOperation.h
#pragma once
#include "operationbase.h"

class OllamaOperation : public OperationBase {
    Q_OBJECT
public:
    // 使用基类的构造函数
    using OperationBase::OperationBase;

    // 保持原有的纯虚函数声明（添加override关键字）
    virtual void processor() override = 0;
    virtual QString name() const override = 0;
    virtual void setButton() override = 0;
    virtual void connectTrigger() override = 0;

    // button() 方法已从基类继承，无需重复声明

    // 析构函数可以省略，因为基类已有默认实现
};