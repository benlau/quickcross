/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QtCore>
#include <QUrlQuery>
#include <QGuiApplication>
#include <QPainter>
#include "qcimageprovider.h"
#include "qcimageloader.h"
#include "qcimagepool.h"

class QCImageProviderQueryID {
public:

    QCImageProviderQueryID() {
        scaleToFitDpi = false;
    }

    bool hasModification() const {
        return scaleToFitDpi || tintColor.isValid();
    }

    QString removeSuffix(const QString& input) const {
        QStringList token = input.split(".");

        return token[0];
    }

    void parse(QString id) {
        QUrl inputUrl(id);
        QUrl outputUrl;

        fileName = removeSuffix(inputUrl.path());
        QUrlQuery inputQuery (inputUrl);
        QUrlQuery outputQuery;

        outputUrl.setPath(fileName);

        if (inputQuery.hasQueryItem("scaleToFitDpi")) {
            QVariant v = inputQuery.queryItemValue("scaleToFitDpi");
            scaleToFitDpi = v.toBool();
            v = scaleToFitDpi;
            outputQuery.addQueryItem("scaleToFitDpi", v.toString());
        }

        if (inputQuery.hasQueryItem("tintColor")) {
            QVariant v = inputQuery.queryItemValue("tintColor");
            if (QColor::isValidColor(v.toString())) {
                tintColor = QColor(v.toString());
                outputQuery.addQueryItem("tintColor", inputQuery.queryItemValue("tintColor"));
            }
        }

        outputUrl.setQuery(outputQuery.query());
        cacheKey = outputUrl.toString();
    }

    QString fileName;
    bool scaleToFitDpi;
    QString cacheKey;
    QColor tintColor;
};

/// Copy from QuickAndroid project
static void gray(QImage& dest,QImage& src)
{
    for (int y = 0; y < src.height(); ++y) {
        unsigned int *data = (unsigned int *)src.scanLine(y);
        unsigned int *outData = (unsigned int*)dest.scanLine(y);

        for (int x = 0 ; x < src.width(); x++) {
            int val = qGray(data[x]);
            outData[x] = qRgba(val,val,val,qAlpha(data[x]));
        }
    }

}

/// Copy from QuickAndroid project
static QImage colorize(QImage src, QColor tintColor) {
    if (src.format() != QImage::Format_ARGB32) {
        src = src.convertToFormat(QImage::Format_ARGB32);
    }

    QImage dst = QImage(src.size(), src.format());

    gray(dst,src);

    QPainter painter(&dst);

    QColor pureColor = tintColor;
    pureColor.setAlpha(255);

    painter.setCompositionMode(QPainter::CompositionMode_Screen);
    painter.fillRect(0,0,dst.width(),dst.height(),pureColor);
    painter.end();

    if (tintColor.alpha() != 255) {
        QImage buffer = QImage(src.size(), src.format());
        buffer.fill(QColor("transparent"));
        QPainter bufPainter(&buffer);
        qreal opacity = tintColor.alpha() / 255.0;
        bufPainter.setOpacity(opacity);
        bufPainter.drawImage(0,0,dst);
        bufPainter.end();
        dst = buffer;
    }

    if (src.hasAlphaChannel()) {
        dst.setAlphaChannel(src.alphaChannel());
    }

    dst.setDevicePixelRatio(src.devicePixelRatio());

    return dst;
}

static QImage process(const QCImageProviderQueryID& query, QImage image, qreal devicePixelRatio) {
    QImage output = image;

    if (query.scaleToFitDpi) {

        if (output.devicePixelRatio() != devicePixelRatio) {
            qreal ratio = devicePixelRatio / output.devicePixelRatio();
            output = output.scaled(output.width() * ratio, output.height() * ratio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            output.setDevicePixelRatio(devicePixelRatio);
        }

    }

    if (query.tintColor.isValid()) {
        output = colorize(output, query.tintColor);
    }

    return output;
}

/*! \class QCImageProvider
    \inmodule QuickCross

    QCImageProvider is an implementation of QQuickImageProvider that provides an interface for supporint image data in QML.

    This class offeres following features:

    1. Preload all image at startup via QCImageLoader

    2. Support @2x , @3x image format

    3. Perform image transformation by query string.

    3.1. tintColor - Alters the colors of the source item by applying an tint color.

    3.2. scaleToFitDpi - Scale up / down the source image to fit current DPI

Example:

\code

Image {
    // Convert a logo to white color
    source: "image://custom/qt-logo-medium?tintColor=" + escape("#FFFFFF");
}

Image {
    // Scale the image to fit current DPI. It is useful for handling BorderImage
    source: "image://custom/qt-logo-medium?scaleToFitDpi=true"
}

\endcode

Setup:

\code

QCImageLoader *loader = QCImageLoader::instance();
loader->load(":/img"); // preload all image from ":/img"
loader->waitForLoaded();

QQmlApplicationEngine engine;
engine.addImageProvider("custom", new QCImageProvider());

\endcode

 */

QCImageProvider::QCImageProvider() : QQuickImageProvider(QQmlImageProviderBase::Image)
{
    m_cache.setMaxCost(1024 * 1024 * 10);

    QGuiApplication* app = qobject_cast<QGuiApplication*>(QGuiApplication::instance());
    devicePixelRatio = app->devicePixelRatio();
}

QImage QCImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);

    QCImagePool* pool = QCImagePool::instance();
    QImage result;

    QCImageProviderQueryID query;
    query.parse(id);

    if (query.hasModification()) {

        mutex.lock();

        if (m_cache.contains(query.cacheKey)) {
            result = *m_cache.object((query.cacheKey));
        } else if (pool->contains(query.fileName)){
            result = process(query,pool->image(query.fileName), devicePixelRatio);
            m_cache.insert(query.cacheKey, new QImage(result), result.bytesPerLine() * result.height());
        }

        mutex.unlock();

    } else if (pool->contains(query.fileName)) {
        result = pool->image(query.fileName);
    }

    if (!result.isNull()) {
        *size = result.size();
    }

    return result;
}

int QCImageProvider::cacheSize() const
{
    return m_cache.maxCost();
}

void QCImageProvider::setCacheSize(int cacheSize)
{
    m_cache.setMaxCost(cacheSize);
}
