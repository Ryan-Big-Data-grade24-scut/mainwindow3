#pragma once
#pragma once
#include <QObject>
#include <vector>
#include <memory>
#include <tuple>
#include <utility>
#include "OCROperation.h"
#include "OCROperationTypes.h"
#include "PaddleOCREngine.h"  // ÃÌº”’‚––


class MainWindow;

class OCRProcessor : public QObject {
	Q_OBJECT
public:
	explicit OCRProcessor(MainWindow* window) : QObject(window), windowPtr(window) {
		initOperations(std::make_index_sequence<std::tuple_size_v<OCROperationTypes>>{});
	}

	void connectAll() {
		for (auto& op : m_operations) op->connectTrigger();
	}

private:
	MainWindow* windowPtr;
	std::vector<std::shared_ptr<OCROperation>> m_operations;

	template <size_t... I>
	void initOperations(std::index_sequence<I...>) {
		(m_operations.push_back(std::make_shared<std::tuple_element_t<I, OCROperationTypes>>(nullptr, windowPtr)), ...);
		for (auto& op : m_operations) op->setButton();
	}
};