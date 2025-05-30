#include "querylayer.h"
#include "utils.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QUrl>
#include <QEventLoop>
#include <QDebug>
#include <algorithm>

QJsonArray QueryLayer::searchWithSearXNG(const QString& query) {
    QNetworkAccessManager networkManager;
    QEventLoop loop;

    QUrl url("http://localhost:8080/search");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("q", QUrl::toPercentEncoding(query));
    urlQuery.addQueryItem("format", "json");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    QScopedPointer<QNetworkReply> reply(networkManager.get(request));

    QObject::connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "搜索请求错误:" << reply->errorString();
        return QJsonArray();
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(responseData);

    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "无效的JSON响应";
        return QJsonArray();
    }

    return doc.object()["results"].toArray();
}

QVector<QueryLayer::SearchResult> QueryLayer::processRawResults(const QJsonArray& jsonResults) {
    QVector<SearchResult> results;

    for (const QJsonValue& result : jsonResults) {
        QJsonObject item = result.toObject();

        SearchResult sr;
        sr.title = Utils::cleanText(item["title"].toString());
        sr.url = item["url"].toString();
        sr.content = Utils::cleanText(item["content"].toString());
        sr.relevance = calculateRelevance(sr.title, sr.content);

        results.append(sr);
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.relevance > b.relevance;
        });

    return results;
}

double QueryLayer::calculateRelevance(const QString& title, const QString& content) {
    return title.length() * 0.6 + content.length() * 0.4;
}

//关键词提取模型
QString QueryLayer::queryOllama(const QString& prompt) {
    QNetworkAccessManager networkManager;
    QEventLoop loop;

    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 构造Ollama请求体（使用预定义的提示词模板）
    QJsonObject body;
    body["model"] = "deepseek-r1:1.5b";  // 使用1.5b模型减少时间
    body["prompt"] = prompt;
    body["stream"] = false;
    body["options"] = QJsonObject{ {"temperature", 0.7}, {"top_p", 0.9} };

    QScopedPointer<QNetworkReply> reply(
        networkManager.post(request, QJsonDocument(body).toJson())
    );

    QObject::connect(reply.data(), &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Ollama请求错误:" << reply->errorString();
        return "";
    }

    QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
    return response.object()["response"].toString().trimmed();
}
//关键词提取
QVector<QString>QueryLayer::extractKeywordsWithOllama(const QString& query) {
    if (query.trimmed().isEmpty()) {
        qWarning() << "输入查询为空";
        return {};
    }
    // 1. 安全构造提示词
    const QString prompt =
        QString::fromUtf8(
            u8"你是一个专业搜索关键词分析器。请完成以下任务：\n"
            u8"1. 从用户问题中提取核心关键词\n"
            u8"2. 为每个关键词生成3个最相关的扩展词\n"
            u8"输出格式要求：关键词之间用逗号分隔，主词与扩展词用冒号分隔\n"
            u8"示例输入：'如何预防感冒'\n"
            u8"示例输出：'预防:避免,防范,防护, 感冒:流感,咳嗽,呼吸道感染'\n"
            u8"实际输入："
        ) + query + u8"\n输出：";

    // 2. 验证提示词完整性
    const QString expectedPrefix = "你是一个专业";
    if (!prompt.startsWith(expectedPrefix)) {
        qCritical() << "提示词构造失败！前缀丢失。实际提示词开头:"
            << prompt.left(expectedPrefix.length());
        return {};
    }

    // 3. 调试输出
    qDebug().nospace() << "完整提示词(转义): "
        << prompt.toHtmlEscaped().replace('\n', "\\n");

    // 获取模型响应
    QString response = queryOllama(prompt);
   // qDebug() << "Ollama原始响应:" << response;

    // 解析响应
    QVector<QString> keywords;
    QStringList groups = response.split(',', Qt::SkipEmptyParts);

    for (QString group : groups) {
        QStringList parts = group.split(':', Qt::SkipEmptyParts);
        if (parts.size() >= 1) {
            // 添加主关键词
            QString mainWord = parts[0].trimmed();
            if (!mainWord.isEmpty()) {
                keywords.append(mainWord);
            }

            // 添加扩展词（最多3个）
            if (parts.size() >= 2) {
                QStringList expansions = parts[1].split(',', Qt::SkipEmptyParts);
                for (int i = 0; i < qMin(3, expansions.size()); ++i) {
                    QString word = expansions[i].trimmed();
                    if (!word.isEmpty() && !keywords.contains(word)) {
                        keywords.append(word);
                    }
                }
            }
        }
    }

    return keywords;
}
//搜索功能集成
QVector<QueryLayer::SearchResult> QueryLayer::search(const QString& query, int maxRetries ) {
   /* // 使用Ollama提取关键词
    auto keywords = extractKeywordsWithOllama(query);
    qDebug() << "最终关键词列表:" << keywords;

    // 将关键词组合成新的搜索查询（示例：用OR连接）
    QString expandedQuery;
    QTextStream stream(&expandedQuery);
    for (int i = 0; i < keywords.size(); ++i) {
        if (i > 0) stream << " OR ";
        stream << keywords[i];
    }
    qDebug() << "----- 搜索测试 -----";
    qDebug() << "原始查询:" << query;
    qDebug() << "解析后的关键词:" << keywords;
    qDebug() << "生成的扩展查询:" << expandedQuery;
    // 使用扩展后的查询执行搜索
    auto jsonResults = searchWithSearXNG(expandedQuery);
    return processRawResults(jsonResults);*/
    auto jsonResults = searchWithSearXNG(query);
    return processRawResults(jsonResults);
}