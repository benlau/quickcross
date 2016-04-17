#ifndef QCIMAGELOADERPRIV_H
#define QCIMAGELOADERPRIV_H

#include <QMap>
#include <QStringList>
#include <QRegExp>
#include <QRegularExpressionMatch>
#include <QRegularExpression>

static QString qcImageLoaderDecodeFileName(const QString &input , qreal* ratio) {
    static QRegularExpression re("@[1-9]x+");
    QRegularExpressionMatch matcher;
    qreal devicePixelRatio = 1;
    QString normalizedFileName = input;

    input.indexOf(re,0,&matcher);

    if (matcher.hasMatch()) {
        normalizedFileName.remove(matcher.capturedStart(0) , matcher.capturedLength(0));

        QString ratioString = matcher.captured(0).remove(0,1);
        ratioString.remove(ratioString.size() - 1,1);
        devicePixelRatio = ratioString.toInt();
    }

    if (ratio) {
        *ratio = devicePixelRatio;
    }
    return normalizedFileName;
}

static QStringList qcImageLoaderFilter(const QStringList& files, qreal ratio) {
    QStringList result;

    QMap<QString,qreal> keyRatioMap;
    QMap<QString,QString> keyFileMap;


    for (int i = 0 ; i < files.size() ; i++) {

        QString file = files.at(i);
        QString key = file;
        qreal fileRatio = 1;

        key = qcImageLoaderDecodeFileName(file, &fileRatio);

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
