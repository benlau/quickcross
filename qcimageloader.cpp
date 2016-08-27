/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QGuiApplication>
#include <QRunnable>
#include <QPointer>
#include <QThreadPool>
#include <QImageReader>
#include <QVariant>
#include <QtCore>
#include <QQueue>
#include <QEventLoop>
#include <QtConcurrent>
#include "QCImagePool"
#include "qcimageloader.h"
#include "priv/qcimageloader_p.h"
#include "QCMainThreadRunner"

static QStringList convert(QFileInfoList &input) {
    QStringList res;

    for (int i = 0 ; i < input.size() ; i++) {
        QFileInfo info = input.at(i);
        if (info.fileName() == "." || info.fileName() == "..")
            continue;
        res << info.absoluteFilePath();
    }

    return res;
}

/*! \class QCImageLoader
    \inmodule QuickCross

    QCImageLoader is an asynchronous image loader. It load images from a folder to QCImagePool
 */

QCImageLoader::QCImageLoader(QObject *parent) : QObject(parent)
{
    m_count = 0;
    m_pending = 0;
    m_running = false;
    m_isLoaded = false;
}

/*! \fn void QCImageLoader::load(QString path)

  Load images on target path. It will be executed in thread with QThreadPool.
  Once it is loaded, it will emit the loaded signal

 */

void QCImageLoader::load(const QString &path)
{
    QPointer<QCImageLoader> thiz = this;
    qreal devicePixelRatio = qobject_cast<QGuiApplication*>(QGuiApplication::instance())->devicePixelRatio();
    QString root = path;

    QUrl url(root);
    if (url.scheme() == "qrc") {
        root = QString(":") + url.path();
    }

    auto func = [=] {

        QStringList filters;

        QQueue<QString> queue;

        queue.enqueue(root);
        filters << "*.png" << "*.jpg";

        while (queue.size() > 0) {
            //@TODO - Load sub-folder
            QString current = queue.dequeue();
            QDir dir(current);
            QFileInfoList infos = dir.entryInfoList(filters,QDir::Files);
            QStringList files = convert(infos);

            files = qcImageLoaderFilter(files, devicePixelRatio);

            for (int i = 0 ; i < files.size();i++) {
                QImageReader reader;
                reader.setFileName(files.at(i));

                QImage image = reader.read();

                if (image.isNull()) {
                    qWarning() << reader.errorString();
                } else {
                    qreal devicePixelRatio = 1;
                    QString key = qcImageLoaderDecodeFileName(files.at(i), &devicePixelRatio);
                    key.remove(0,root.size() + 1);
                    image.setDevicePixelRatio(devicePixelRatio);
                    QCImagePool::instance()->insert(QCImagePool::normalizeKey(key), image);
                }
            }
        }

        QCMainThreadRunner::run([=](){
            if (thiz.isNull()) {
                return;
            }
            QMetaObject::invokeMethod(thiz.data(), "onFinished", Qt::DirectConnection);
        });
    };

    m_pending++;
    updateRunning();

    QtConcurrent::run(func);
}

/*! \property QCImageLoader::running
    \brief The property indicate whether is the loader running.
 */

bool QCImageLoader::running() const
{
    return m_running;
}

void QCImageLoader::onFinished()
{
    m_pending--;
    updateRunning();

    if (!m_isLoaded) {
        setIsLoaded(true);
    }

    emit loaded();
}

void QCImageLoader::updateRunning()
{
    bool value = m_pending > 0 ? true: false;

    if (value != m_running) {
        m_running = value;
        emit runningChanged();
    }
}
/*! \property QCImageLoader::isLoaded
    \brief The property indicate whether is the loader loaded anything.
 */

bool QCImageLoader::isLoaded() const
{
    return m_isLoaded;
}

/*! \fn void QCImageLoader::waitForLoaded(int timeout)

  Block the execution of current program and wait until images loaded.
 */

void QCImageLoader::waitForLoaded(int timeout)
{
    if (!m_running) {
        return;
    }

    QTimer timer;
    timer.setInterval(timeout);

    QEventLoop loop;
    connect(&timer,SIGNAL(timeout()),
            &loop, SLOT(quit()));
    connect(this, SIGNAL(loaded()),
            &loop, SLOT(quit()));
    timer.start();
    loop.exec();
}

void QCImageLoader::setIsLoaded(bool isLoaded)
{
    m_isLoaded = isLoaded;
    emit isLoadedChanged();
}

