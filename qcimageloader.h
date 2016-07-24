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

    bool running() const;

    // Return true if it has loaded any images
    bool isLoaded() const;

    void waitForLoaded(int timeout = 10000);

signals:
    void runningChanged();
    void isLoadedChanged();
    void loaded();

public slots:

    // Load an image from path
    void load(const QString &path);


private slots:
    void onFinished();

private:
    void updateRunning();
    void realLoad(QString path);
    void setIsLoaded(bool isLoaded);

    int m_count;

    bool m_running;

    // no. of pending queue
    int m_pending;
    bool m_isLoaded;

};

#endif // QCIMAGELOADER_H
