#pragma once


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QInputDialog>
#include <opencv2/opencv.hpp>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QApplication>
#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QEventLoop>
#include <QUrlQuery>  // 必须包含此头文件
#include <QUrl>       // QUrl 也需要
#include <QThread>
#include <memory>
//#include "SearchProcessor.h"
//#include "ImgProcessor.h"
class ImageProcessor;
class OllamaProcessor;
class SearchProcessor;
class OCRProcessor;
using namespace cv;

namespace Ui {
    class mainwindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Mat image, mat_Gaussian, gray;
    void display_MatInQT(QLabel* label, const cv::Mat& mat);
    Ui::mainwindowClass* ui;
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_history_clicked();
    void on_esc_pressed();
    //void startOllamaInteraction();
    //void on_historyItemClicked(QListWidgetItem* item);

    void on_pushButton_toPage4_clicked();

    //void on_pushButton_addFavorite_clicked();
    //void on_pushButton_removeFavorite_clicked();
    //void on_favoriteItemClicked(QListWidgetItem* item); // 可选，用于点击显示内容
    

    


    //void on_pushButton_search_clicked();  // 搜索按钮点击事件
    //void handleSearchReply(QNetworkReply* reply);  // 处理搜索回复


public:
    QImage MatToQImage(const cv::Mat& mat);
    bool eventFilter(QObject* obj, QEvent* event);
    QString parseOllamaResponse(const QByteArray& data);

    void saveImageHistory();        // 保存历史路径到文件
    void loadImageHistory();        // 从文件加载历史路径

    void saveOllamaHistory();
    void loadOllamaHistory();

    void saveFavoritesHistory();
    void loadFavoritesHistory();

    QVector<QPair<QString, QString>> favoritesHistory;
    

    // 在 private 部分添加方法：
    //void performWebSearch(const QString& query);  // 执行搜索
    //QString formatSearchResults(const QJsonArray& results);  // 格式化搜索结果
    //QString generateRAGPrompt(const QJsonArray& results, const QString& question);  // 生成RAG提示
    //QString ragWithOllamaAndSearXNG(const QString& question);  // RAG流程集成
    //QString callOllama(const QString& prompt);


public:


    std::vector<Mat> history;
    int historyIndex;

    QNetworkAccessManager* networkManager;
    QEventLoop loop;

    QVector<QPair<QString, QString>> ollamaHistory;

    QStringList imagePathHistory;
    const QString historyFilePath = "image_history.json";

    // 在 private 成员变量部分添加：
    //QNetworkAccessManager* searchManager;  // 可以复用现有的networkManager
    //QString currentSearchQuery;  // 保存当前搜索查询
private:
    std::unique_ptr<ImageProcessor> m_processor;
    std::unique_ptr<OllamaProcessor> ollamaProcessor;
    std::unique_ptr<SearchProcessor> searchProcessor;
    std::unique_ptr<OCRProcessor> ocrProcessor;
};

#endif // MAINWINDOW_H






