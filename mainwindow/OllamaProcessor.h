#pragma once
#include <QObject>
#include <vector>
#include <memory>
#include <tuple>
#include <utility>
//#include "mainwindow.h"
#include "OllamaOperation.h"
#include "OllamaTypes.h"
class MainWindow;
class OllamaProcessor : public QObject {
    Q_OBJECT
public:
    explicit OllamaProcessor(MainWindow* window);

    void connectAll();

    template<size_t... I>
    void initOperations(std::index_sequence<I...>) {
        (m_operations.push_back(
            std::make_shared<std::tuple_element_t<I, OllamaOperationTypes>>(nullptr, windowPtr)
        ), ...);
        for (auto& op : m_operations) op->setButton();
    }

private:
    MainWindow* windowPtr;
    std::vector<std::shared_ptr<OllamaOperation>> m_operations;
};