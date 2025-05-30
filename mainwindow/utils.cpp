#include "utils.h"
#include <QRegularExpression>

void Utils::delay(int milliseconds) {
    QThread::msleep(milliseconds);
}

QString Utils::cleanText(const QString& text) {
    QString cleaned = text;
    cleaned.replace(QRegularExpression("\\s+"), " ");
    return cleaned.trimmed();
}