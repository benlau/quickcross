#ifndef QCIMAGEPROVIDER_H
#define QCIMAGEPROVIDER_H

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QQuickImageProvider>
#include <QCache>
#include <QMutex>

class QCImageProvider : public QQuickImageProvider
{
public:
    QCImageProvider();

    virtual QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

    int cacheSize() const;
    void setCacheSize(int cacheSize);

private:
    /// Cache of transformed images
    QCache<QString,QImage> m_cache;

    qreal devicePixelRatio;

    QMutex mutex;
};

#endif // QCIMAGEPROVIDER_H
