#ifndef QCIMAGEPOOL_H
#define QCIMAGEPOOL_H

#include <QMap>
#include <QImage>
#include <QMutex>
#include <QObject>

class QCImagePool : public QObject
{
public:

    int count() const;

    void clear();

    bool contains(const QString& key) const;

    QImage image(const QString& key) const;

    void insert(const QString &key, const QImage& image);

    static QCImagePool* instance();

protected:
    QCImagePool(QObject* parent = 0);

private:

    QMap<QString, QImage> m_images;

    mutable QMutex mutex;
};

#endif // QCIMAGEPOOL_H
