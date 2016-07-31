#include <QtCore>
#include <QUrl>
#include <QQmlComponent>
#include <QStandardPaths>

#ifdef WIN32
#include <sys/utime.h>
#else
#include <utime.h>
#endif

#include "qcutils.h"

static bool match(const QString&fileName,const QStringList &nameFilters) {
    bool res = false;

    for (int i = 0 ; i < nameFilters.size() ; i++) {
        const QString& filter = nameFilters.at(i);
        QRegExp rx(filter,Qt::CaseInsensitive,QRegExp::Wildcard);
        if (rx.exactMatch(fileName))  {
            res = true;
            break;
        }
    }

    return res;
}

/// Take out "." and ".." files
static QStringList filterLocalFiles(const QStringList& files) {
    QStringList result;
    for (int i = 0 ; i < files.size() ; i++) {
        QString file = files[i];
        if (file == "." || file == "..") {
            continue;
        }
        result << file;
    }

    return result;
}

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

QJSValue QCUtils::loadJavascript(QQmlEngine *engine, const QString &url, const QString &member)
{

    QString pattern  = "import QtQuick 2.0\nimport \"%1\" as JSObject;QtObject { property var object : JSObject.%2}";

    QString qml = pattern.arg(url).arg(member);

    QObject* holder = 0;

    QQmlComponent comp (engine);
    comp.setData(qml.toUtf8(),QUrl());
    holder = comp.create();

    if (!holder) {
        qWarning() << QString("QuickCross: Failed to load Javscript: %1").arg(url);
        qWarning() << QString("Error: ") << comp.errorString();
        return 0;
    }

    QJSValue object = holder->property("object").value<QJSValue>();
    holder->deleteLater();

    if (object.isError()) {
        qWarning() << QString("QuickCross: Failed to load Javascript: %1").arg(url);
        qWarning() << object.toString();
    }

    return object;
}

QString QCUtils::dirname(const QString &path)
{
    QFileInfo info(path);
    QString parent = info.absolutePath();
    if (parent.isEmpty())
       parent = "/";

    // No tailing "/" for dirname command

    return parent;
}

QString QCUtils::basename(const QString &path)
{
    QString result = "/";
    QStringList token = path.split("/");

    int i = token.size() -1;
    while (i >=0) {
        QString name = token.at(i--);
        if (!name.isEmpty()) {
            result = name;
            break;
        }
    }

    return result;
}

QStringList QCUtils::find(const QString &root, const QStringList &nameFilters)
{
    QDir dir(root);
    QString absRoot = dir.absolutePath();

    QQueue<QString> queue;
    queue.enqueue(absRoot);
    QStringList result;

    while (queue.size() > 0) {
        QString current = queue.dequeue();
        QDir dir(current);
        QFileInfoList infos = dir.entryInfoList();

        for (int i = 0 ; i < infos.size() ; i++) {
            QFileInfo info = infos.at(i);

            if (info.fileName() == "." || info.fileName() == "..") {
                continue;
            }

            QString absPath = info.absoluteFilePath();

            if (info.isDir()) {
                queue.enqueue(absPath);
                continue;
            }

            QString fileName = info.fileName();

            if (nameFilters.size() > 0 && !match(fileName, nameFilters)) {
                continue;
            }

            if (root != absRoot) {
                absPath.replace(absRoot, root);
            }

            result << absPath;
        }
    }

    return result;
}

bool QCUtils::rmdir(const QString &path, bool recursive)
{
    QStringList preservePaths;
    preservePaths << "/";

    for (int i = QStandardPaths::DesktopLocation ;
         i <= QStandardPaths::AppConfigLocation; i++) {
        QStringList paths =  QStandardPaths::standardLocations((QStandardPaths::StandardLocation) i);
        preservePaths.append(paths);
    }

    if (preservePaths.indexOf(path) >= 0) {
        qWarning() <<  "rmdir() - It can't remove preserve paths";
        return false;
    }

    QDir dir(path);

    if (!recursive) {
        QStringList entry = filterLocalFiles(dir.entryList());
        if (entry.size() > 0) {
            return false;
        }
    }
    return dir.removeRecursively();
}

void QCUtils::touch(const QString &path)
{

    QFileInfo info(path);

    if (!info.exists()) {

        QFile file(path);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Failed to create file:" << path;
        }
        file.close();

    } else {

        if (utime(path.toLocal8Bit().constData(), 0) == -1) {
            qWarning() << "utimes failed:" << path;
        }
    }
}
