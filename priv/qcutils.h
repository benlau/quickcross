#ifndef QCUTILS_H
#define QCUTILS_H

#include <QString>
#include <QQmlEngine>
#include <QJSValue>

class QCUtils
{
public:
    static bool isImageProviderUrl(const QString& url);

    static bool isResourceUrl(const QString& url);

    static QString normalizeResourceUrl(const QString& url);

    static QString normalizeLocalUrl(const QString& url);

    static QString imageProviderId(const QString& url);

    static QString imageProviderRequestId(const QString& url);

    static QJSValue loadJavascript(QQmlEngine* engine, const QString& url, const QString &member);


};

#endif // QCUTILS_H
