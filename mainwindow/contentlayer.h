#ifndef CONTENTLAYER_H
#define CONTENTLAYER_H

#include <QVector>
#include <QString>
#include "querylayer.h"

class ContentLayer {
public:
    struct ProcessedContent {
        QString summary;
        QString keyPoints;
        QString sourceUrl;
    };

    QVector<ProcessedContent> processResults(const QVector<QueryLayer::SearchResult>& results, int maxResults = 5);

private:
    QString generateSummary(const QString& content, int maxLength = 200);
    QString extractKeyPoints(const QString& content);
};

#endif // CONTENTLAYER_H