#pragma once
#ifndef GENERATIONLAYER_H
#define GENERATIONLAYER_H

#include <QString>
#include <QVector>
#include "contentlayer.h"

class GenerationLayer {
public:
    QString generateResponse(const QString& question, const QVector<ContentLayer::ProcessedContent>& contents);

private:
    QString buildPrompt(const QString& question, const QVector<ContentLayer::ProcessedContent>& contents);
    QString callOllama(const QString& prompt);
    QString parseOllamaResponse(const QByteArray& data);
};

#endif // GENERATIONLAYER_H