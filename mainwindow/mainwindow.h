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
#include <QUrlQuery>  // ���������ͷ�ļ�
#include <QUrl>       // QUrl Ҳ��Ҫ
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
    //void on_favoriteItemClicked(QListWidgetItem* item); // ��ѡ�����ڵ����ʾ����
    

    


    //void on_pushButton_search_clicked();  // ������ť����¼�
    //void handleSearchReply(QNetworkReply* reply);  // ���������ظ�


public:
    QImage MatToQImage(const cv::Mat& mat);
    bool eventFilter(QObject* obj, QEvent* event);
    QString parseOllamaResponse(const QByteArray& data);

    void saveImageHistory();        // ������ʷ·�����ļ�
    void loadImageHistory();        // ���ļ�������ʷ·��

    void saveOllamaHistory();
    void loadOllamaHistory();

    void saveFavoritesHistory();
    void loadFavoritesHistory();

    QVector<QPair<QString, QString>> favoritesHistory;
    

    // �� private ������ӷ�����
    //void performWebSearch(const QString& query);  // ִ������
    //QString formatSearchResults(const QJsonArray& results);  // ��ʽ���������
    //QString generateRAGPrompt(const QJsonArray& results, const QString& question);  // ����RAG��ʾ
    //QString ragWithOllamaAndSearXNG(const QString& question);  // RAG���̼���
    //QString callOllama(const QString& prompt);


public:


    std::vector<Mat> history;
    int historyIndex;

    QNetworkAccessManager* networkManager;
    QEventLoop loop;

    QVector<QPair<QString, QString>> ollamaHistory;

    QStringList imagePathHistory;
    const QString historyFilePath = "image_history.json";

    // �� private ��Ա����������ӣ�
    //QNetworkAccessManager* searchManager;  // ���Ը������е�networkManager
    //QString currentSearchQuery;  // ���浱ǰ������ѯ
private:
    std::unique_ptr<ImageProcessor> m_processor;
    std::unique_ptr<OllamaProcessor> ollamaProcessor;
    std::unique_ptr<SearchProcessor> searchProcessor;
    std::unique_ptr<OCRProcessor> ocrProcessor;
};

#endif // MAINWINDOW_H






