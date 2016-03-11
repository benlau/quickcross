#include <QRunnable>
#include <QThreadPool>
#include <QImageReader>
#include "priv/qcmainthreadrunner.h"
#include "qcimagereader.h"

QCImageReader::QCImageReader(QObject *parent) : QObject(parent)
{
    m_canRead = false;
    m_isFetched = false;
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

        void run() {
            QImageReader reader;
            reader.setFileName(source);

            res["canRead"] = reader.canRead();
            res["size"] = reader.size();

            QCMainThreadRunner::runOnMainThread(Runnable::cleanup, this);
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
    runnable->source = m_source;

    QThreadPool::globalInstance()->start(runnable);
}

void QCImageReader::onFetchFinished(QVariantMap map)
{
    setCanRead(map["canRead"].toBool());
    setSize(map["size"].toSize());
    setIsFetched(true);
}

QString QCImageReader::source() const
{
    return m_source;
}

void QCImageReader::setSource(const QString &source)
{
    m_source = source;
    void sourceChanged();
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
