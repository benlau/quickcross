#include <QtCore>
#include <QRunnable>
#include <QThreadPool>
#include <QImageReader>
#include <QtQml>
#include <QQuickImageProvider>
#include "priv/qcmainthreadrunner.h"
#include "priv/qcutils.h"
#include "qcimagereader.h"

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

QCImageReader::QCImageReader(QObject *parent) : QObject(parent)
{
    m_canRead = false;
    m_isFetched = false;
    m_isReady = false;
    m_isError = false;
    m_isCompleted = false;
    m_engine = 0;
}

bool QCImageReader::canRead() const
{
    return m_canRead;
}

void QCImageReader::setCanRead(bool canRead)
{
    m_canRead = canRead;
    emit canReadChanged();
}

QSize QCImageReader::size() const
{
    return m_size;
}

void QCImageReader::setSize(const QSize &size)
{
    m_size = size;
    emit sizeChanged();
}

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

void QCImageReader::clear()
{
    setIsError(false);
    setIsReady(false);
    setIsFetched(false);
    setIsCompleted(false);
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

    emit completed();
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

bool QCImageReader::isError() const
{
    return m_isError;
}

void QCImageReader::setIsError(bool isError)
{
    m_isError = isError;
    emit isErrorChanged();
}

bool QCImageReader::isCompleted() const
{
    return m_isCompleted;
}

void QCImageReader::setIsCompleted(bool isCompleted)
{
    m_isCompleted = isCompleted;
    emit isCompletedChanged();
}

QString QCImageReader::errorString() const
{
    return m_errorString;
}

void QCImageReader::setErrorString(const QString &errorString)
{
    m_errorString = errorString;
    emit errorStringChanged();
}

bool QCImageReader::isReady() const
{
    return m_isReady;
}

void QCImageReader::setIsReady(bool isReady)
{
    m_isReady = isReady;
    emit isReadyChanged();
}

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

bool QCImageReader::isFetched() const
{
    return m_isFetched;
}

void QCImageReader::setIsFetched(bool isFetched)
{
    m_isFetched = isFetched;
    emit isFetchedChanged();
}
