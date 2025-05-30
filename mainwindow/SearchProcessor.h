#pragma once
#pragma once
#include <QObject>
#include <vector>
#include <memory>
#include <tuple>
#include "SearchOperation.h"
#include "SearchTypes.h"

class MainWindow;

class SearchProcessor : public QObject {
    Q_OBJECT
public:
    explicit SearchProcessor(MainWindow* window);

    void connectAll();

    template<size_t... I>
    void initOperations(std::index_sequence<I...>) {
        (m_operations.push_back(
            std::make_shared<std::tuple_element_t<I, SearchOperationTypes>>(nullptr, windowPtr)
        ), ...);
        for (auto& op : m_operations) op->setButton();
    }

private:
    MainWindow* windowPtr;
    std::vector<std::shared_ptr<SearchOperation>> m_operations;
};
