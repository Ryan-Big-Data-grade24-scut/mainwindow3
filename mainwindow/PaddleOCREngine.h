#ifndef PADDLEOCRENGINE_H
#define PADDLEOCRENGINE_H

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")
#include <vector>
#include <string>
#include <stdexcept>
#include <QMessageBox>

class PaddleOCREngine {
public:
    // 构造函数：初始化Python环境
    PaddleOCREngine() {
        initialize_python();
        initialize_model();
    }

    // 析构函数：资源清理
    ~PaddleOCREngine() {
        cleanup();
    }

    // OCR处理主函数
    std::vector<std::string> process(const std::string& image_path) {
        PyObject* pFunc = PyObject_GetAttrString(pModule, "predict_image");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            PyErr_Print();
            throw std::runtime_error("predict_image function not found");
        }

        // 准备参数
        PyObject* pArgs = PyTuple_New(2);
        Py_INCREF(pModel);
        PyTuple_SetItem(pArgs, 0, pModel);
        PyTuple_SetItem(pArgs, 1, PyUnicode_FromString(image_path.c_str()));

        // 执行预测
        PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
        Py_DECREF(pFunc);
        Py_DECREF(pArgs);

        // 处理结果
        std::vector<std::string> texts;
        if (pResult && PyList_Check(pResult)) {
            for (Py_ssize_t i = 0; i < PyList_Size(pResult); i++) {
                PyObject* pItem = PyList_GetItem(pResult, i);
                if (!pItem) continue;

                if (PyUnicode_Check(pItem)) {
                    const char* text = PyUnicode_AsUTF8(pItem);
                    if (text) texts.push_back(text);
                }
            }
        }

        Py_XDECREF(pResult);
        return texts;
    }

    // 显示OCR结果（使用QMessageBox）
    void show_results(const std::vector<std::string>& texts) {
        QString resultText = "OCR Results:\n";
        for (const auto& text : texts) {
            resultText += QString::fromStdString(text) + " ";
        }

        QMessageBox::information(nullptr, "OCR Results", resultText);
    }

private:
    bool python_initialized;
    PyObject* pModule = nullptr;
    PyObject* pModel = nullptr;

    // 1. Python环境初始化
    void initialize_python() {
        if (!python_initialized) {

            Py_Initialize();
            PyRun_SimpleString("import sys, os");
            // 修改为你的实际Python环境路径
            PyRun_SimpleString("sys.path.append(r'C:\\\\Users\\\\c\\\\anaconda3\\\\envs\\\\paddle_env\\\\Lib\\\\site-packages')");
            PyRun_SimpleString("sys.path.append('.')");
            python_initialized = true;
        }
    }

    // 2. 模型初始化
    void initialize_model() {
        pModule = PyImport_ImportModule("paddleocr_wrapper");
        if (!pModule) {
            PyErr_Print();
            throw std::runtime_error("Failed to import paddleocr_wrapper");
        }

        PyObject* pFunc = PyObject_GetAttrString(pModule, "init_model");
        if (!pFunc || !PyCallable_Check(pFunc)) {
            PyErr_Print();
            throw std::runtime_error("init_model function not found");
        }

        pModel = PyObject_CallObject(pFunc, NULL);
        Py_DECREF(pFunc);

        if (!pModel) {
            PyErr_Print();
            throw std::runtime_error("Model initialization failed");
        }
    }

    // 3. 资源清理
    void cleanup() {
        Py_XDECREF(pModel);
        Py_XDECREF(pModule);
        if (python_initialized) {
            Py_Finalize();
            python_initialized = false;
        }
    }
};

#endif