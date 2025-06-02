// OllamaOperation.h
#pragma once
#include "operationbase.h"

class OllamaOperation : public OperationBase {
    Q_OBJECT
public:
    // ʹ�û���Ĺ��캯��
    using OperationBase::OperationBase;

    // ����ԭ�еĴ��麯�����������override�ؼ��֣�
    virtual void processor() override = 0;
    virtual QString name() const override = 0;
    virtual void setButton() override = 0;
    virtual void connectTrigger() override = 0;

    // button() �����Ѵӻ���̳У������ظ�����

    // ������������ʡ�ԣ���Ϊ��������Ĭ��ʵ��
};