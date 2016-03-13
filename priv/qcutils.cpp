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
