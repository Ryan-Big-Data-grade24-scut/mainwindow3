#include "contentlayer.h"
#include <QRegularExpression>

QVector<ContentLayer::ProcessedContent> ContentLayer::processResults(
    const QVector<QueryLayer::SearchResult>& results, int maxResults) {

    QVector<ProcessedContent> processed;
    int count = std::min(maxResults, static_cast<int>(results.size()));       //Ô­À´µÄÓï¾ä £º  int count = std::min(maxResults, results.size());

    for (int i = 0; i < count; ++i) {
        const auto& result = results[i];
        ProcessedContent pc;
        pc.sourceUrl = result.url;
        pc.summary = generateSummary(result.content);
        pc.keyPoints = extractKeyPoints(result.content);
        processed.append(pc);
    }

    return processed;
}

QString ContentLayer::generateSummary(const QString& content, int maxLength) {
    return content.left(maxLength) + (content.length() > maxLength ? "..." : "");
}

QString ContentLayer::extractKeyPoints(const QString& content) {
    QStringList sentences = content.split(QRegularExpression("[.!?]\\s*"));
    QStringList keySentences;

    for (const QString& sentence : sentences) {
        if (sentence.length() > 30 && sentence.length() < 150) {
            keySentences.append("- " + sentence.trimmed());
            if (keySentences.size() >= 3) break;
        }
    }

    return keySentences.join("\n");
}