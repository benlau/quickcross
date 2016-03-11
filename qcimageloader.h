#ifndef QCIMAGELOADER_H
#define QCIMAGELOADER_H

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */


#include <QObject>
#include <QImage>
#include <QMap>

/// Load all images from a folder

class QCImageLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY isLoadedChanged)

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

    bool isLoaded() const;

    bool waitForLoaded(int timeout = 10000);

signals:
    void runningChanged();
    void isLoadedChanged();
    void loaded();

public slots:

private slots:
    void onFinished(const QVariant& result);

private:
    void updateRunning();
    void realLoad(QString path);
    void setIsLoaded(bool isLoaded);

    int m_count;

    bool m_running;
    QMap<QString, QImage> m_images;

    // no. of pending queue
    int m_pending;
    bool m_isLoaded;

};

#endif // QCIMAGELOADER_H
