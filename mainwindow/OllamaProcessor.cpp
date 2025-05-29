#include "OllamaProcessor.h"
#include "mainwindow.h"
#include <QPushButton>

OllamaProcessor::OllamaProcessor(MainWindow* window)
    : QObject(window), windowPtr(window)
{
    initOperations(std::make_index_sequence<std::tuple_size_v<OllamaOperationTypes>>());
    connectAll();
}

void OllamaProcessor::connectAll() {
    for (auto& op : m_operations) {
        op->connectTrigger();  // ÿ������ʵ���Լ����ź������߼�
    }
}
