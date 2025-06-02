// OCROperation.h
#pragma once
#include "operationbase.h"

class OCROperation : public OperationBase {
public:
    // 使用基类的构造函数
    using OperationBase::OperationBase;

    // 工具名称（纯虚函数）
    virtual QString name() const override = 0;

    // 设置关联按钮（纯虚函数）
    virtual void setButton() override = 0;

    // 连接触发信号（纯虚函数）
    virtual void connectTrigger() override = 0;

    // 处理函数（纯虚函数）
    virtual void processor() override = 0;

    // 析构函数（保持空实现）
    virtual ~OCROperation() override = default;
};