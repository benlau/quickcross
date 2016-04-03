/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QtCore>
#include <QRunnable>
#include <QThreadPool>
#include <QImageReader>
#include <QtQml>
#include <QQuickImageProvider>
#include "priv/qcmainthreadrunner.h"
#include "priv/qcutils.h"
#include "qcimagereader.h"

/*!
 * \qmltype ImageReader
 * \instantiates QCImageReader
 * \inqmlmodule QuickCross
 * \inherits QObject
 *
 * \brief ImageReader is a wrapper of QImageReader that support asynchronous operation and read from image provider.
 *
 * It is a singleton component
 *
 */

static QImage readFromImageProvider(QQmlEngine* engine, const QString source) {
    QImage res;
    QSize readSize;
    QSize requestSize;
    QPixmap pixmap;
    QQuickTextureFactory* factory = 0;

    QQuickImageProvider *provider = static_cast<QQuickImageProvider *>(engine->imageProvider(QCUtils::imageProviderId(source)));

    if (!provider) {
        return res;
    }

    QString requestId = QCUtils::imageProviderRequestId(source);

    QQuickImageProvider::ImageType type  = provider->imageType();

    switch (type) {
    case QQuickImageProvider::Image:
        res = provider->requestImage(requestId, &readSize, requestSize);
        break;
    case QQuickImageProvider::Pixmap:
        pixmap = provider->requestPixmap(requestId, &readSize, requestSize);
        res = pixmap.toImage();
        break;
    case QQuickImageProvider::Texture:
        factory = provider->requestTexture(requestId, &readSize, requestSize);
        res = factory->image();
        break;
    default:
        break;
    }


    return res;
}

static bool isProviderAvailable(QQmlEngine* engine, const QString& source) {
    QQuickImageProvider *provider = static_cast<QQuickImageProvider *>(engine->imageProvider(QCUtils::imageProviderId(source)));
    return provider != 0;
}

QCImageReader::QCImageReader(QObject *parent) : QCReader(parent)
{
    m_canRead = false;
    m_isFetched = false;
    m_isReady = false;
    m_isError = false;
    m_isFinished = false;
    m_engine = 0;
}

/*!
 * \qmlproperty bool ImageReader::canRead
 *
 * Returns true if an image can be read for the device (i.e., the image format is supported, and the device seems to contain valid data); otherwise returns false.
 *
 * You need to call fetch() before access this property.
 */

bool QCImageReader::canRead() const
{
    return m_canRead;
}

void QCImageReader::setCanRead(bool canRead)
{
    m_canRead = canRead;
    emit canReadChanged();
}

/*! \qmlproperty size ImageReader::size

    Returns the size of the image.

    You need to call fetch() before access this property.

*/


QSize QCImageReader::size() const
{
    return m_size;
}

void QCImageReader::setSize(const QSize &size)
{
    m_size = size;
    emit sizeChanged();
}

/*!
    \qmlmethod void ImageReader::fetch()

    Fetch the information of the image without actually reading the image content. It is asynchronous call.
    Once it is fetched, it will emit fetched() signal and set isFetched property to true.
 */

void QCImageReader::fetch()
{

    class Runnable : public QRunnable {
    public:
        QPointer<QCImageReader> owner;
        QString source;
        QVariantMap res;
        QQmlEngine* engine;

        void run() {

            if (QCUtils::isImageProviderUrl(source)) {

                bool available = isProviderAvailable(engine, source);
                res["canRead"] = available;

                if (!available) {
                    res["errorString"] = "Image provider is not available";
                }

            } else {

                QImageReader reader;
                reader.setFileName(source);

                res["canRead"] = reader.canRead();
                res["size"] = reader.size();
                if (!reader.canRead()) {
                    res["errorString"] = reader.errorString();
                }
            }

            QCMainThreadRunner::start(Runnable::cleanup, this);
        }

        static void cleanup(void *data) {
            Runnable* runnable = (Runnable*) data;
            if (!runnable->owner.isNull()) {
                QMetaObject::invokeMethod(runnable->owner.data(), "onFetchFinished", Qt::DirectConnection,
                                          Q_ARG(QVariantMap, runnable->res));
            }
            delete runnable;
        }
    };

    QCMainThreadRunner::prepare();

    Runnable* runnable = new Runnable();
    runnable->setAutoDelete(false);
    runnable->owner = this;
    runnable->engine = m_engine;
    runnable->source = QCUtils::normalizeResourceUrl(m_source);

    QThreadPool::globalInstance()->start(runnable);
}

