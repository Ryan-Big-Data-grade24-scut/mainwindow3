#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <QKeyEvent>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QFileDialog>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QJsonArray>
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
#include <QDesktopWidget>  // Qt 5�������ͷ�ļ�
#include <QMessageBox>
#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QMetaObject>
#include "OllamaProcessor.h"
#include "ImgProcessor.h"




MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::mainwindowClass),
    historyIndex(-1),
    networkManager(new QNetworkAccessManager(this)),
    searchManager(new QNetworkAccessManager(this)),// ��ʼ�����������
    m_processor(nullptr)
{
    ui->setupUi(this);
    //QMessageBox::warning(this, "UI", "UI Successfully setup");

    // ��ʼ����ť״̬
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_history->setEnabled(false);
    ui->pushButton_toPage4->setEnabled(false);
    history.clear();

    // ���ҳ��
    ui->stackedWidget->addWidget(new QWidget()); // �ڶ�ҳ
    ui->stackedWidget->addWidget(new QWidget()); // ����ҳ
    ui->stackedWidget->addWidget(new QWidget()); // ����ҳ

    // ��װ�¼�������
    ui->stackedWidget->installEventFilter(this);
    ui->stackedWidget->widget(1)->installEventFilter(this);
    ui->stackedWidget->widget(2)->installEventFilter(this);
    ui->stackedWidget->widget(3)->installEventFilter(this);

    // ��ť��
    connect(ui->pushButton_toPage4, &QPushButton::clicked, this, &MainWindow::on_pushButton_toPage4_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    //connect(ui->pushButton_ollama, &QPushButton::clicked, this, &MainWindow::startOllamaInteraction);
    //connect(ui->listWidget_history, &QListWidget::itemClicked, this, &MainWindow::on_historyItemClicked);
    //connect(ui->lineEdit_prompt, &QLineEdit::returnPressed, this, &MainWindow::startOllamaInteraction); // ����
    //connect(ui->pushButton_addFavorite, &QPushButton::clicked, this, &MainWindow::on_pushButton_addFavorite_clicked);
    //connect(ui->pushButton_removeFavorite, &QPushButton::clicked, this, &MainWindow::on_pushButton_removeFavorite_clicked);
    //connect(ui->listWidget_favorites, &QListWidget::itemClicked, this, &MainWindow::on_favoriteItemClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton_3_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);
    connect(ui->pushButton_history, &QPushButton::clicked, this, &MainWindow::on_pushButton_history_clicked);
    //connect(ui->pushButton_gaussian, &QPushButton::clicked, this, &MainWindow::on_pushButton_gaussian_clicked);
    /*
    ע�⣺
    ui_mainwindow.h��//QMetaObject::connectSlotsByName(mainwindowClass);
    Ҫע�͵�
    ����һ��Ҫ��on_..._clicked������������������̬�޴�̸��
    ���ǲ���git�ķ�֧������
    */

    /*
    1. �ܹ����ͣ�

        MainWindow::MainWindow������������ġ��ӿڡ�

            ����m_processor(ImageProcessor)

            ������ָ�봫������

        ImageProcessor��������ע�ᡢ���ӵġ����ࡱ

            ImageProcessor::ImageProcessor

                ע�ᣨ��Opt_types�л�ȡ��ע��Ĺ��ܣ�

            connectAll

                ���ӣ��źţ�op->button()��ۺ�����op->process()���ӣ�

        ImageOperations����ͼ������Ļ���

        GaussianBlurOperation����������������������

    2. �������µĹ��ܣ�

        ���ȣ���ͷ�ļ�����Core_opts�У�����ͷ�ļ�������һ��___Operation��
        Ȼ�󣺷��������࣬
            �޸�"setButton"�е�buttonָ��
            �޸�"process"�еĲ���
            �޸�"name"�е�����
        �����ͷ�ļ�����Opt_type����Opt_types.h�У������ͷ�ļ�������������
    */
    m_processor = std::make_unique<ImageProcessor>(this, this);  // ����thisָ��
    m_processor->connectAll();

    ollamaProcessor = std::make_unique<OllamaProcessor>(this);
    ollamaProcessor->connectAll();



    //connect(ui->pushButton_gray, &QPushButton::clicked, this, &MainWindow::on_pushButton_gray_clicked);
    //connect(ui->pushButton_canny, &QPushButton::clicked, this, &MainWindow::on_pushButton_canny_clicked);

    loadImageHistory(); // ������ʷ��¼
    loadOllamaHistory();
    loadFavoritesHistory();

    // ��������
    /*connect(ui->pushButton_search, &QPushButton::clicked,
        this, &MainWindow::on_pushButton_search_clicked);
    connect(ui->pushButton_rag, &QPushButton::clicked, [this]() {
        if (!currentSearchQuery.isEmpty()) {
            QString answer = ragWithOllamaAndSearXNG(currentSearchQuery);
            ui->textBrowser_results->append("\nAI�������:\n" + answer);
        }
        });*/
}

MainWindow::~MainWindow()
{
    delete ui;
    saveOllamaHistory();
    saveFavoritesHistory();

}

void MainWindow::on_pushButton_clicked()
{
    ui->label->clear();
    ui->label_1->clear();
    QString filename = QFileDialog::getOpenFileName(this, tr("��ͼƬ"), ".", tr("Image file(*.png *.jpg *.bmp)"));
    image = cv::imread(filename.toLocal8Bit().data());
    if (image.data) {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_history->setEnabled(true);
        ui->pushButton_toPage4->setEnabled(true);
        history.clear();
        historyIndex = -1;
        display_MatInQT(ui->label, image);

        if (!imagePathHistory.contains(filename)) {
            imagePathHistory.append(filename);
            saveImageHistory();
        }
    }
    else {
        QMessageBox::information(this, tr("��ʾ"), tr("δ�ɹ�����ͼƬ��"), QMessageBox::Ok);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QLabel* label_1_new_page = ui->stackedWidget->widget(1)->findChild<QLabel*>("label_3");
    if (label_1_new_page) {
        display_MatInQT(label_1_new_page, image);
    }
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_4_clicked()
{
    auto reply = QMessageBox::question(this, tr("�˳�ȷ��"), tr("��ȷ��Ҫ�˳�������"),
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::on_pushButton_history_clicked()
{
    if (imagePathHistory.isEmpty()) {
        QMessageBox::information(this, tr("��ʾ"), tr("û����ʷ��¼��"), QMessageBox::Ok);
        return;
    }

    QString selected = QInputDialog::getItem(this, tr("��ʷͼƬ"), tr("ѡ��һ����ʷͼƬ·��:"), imagePathHistory, 0, false);
    if (!selected.isEmpty() && QFile::exists(selected)) {
        image = cv::imread(selected.toLocal8Bit().data());
        if (image.data) {
            display_MatInQT(ui->label, image);
        }
        else {
            QMessageBox::warning(this, tr("����"), tr("�޷����ظ�ͼƬ��"));
        }
    }
}

QImage MainWindow::MatToQImage(const cv::Mat& mat)
{
    try {
        // �����֤�������
        if (mat.empty() || mat.dims != 2 || mat.rows <= 0 || mat.cols <= 0) {
            qWarning() << "Invalid matrix dimensions:" << mat.rows << "x" << mat.cols;
            return QImage();
        }

        // ��ȫ��ȡ�������ݣ����ر�����
        cv::Mat localMat;
        if (!mat.isContinuous() || mat.u == nullptr || !mat.u->refcount) {
            localMat = mat.clone();
            qDebug() << "Matrix cloned for safety";
        }
        else {
            // ��ȷ���������ڴ��С
            size_t requiredSize = mat.rows * mat.step;
            if (requiredSize == 0) {
                qWarning() << "Zero-size matrix detected";
                return QImage();
            }
            localMat.create(mat.rows, mat.cols, mat.type());
            mat.copyTo(localMat);
        }

        // ��ȫת������
        auto safeConvert = [](const cv::Mat& mat) -> QImage {
            // ��֤�����������
            if (mat.depth() != CV_8U) {
                qWarning() << "Unsupported matrix depth:" << mat.depth();
                return QImage();
            }

            // ����ͨ����ѡ����ʽ
            switch (mat.channels()) {
            case 1: {  // �Ҷ�ͼ
                QImage image(mat.cols, mat.rows, QImage::Format_Grayscale8);

                // ˫����֤ͼ�񴴽����
                if (image.isNull() || image.width() != mat.cols || image.height() != mat.rows) {
                    qCritical() << "Failed to create grayscale QImage";
                    return QImage();
                }

                // ��ȫ�ڴ濽�������߽��飩
                for (int row = 0; row < mat.rows; ++row) {
                    const uchar* src = mat.ptr<uchar>(row);
                    uchar* dst = image.scanLine(row);
                    size_t copySize = std::min(
                        static_cast<size_t>(mat.cols),
                        static_cast<size_t>(image.bytesPerLine())
                    );
                    if (src && dst && copySize > 0) {
                        memcpy(dst, src, copySize);
                    }
                }
                return image;
            }

            case 3: {  // BGRͼ
                QImage image(mat.cols, mat.rows, QImage::Format_RGB888);
                if (image.isNull()) return QImage();

                // ��ȫת��BGR->RGB
                for (int row = 0; row < mat.rows; ++row) {
                    const cv::Vec3b* src = mat.ptr<cv::Vec3b>(row);
                    uchar* dst = image.scanLine(row);

                    if (src && dst) {
                        for (int col = 0; col < mat.cols; ++col) {
                            dst[col * 3 + 0] = src[col][2];  // R
                            dst[col * 3 + 1] = src[col][1];  // G
                            dst[col * 3 + 2] = src[col][0];  // B
                        }
                    }
                }
                return image;
            }

            case 4: {  // BGRAͼ
                QImage image(mat.cols, mat.rows, QImage::Format_ARGB32);
                if (image.isNull()) return QImage();

                // ��ȫת��BGRA->ARGB
                for (int row = 0; row < mat.rows; ++row) {
                    const cv::Vec4b* src = mat.ptr<cv::Vec4b>(row);
                    QRgb* dst = reinterpret_cast<QRgb*>(image.scanLine(row));

                    if (src && dst) {
                        for (int col = 0; col < mat.cols; ++col) {
                            dst[col] = qRgba(
                                src[col][2],  // R
                                src[col][1],  // G
                                src[col][0],  // B
                                src[col][3]   // A
                            );
                        }
                    }
                }
                return image;
            }

            default: {
                qWarning() << "Unsupported channel count:" << mat.channels();
                return QImage();
            }
            }
            };

        // ִ��ת��
        QImage result = safeConvert(localMat);

        // ������֤
        if (result.isNull()) {
            qWarning() << "Final conversion failed for matrix type:" << localMat.type();
        }
        return result;

    }
    catch (const cv::Exception& e) {
        qCritical() << "OpenCV Error:" << e.what()
            << "in function" << e.func.c_str()
            << "at" << e.file.c_str() << ":" << e.line;
    }
    catch (const std::exception& e) {
        qCritical() << "STD Exception:" << e.what();
    }
    catch (...) {
        qCritical() << "Unknown exception occurred";
    }

    return QImage();
}

void MainWindow::display_MatInQT(QLabel* label, const cv::Mat& mat) {
    // 0. �̰߳�ȫ��֤
    if (QThread::currentThread() != qApp->thread()) {
        QMetaObject::invokeMethod(this, [=]() {
            display_MatInQT(label, mat);
            }, Qt::QueuedConnection); // ��Ϊ������ʽ
        return;
    }

    // 1. ת��Ϊ�Գ��ڴ��QImage
    QImage qimg = MatToQImage(mat); // ����static��Ա����
    if (qimg.isNull()) return;

    // 2. ֱ�Ӵ���QPixmap
    QPixmap pixmap = QPixmap::fromImage(qimg);
    if (pixmap.isNull()) return;

    // 3. ��ʾ����
    QPixmap scaledPixmap = pixmap.scaled(
        label->width(), label->height(), // ʹ��width/height���ɿ�
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    );

    label->setPixmap(scaledPixmap);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Escape) {
            on_esc_pressed();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::on_esc_pressed()
{
    if (ui->stackedWidget->currentIndex() == 1 || ui->stackedWidget->currentIndex() == 2 || ui->stackedWidget->currentIndex() == 3) {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

//void MainWindow::startOllamaInteraction()
//{
//    QString prompt = ui->lineEdit_prompt->text().trimmed();
//    if (prompt.isEmpty()) {
//        QMessageBox::warning(this, tr("����"), tr("��ʾ�ʲ���Ϊ�գ�"));
//        return;
//    }
//
//    QUrl url("http://localhost:11434/api/generate");
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonObject json;
//    json.insert("model", "deepseek-r1:1.5b");
//    json.insert("prompt", prompt);
//    json.insert("max_tokens", 150);
//
//    QNetworkReply* reply = networkManager->post(request, QJsonDocument(json).toJson());
//
//    connect(reply, &QNetworkReply::finished, this, [this, reply, prompt]() {
//        QString response;
//        if (reply->error() == QNetworkReply::NoError) {
//            QByteArray responseData = reply->readAll();
//            response = parseOllamaResponse(responseData);
//        }
//        else {
//            qDebug() << "�������:" << reply->errorString();
//            ui->textEdit_result->setText("�������: " + reply->errorString());
//            return;
//        }
//
//        QStringList lines = response.split("\n");
//        QString result, resp;
//        for (const QString& line : lines) {
//            QJsonParseError error;
//            QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &error);
//            if (error.error == QJsonParseError::NoError && doc.isObject()) {
//                QJsonObject obj = doc.object();
//                if (obj.contains("response") && obj.contains("done")) {
//                    bool done = obj["done"].toBool();
//                    resp = obj["response"].toString();
//                    result += resp;
//                    if (done) break;
//                }
//            }
//        }
//
//        // ��ʾ���
//        ui->textEdit_result->setText(result);
//
//        // ������ʷ
//        ollamaHistory.append(qMakePair(prompt, result));
//        ui->listWidget_history->addItem(prompt.left(30)); // ��ʾǰ30���ַ�
//        // ? ��������
//        ui->lineEdit_prompt->clear();
//        });
//}
//
//void MainWindow::on_historyItemClicked(QListWidgetItem* item)
//{
//    int index = ui->listWidget_history->row(item);
//    if (index >= 0 && index < ollamaHistory.size()) {
//        QString q = ollamaHistory[index].first;
//        QString a = ollamaHistory[index].second;
//        ui->textEdit_result->setText("Q: " + q + "\n\nA: " + a);
//    }
//}

void MainWindow::on_pushButton_toPage4_clicked()
{

    QLabel* label_on_page4 = ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6");
    if (label_on_page4 && image.data) {
        display_MatInQT(label_on_page4, image);
    }
    ui->stackedWidget->setCurrentIndex(3);

}

//void MainWindow::on_pushButton_addFavorite_clicked()
//{
//    int index = ui->listWidget_history->currentRow();
//    if (index < 0 || index >= ollamaHistory.size()) {
//        QMessageBox::warning(this, tr("��ʾ"), tr("����ѡ��һ����ʷ��¼��"));
//        return;
//    }
//
//    auto pair = ollamaHistory[index];
//
//    // ��׼ȥ�أ�prompt �� response ������ƥ��
//    for (const auto& existing : favoritesHistory) {
//        if (existing.first == pair.first && existing.second == pair.second) {
//            return;
//        }
//    }
//
//    favoritesHistory.append(pair);
//
//    auto* item = new QListWidgetItem(pair.first.left(30));
//    // ? �������� prompt �� response ��Ϊ UserRole+UserRole+1
//    item->setData(Qt::UserRole, pair.first);
//    item->setData(Qt::UserRole + 1, pair.second);
//    ui->listWidget_favorites->addItem(item);
//
//    saveFavoritesHistory();
//}

//void MainWindow::on_pushButton_removeFavorite_clicked()
//{
//    int index = ui->listWidget_favorites->currentRow();
//    if (index < 0 || index >= ui->listWidget_favorites->count())
//        return;
//
//    QListWidgetItem* item = ui->listWidget_favorites->item(index);
//    QString prompt = item->data(Qt::UserRole).toString();
//    QString response = item->data(Qt::UserRole + 1).toString();
//
//    // ����ȷ��
//    auto confirm = QMessageBox::question(this, tr("ȷ���Ƴ��ղ�"),
//        tr("��ȷ��Ҫ�Ƴ������ղؼ�¼��\n\nPrompt:\n%1\n\nResponse:\n%2")
//        .arg(prompt.left(100)).arg(response.left(100)),
//        QMessageBox::Yes | QMessageBox::No);
//
//    if (confirm != QMessageBox::Yes)
//        return;
//
//    // ��ȫɾ��������ȫƥ�����ֹ���ظ���
//    favoritesHistory.erase(std::remove_if(favoritesHistory.begin(), favoritesHistory.end(),
//        [&](const QPair<QString, QString>& pair) {
//            return pair.first == prompt && pair.second == response;
//        }), favoritesHistory.end());
//
//    delete ui->listWidget_favorites->takeItem(index);
//    saveFavoritesHistory();
//}
//
//void MainWindow::on_favoriteItemClicked(QListWidgetItem* item)
//{
//    QString prompt = item->data(Qt::UserRole).toString();
//    QString response = item->data(Qt::UserRole + 1).toString();
//    ui->textEdit_result->setText("Q: " + prompt + "\n\nA: " + response);
//}
//
//
//
//void MainWindow::on_pushButton_gaussian_clicked()
//{
//    /*
//    if (!image.data) {
//        QMessageBox::warning(this, tr("����"), tr("δ����ͼ��"));
//        return;
//    }
//    GaussianBlur(image, mat_Gaussian, Size(29, 29), 0, 0);
//    QLabel* label = ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6");
//    if (label)
//        display_MatInQT(label, mat_Gaussian);*/
//}
//
//
//void MainWindow::on_pushButton_gray_clicked()
//{
//    if (!image.data) {
//        QMessageBox::warning(this, tr("����"), tr("δ����ͼ��"));
//        return;
//    }
//    cvtColor(image, gray, COLOR_BGR2GRAY);
//    QLabel* label = ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6");
//    if (label)
//        display_MatInQT(label, gray);
//}
//
//void MainWindow::on_pushButton_canny_clicked()
//{
//    if (!image.data) {
//        QMessageBox::warning(this, tr("����"), tr("δ����ͼ��"));
//        return;
//    }
//    cvtColor(image, gray, COLOR_BGR2GRAY); // Canny Ҫ��Ҷ�ͼ
//    cv::Mat edge;
//    Canny(gray, edge, 100, 150, 3);
//    QLabel* label = ui->stackedWidget->widget(3)->findChild<QLabel*>("label_6");
//    if (label)
//        display_MatInQT(label, edge);
//}

QString MainWindow::parseOllamaResponse(const QByteArray& data)
{
    return QString::fromUtf8(data);
}

void MainWindow::saveImageHistory()
{
    QJsonArray array;
    for (const QString& path : imagePathHistory)
        array.append(path);

    QJsonDocument doc(array);
    QFile file(historyFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadImageHistory()
{
    QFile file(historyFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray array = doc.array();
            for (const QJsonValue& val : array) {
                QString path = val.toString();
                if (QFile::exists(path)) {
                    imagePathHistory.append(path);
                }
            }
        }
    }
}

void MainWindow::saveOllamaHistory()
{
    QJsonArray array;
    for (const auto& pair : ollamaHistory) {
        QJsonObject obj;
        obj["prompt"] = pair.first;
        obj["response"] = pair.second;
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file("ollama_history.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadOllamaHistory()
{
    QFile file("ollama_history.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray array = doc.array();
            for (const QJsonValue& val : array) {
                QJsonObject obj = val.toObject();
                QString prompt = obj["prompt"].toString();
                QString response = obj["response"].toString();
                ollamaHistory.append(qMakePair(prompt, response));
                ui->listWidget_history->addItem(prompt.left(30));
            }
        }
    }
}

void MainWindow::saveFavoritesHistory()
{
    QJsonArray array;
    for (const auto& pair : favoritesHistory) {
        QJsonObject obj;
        obj["prompt"] = pair.first;
        obj["response"] = pair.second;
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file("favorites_history.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

//void MainWindow::on_pushButton_search_clicked()
//{
//    QString query = ui->lineEdit_search->text().trimmed();
//    if (query.isEmpty()) {
//        QMessageBox::warning(this, "��ʾ", "��������������");
//        return;
//    }
//    currentSearchQuery = query;
//    performWebSearch(query);
//}

void MainWindow::loadFavoritesHistory()
{
    QFile file("favorites_history.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray array = doc.array();
            QSet<QString> seen; // ����ȥ��
            for (const QJsonValue& val : array) {
                QJsonObject obj = val.toObject();
                QString prompt = obj["prompt"].toString();
                QString response = obj["response"].toString();
                QString key = prompt + "::" + response;
                if (!seen.contains(key)) {
                    seen.insert(key);
                    favoritesHistory.append(qMakePair(prompt, response));

                    // ? �������������ݵ� item
                    auto* item = new QListWidgetItem(prompt.left(30));
                    item->setData(Qt::UserRole, prompt);
                    item->setData(Qt::UserRole + 1, response);
                    ui->listWidget_favorites->addItem(item);
                }
            }
        }
    }
}

//void MainWindow::handleSearchReply(QNetworkReply* reply)
//{
//    if (reply->error() == QNetworkReply::NoError) {
//        QByteArray responseData = reply->readAll();
//        QJsonDocument doc = QJsonDocument::fromJson(responseData);
//
//        if (!doc.isNull() && doc.isObject()) {
//            QJsonArray results = doc.object()["results"].toArray();
//            ui->textBrowser_results->setHtml(formatSearchResults(results));
//        }
//        else {
//            ui->textBrowser_results->setText("��Ч����Ӧ��ʽ");
//        }
//    }
//    else {
//        ui->textBrowser_results->setText("����ʧ��: " + reply->errorString());
//    }
//    reply->deleteLater();
//}
//
//void MainWindow::performWebSearch(const QString& query)
//{
//    ui->textBrowser_results->setText("������...");
//
//    QUrl url("http://localhost:8080/search");
//    QUrlQuery urlQuery;
//    urlQuery.addQueryItem("q", QUrl::toPercentEncoding(query));
//    urlQuery.addQueryItem("format", "json");
//    url.setQuery(urlQuery);
//
//    QNetworkRequest request(url);
//    QNetworkReply* reply = searchManager->get(request);
//
//    connect(reply, &QNetworkReply::finished,
//        this, [this, reply]() { handleSearchReply(reply); });
//}
//
//QString MainWindow::formatSearchResults(const QJsonArray& results)
//{
//    QString html;
//    if (results.isEmpty()) {
//        return "δ�ҵ���ؽ��";
//    }
//
//    html += "<h2>�������:</h2>";
//    for (int i = 0; i < results.size() && i < 5; ++i) { // ������ʾ5�����
//        QJsonObject item = results[i].toObject();
//        html += QString("<div style='margin-bottom: 15px;'>"
//            "<h3><a href='%1'>%2</a></h3>"
//            "<p>%3</p>"
//            "</div>")
//            .arg(item["url"].toString())
//            .arg(item["title"].toString())
//            .arg(item["content"].toString());
//    }
//    return html;
//}
//
//QString MainWindow::generateRAGPrompt(const QJsonArray& results, const QString& question)
//{
//    QString prompt = "����������������ش����⣺\n\n";
//    for (const QJsonValue& result : results) {
//        QJsonObject item = result.toObject();
//        prompt += "����: " + item["title"].toString() + "\n";
//        prompt += "����: " + item["content"].toString() + "\n\n";
//    }
//    prompt += "����: " + question + "\n�ش�:";
//    return prompt;
//}
//
//QString MainWindow::ragWithOllamaAndSearXNG(const QString& question)
//{
//    QUrl url("http://localhost:8080/search");
//    QUrlQuery urlQuery;
//    urlQuery.addQueryItem("q", QUrl::toPercentEncoding(question));
//    urlQuery.addQueryItem("format", "json");
//    url.setQuery(urlQuery);
//
//    QNetworkRequest request(url);
//    QNetworkReply* reply = networkManager->get(request);
//
//    QEventLoop loop;
//    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();
//
//    if (reply->error() != QNetworkReply::NoError) {
//        return "����ʧ��: " + reply->errorString();
//    }
//
//    QByteArray responseData = reply->readAll();
//    QJsonDocument doc = QJsonDocument::fromJson(responseData);
//    if (doc.isNull() || !doc.isObject()) {
//        return "��Ч�����������ʽ";
//    }
//
//    QJsonArray results = doc.object()["results"].toArray();
//    if (results.isEmpty()) {
//        return "δ�ҵ������Ϣ";
//    }
//
//    QString prompt = generateRAGPrompt(results, question);
//    return callOllama(prompt); // ʹ�������е�callOllama����
//}
//
//QString MainWindow::callOllama(const QString& prompt)
//{
//    QNetworkAccessManager networkManager;
//    QEventLoop loop;
//
//    QUrl url("http://localhost:11434/api/generate");
//    QNetworkRequest request(url);
//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//
//    QJsonObject json;
//    json["model"] = "deepseek-r1:8b";
//    json["prompt"] = prompt;
//    json["stream"] = false;
//
//    QNetworkReply* reply = networkManager.post(request, QJsonDocument(json).toJson());
//
//    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
//    loop.exec();
//
//    if (reply->error() != QNetworkReply::NoError) {
//        qDebug() << "Ollama request error:" << reply->errorString();
//        return "";
//    }
//
//    QByteArray responseData = reply->readAll();
//    return parseOllamaResponse(responseData);
//}




