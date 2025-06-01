#pragma once
#include <QObject>
#include <QPushButton>
#include "mainwindow.h"

class OCROperation {
public:
	explicit OCROperation(QPushButton* btn, MainWindow* window)
		: m_associatedButton(btn), windowPtr(window) {
	}
	virtual ~OCROperation() {}

	virtual QString name() const = 0;
	virtual void setButton() = 0;
	virtual void connectTrigger() = 0;
	virtual void processor() = 0;

protected:
	QPushButton* m_associatedButton;
	MainWindow* windowPtr;
};