/*! \qmlmethod void ImageReader::read()

    Reads an image from the source. On success, it will set the isReady and image property.

    Otherwise, it will set isError to true. You can then access errorString poroperty to find out the type of error that occurred.

 */

void QCImageReader::read()
{
    class Runnable : public QRunnable {
    public:
        QPointer<QCImageReader> owner;
        QString source;
        QVariantMap res;
        QQmlEngine *engine;

        void run() {
            QImageReader reader;
            QImage image;
            reader.setFileName(source);

            if (QCUtils::isImageProviderUrl(source)) {
                QImage image = readFromImageProvider(engine, source);

                if (!image.isNull()) {
                    res["image"] = image;
                } else {
                    res["errorString"] = "Failed to read from image provider";
                }

            } else {
                image = reader.read();

                if (!image.isNull()) {
                    res["image"] = image;
                } else {
                    res["errorString"] = reader.errorString();
                }
            }

            QCMainThreadRunner::start(Runnable::cleanup, this);
        }

        static void cleanup(void *data) {
            Runnable* runnable = (Runnable*) data;
            if (!runnable->owner.isNull()) {
                QMetaObject::invokeMethod(runnable->owner.data(), "onReadImageFinished", Qt::DirectConnection,
                                          Q_ARG(QVariantMap, runnable->res));
            }
            delete runnable;
        }
    };

    QCMainThreadRunner::prepare();

    Runnable* runnable = new Runnable();
    runnable->setAutoDelete(false);
    runnable->owner = this;
    runnable->engine = m_engine;
    runnable->source = QCUtils::normalizeResourceUrl(m_source);

    QThreadPool::globalInstance()->start(runnable);
}

/*! \qmlmethod void ImageReader::clear()

  Clear all the content of property then restore to initial status.

 */

void QCImageReader::clear()
{
    setIsError(false);
    setIsReady(false);
    setIsFetched(false);
    setIsFinished(false);
    setCanRead(false);
    setImage(QImage());
    setSource(QString());
    setSize(QSize());
}

void QCImageReader::onFetchFinished(QVariantMap map)
{
    setCanRead(map["canRead"].toBool());
    setSize(map["size"].toSize());

    QString errorString = map["errorString"].toString();
    if (!errorString.isEmpty()) {
        setIsError(true);
        setErrorString(errorString);
    }

    setIsFetched(true);

    emit fetched();
}

void QCImageReader::onReadImageFinished(QVariantMap map)
{
    if (map.contains("errorString")) {
        setErrorString(map["errorString"].toString());
        setIsError(true);
    } else {
        setImage(map["image"].value<QImage>());
        setIsReady(true);
    }

    finish();
}

void QCImageReader::classBegin()
{

}

void QCImageReader::componentComplete()
{
    QQmlEngine *engine = qmlEngine(this);
    Q_ASSERT(engine);
    m_engine = engine;
}

/*! \qmlproperty QImage ImageReader::image

  This property holds the image read by the reader.
  You need to call read() before access this property

 */

QImage QCImageReader::image() const
{
    return m_image;
}

void QCImageReader::setImage(const QImage &image)
{
    m_image = image;
    emit imageChanged();

    if (image.size() != m_size) {
        setSize(image.size());
    }
}

/*! \qmlproperty url ImageReader::source

  This property holds the location of image.

 */

QString QCImageReader::source() const
{
    return m_source;
}

void QCImageReader::setSource(const QString &source)
{
    if (!source.isNull()) {
        clear();
    }
    m_source = source;
    emit sourceChanged();
}

/*! \qmlproperty bool ImageReader::isFetch

  Returns TRUE if the fetch() function is completed. Otherwise, it will returns false.

 */

bool QCImageReader::isFetched() const
{
    return m_isFetched;
}

void QCImageReader::setIsFetched(bool isFetched)
{
    m_isFetched = isFetched;
    emit isFetchedChanged();
}
