#ifndef QCIMAGEPROVIDER_H
#define QCIMAGEPROVIDER_H

#include <QQuickImageProvider>


class QCImageProvider : public QQuickImageProvider
{
public:
    QCImageProvider();

    virtual QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);
};

#endif // QCIMAGEPROVIDER_H
