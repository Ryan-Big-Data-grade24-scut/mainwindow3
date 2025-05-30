#ifndef QUERYLAYER_H
#define QUERYLAYER_H

#include <QVector>
#include <QString>
#include <QJsonArray>

class QueryLayer {
public:
    struct SearchResult {
        QString title;
        QString url;
        QString content;
        double relevance = 0.0;
    };

    QVector<SearchResult> search(const QString& query, int maxRetries = 3);

private:
    QJsonArray searchWithSearXNG(const QString& query);
    QVector<SearchResult> processRawResults(const QJsonArray& jsonResults);
    double calculateRelevance(const QString& title, const QString& content);
    QString queryOllama(const QString& prompt);
    QVector<QString> extractKeywordsWithOllama(const QString& query);
};

#endif // QUERYLAYER_H