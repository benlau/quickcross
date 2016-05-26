/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */
#include <QCoreApplication>
#include <QMutexLocker>
#include <QFileInfo>
#include "qcimagepool.h"

static QString removeSuffix(const QString& fileName) {
    QFileInfo info(fileName);

    return info.baseName();
}

/*! \class QCImagePool
    \inmodule QuickCross

    QCImagePool is a singleton component for holding images data.

    The images loaded by QCImageLoader will be saved in this component, and allow to be read by QCImageProvider.

    All the functions within QCImagePool are thread safe.
 */

QCImagePool::QCImagePool(QObject* parent) : QObject(parent)
{

}

/*! \fn int QCImagePool::count() const

  Return number of images in QCImagePool
 */

int QCImagePool::count() const
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    return m_images.count();
}

/*! \fn void QCImagePool::clear()

  Remove all the images
 */


void QCImagePool::clear()
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    m_images.clear();
}

/*! \fn void QCImagePool::contains(const QString &key)  const

  Return TRUE if the image pool contains the image with key.
 */

bool QCImagePool::contains(const QString &key) const
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    return m_images.contains(normalizeKey(key));
}

/*! \fn QImage QCImagePool::image(const QString &key) const

  Get the image with key. If it is not existed, it will return an empty image.
 */

QImage QCImagePool::image(const QString &key) const
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    QImage res;

    if (m_images.contains(key)) {
        res = m_images[key];
    }

    return res;
}

/*! void QCImagePool::insert(const QString &key, const QImage &image)

  Insert the image with key into the image pool.

 */
void QCImagePool::insert(const QString &key, const QImage &image)
{
    QMutexLocker locker(&mutex);
    Q_UNUSED(locker);

    m_images[key] = image;
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

QString QCImagePool::normalizeKey(const QString &key)
{
    return removeSuffix(key).toLower();
}
