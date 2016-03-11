#ifndef QCIMAGEPROVIDER_H
#define QCIMAGEPROVIDER_H

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QQuickImageProvider>

class QCImageProvider : public QQuickImageProvider
{
public:
    QCImageProvider();

    virtual QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);
};

#endif // QCIMAGEPROVIDER_H
