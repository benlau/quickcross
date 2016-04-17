/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QtCore>
#include <QUrlQuery>
#include <QGuiApplication>
#include "qcimageprovider.h"
#include "qcimageloader.h"

class QCImageProviderQueryID {
public:

    QCImageProviderQueryID() {
        scaleToFitDpi = false;
    }

    bool hasModification() const {
        return scaleToFitDpi;
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

        outputUrl.setQuery(outputQuery.query());
        cacheKey = outputUrl.toString();
    }

    QString fileName;
    bool scaleToFitDpi;
    QString cacheKey;
};

static QImage process(const QCImageProviderQueryID& query, QImage image, qreal devicePixelRatio) {
    QImage output = image;

    if (query.scaleToFitDpi) {

        if (output.devicePixelRatio() != devicePixelRatio) {
            qreal ratio = devicePixelRatio / output.devicePixelRatio();
            output = output.scaled(output.width() * ratio, output.height() * ratio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            output.setDevicePixelRatio(devicePixelRatio);
        }

    }

    return output;
}

QCImageProvider::QCImageProvider() : QQuickImageProvider(QQmlImageProviderBase::Image)
{
    m_cache.setMaxCost(1024 * 1024 * 10);

    QGuiApplication* app = qobject_cast<QGuiApplication*>(QGuiApplication::instance());
    devicePixelRatio = app->devicePixelRatio();
}

QImage QCImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);

    QCImageLoader* loader = QCImageLoader::instance();
    QImage result;

    QCImageProviderQueryID query;
    query.parse(id);

    if (query.hasModification()) {

        mutex.lock();

        if (m_cache.contains(query.cacheKey)) {
            result = *m_cache.object((query.cacheKey));
        } else if (loader->contains(query.fileName)){
            result = process(query,loader->image(query.fileName), devicePixelRatio);
            m_cache.insert(query.cacheKey, new QImage(result), result.bytesPerLine() * result.height());
        }

        mutex.unlock();

    } else if (loader->contains(query.fileName)) {
        result = loader->image(query.fileName);
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
