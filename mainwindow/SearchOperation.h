#pragma once
// SearchOperation.h
#pragma once
#include "operationbase.h"

class SearchOperation : public OperationBase {
    Q_OBJECT
public:
    // ʹ�û���Ĺ��캯��
    using OperationBase::OperationBase;

    // ����ԭ�еĴ��麯������
    virtual QString name() const override = 0;
    virtual void processor() override = 0;
    virtual void setButton() override = 0;
    virtual void connectTrigger() override = 0;

    // ������������ʡ�ԣ���Ϊ��������Ĭ��ʵ��
};