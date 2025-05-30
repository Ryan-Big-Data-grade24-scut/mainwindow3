#include "SearchProcessor.h"
#include "mainwindow.h"

SearchProcessor::SearchProcessor(MainWindow* window)
    : QObject(window), windowPtr(window) {
    initOperations(std::make_index_sequence<std::tuple_size_v<SearchOperationTypes>>());
    connectAll();
}

void SearchProcessor::connectAll() {
    for (auto& op : m_operations) {
        op->connectTrigger();
    }
}
