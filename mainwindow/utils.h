#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QThread>

class Utils {
public:
    static void delay(int milliseconds);
    static QString cleanText(const QString& text);
};

#endif // UTILS_H