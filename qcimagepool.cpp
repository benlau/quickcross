#include <QCoreApplication>
#include <QMutexLocker>
#include <QFileInfo>
#include "qcimagepool.h"

static QString removeSuffix(const QString& fileName) {
    QFileInfo info(fileName);

    return info.baseName();
}

static QString normalizeKey(const QString &key) {
    return removeSuffix(key).toLower();
}

QCImagePool::QCImagePool(QObject* parent) : QObject(parent)
{

}

int QCImagePool::count() const
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    return m_images.count();
}

void QCImagePool::clear()
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    m_images.clear();
}

bool QCImagePool::contains(const QString &key) const
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    return m_images.contains(normalizeKey(key));
}

QImage QCImagePool::image(const QString &key) const
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    QString k = normalizeKey(key);
    QImage res;

    if (m_images.contains(k)) {
        res = m_images[k];
    }

    return res;
}

void QCImagePool::insert(const QString &key, const QImage &image)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    QString k = normalizeKey(key);

    m_images[k] = image;
}

QCImagePool *QCImagePool::instance()
{
    static QCImagePool* m_instance = 0;

    if (!m_instance) {
        QCoreApplication* app = QCoreApplication::instance();

        m_instance = new QCImagePool(app);
    }

    return m_instance;
}
