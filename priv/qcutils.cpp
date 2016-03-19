#include <QUrl>
#include "qcutils.h"

bool QCUtils::isImageProviderUrl(const QString &url)
{
    QUrl u(url);

    return u.scheme() == "image";
}

bool QCUtils::isResourceUrl(const QString &url)
{
    if (url.indexOf(":") == 0) {
        return true;
    }

    QUrl u(url);

    return u.scheme() == "qrc";
}

QString QCUtils::normalizeResourceUrl(const QString &url)
{
    if (url.indexOf(":") == 0) {
        return url;
    }

    QUrl u(url);
    if (u.scheme() == "qrc") {
        return QString(":") + u.path();
    } else {
        return url;
    }

}

QString QCUtils::normalizeLocalUrl(const QString &url)
{
    if (url.indexOf(":") == 0 || url.indexOf("/") == 0) {
        return url;
    }

    QUrl u(url);
    if (u.scheme() == "qrc") {
        return QString(":") + u.path();
    } else if (u.scheme() == "file") {
        return u.toLocalFile();
    } else {
        return url;
    }

    return url;
}

QString QCUtils::imageProviderId(const QString &url)
{
    QUrl u(url);

    return u.host();
}

QString QCUtils::imageProviderRequestId(const QString &url)
{
    QUrl u(url);

    QString path = u.path();
    if (path.indexOf("/") == 0) {
        path.remove(0,1);
    }

    return path;
}
