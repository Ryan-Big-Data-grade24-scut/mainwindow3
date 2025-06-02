// OCROperation.h
#pragma once
#include "operationbase.h"

class OCROperation : public OperationBase {
public:
    // ʹ�û���Ĺ��캯��
    using OperationBase::OperationBase;

    // �������ƣ����麯����
    virtual QString name() const override = 0;

    // ���ù�����ť�����麯����
    virtual void setButton() override = 0;

    // ���Ӵ����źţ����麯����
    virtual void connectTrigger() override = 0;

    // �����������麯����
    virtual void processor() override = 0;

    // �������������ֿ�ʵ�֣�
    virtual ~OCROperation() override = default;
};