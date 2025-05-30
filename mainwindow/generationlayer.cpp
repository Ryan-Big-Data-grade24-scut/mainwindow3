#include "generationlayer.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>
#include <QDebug>
#include <QRegularExpression>

QString GenerationLayer::generateResponse(const QString& question, const QVector<ContentLayer::ProcessedContent>& contents) {
    QString prompt = buildPrompt(question, contents);
    return callOllama(prompt);
}

QString GenerationLayer::buildPrompt(const QString& question, const QVector<ContentLayer::ProcessedContent>& contents) {
    QString prompt = "你是一个智能助手，请基于以下搜索结果回答问题。"
        "确保回答准确、简洁，并注明信息来源。\n\n";

    prompt += "问题: " + question + "\n\n";
    prompt += "搜索结果:\n";

    for (int i = 0; i < contents.size(); ++i) {
        const auto& content = contents[i];
        prompt += QString("\n[来源 %1 - %2]\n").arg(i + 1).arg(content.sourceUrl);
        prompt += "摘要: " + content.summary + "\n";
        prompt += "关键点:\n" + content.keyPoints + "\n";
    }

    prompt += "\n请基于以上信息回答问题，并在回答中引用相关来源。回答:";
    return prompt;
}

QString GenerationLayer::callOllama(const QString& prompt) {
    QNetworkAccessManager networkManager;
    QEventLoop loop;

    QUrl url("http://localhost:11434/api/generate");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "deepseek-r1:1.5b";
    json["prompt"] = prompt;
    json["stream"] = false;
    json["options"] = QJsonObject{ {"temperature", 0.7}, {"top_p", 0.9} };

    QNetworkReply* reply = networkManager.post(request, QJsonDocument(json).toJson());

    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Ollama请求错误:" << reply->errorString();
        return "无法生成回答，请检查Ollama服务是否运行正常。";
    }

    QByteArray responseData = reply->readAll();
    return parseOllamaResponse(responseData);
}

QString GenerationLayer::parseOllamaResponse(const QByteArray& data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qDebug() << "无效的Ollama响应";
        return "无法解析回答。";
    }

    QJsonObject obj = doc.object();
    QString response = obj["response"].toString();
    response.replace(QRegularExpression("\\n\\s+\\n"), "\n\n");
    return response.trimmed();
}