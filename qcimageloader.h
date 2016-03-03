#ifndef QCIMAGELOADER_H
#define QCIMAGELOADER_H

#include <QObject>
#include <QImage>
#include <QMap>

/// Load all images from a folder

class QCImageLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit QCImageLoader(QObject *parent = 0);

    static QCImageLoader* instance();

    int count() const;

    void clear();

    // Load an image from path
    void load(QString path);

    bool running() const;

    QImage image(QString name) const;

    bool contains(QString key) const;

signals:
    void runningChanged();

public slots:

private slots:
    void onFinished(const QVariant& result);

private:
    void updateRunning();
    void realLoad(QString path);

    int m_count;

    bool m_running;
    QMap<QString, QImage> m_images;

    // no. of pending queue
    int m_pending;

};

#endif // QCIMAGELOADER_H
