#ifndef QCIMAGELOADERPRIV_H
#define QCIMAGELOADERPRIV_H

#include <QMap>
#include <QStringList>
#include <QRegExp>
#include <QRegularExpressionMatch>
#include <QRegularExpression>

static QStringList qcImageLoaderFilter(const QStringList& files, qreal ratio) {
    QStringList result;

    QMap<QString,qreal> keyRatioMap;
    QMap<QString,QString> keyFileMap;

    QRegularExpression re("@[0-9]+");

    for (int i = 0 ; i < files.size() ; i++) {

        QString file = files.at(i);
        QRegularExpressionMatch match;
        file.indexOf(re,0,&match);
        QString key = file;
        qreal fileRatio = 1;

        if (match.hasMatch()) {
            key.remove(match.capturedStart(0) , match.capturedLength(0));

            QString ratioString = match.captured(0).remove(0,1);
            fileRatio = ratioString.toInt();
        }

        if (!keyRatioMap.contains(key)) {
            keyRatioMap[key] = fileRatio;
            keyFileMap[key] = file;
            continue;
        }

        qreal origDiff = qAbs(keyRatioMap[key] - ratio);
        qreal currDiff = qAbs(fileRatio - ratio);

        if ( (currDiff < origDiff ) ||
             (currDiff == origDiff && currDiff > ratio)) {
            keyRatioMap[key] = fileRatio;
            keyFileMap[key] = file;
        }
    }

    QMapIterator<QString,qreal> iter(keyRatioMap);
    while (iter.hasNext()) {
        iter.next();

        QString key = iter.key();
        result << keyFileMap[key];
    }

    return result;
}

#endif // QCIMAGELOADERPRIV_H
