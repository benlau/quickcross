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
#include "qcimageloader.h"
#include "priv/qcmainthreadrunner.h"
#include "priv/qcimageloader_p.h"

static QString removeSuffix(QString fileName) {
    //@TODO - handle directory with "."

    QStringList token = fileName.split(".");

    return token[0];
}

static QString genKey(QString name) {
    return removeSuffix(name).toLower();
}

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

QCImageLoader::QCImageLoader(QObject *parent) : QObject(parent)
{
    m_count = 0;
    m_pending = 0;
    m_running = false;
    m_isLoaded = false;
}

QCImageLoader* QCImageLoader::instance()
{
    static QCImageLoader* m_instance = 0;

    if (!m_instance) {
        QCoreApplication* app = QCoreApplication::instance();

        m_instance = new QCImageLoader(app);

    }

    return m_instance;
}

int QCImageLoader::count() const
{
    return m_images.count();
}

void QCImageLoader::clear()
{
    m_images.clear();
    m_isLoaded = false;
}

void QCImageLoader::load(QString path)
{
    class Runnable : public QRunnable {
    public:
        QPointer<QCImageLoader> owner;
        QString root;
        QMap<QString, QImage> images;
        qreal devicePixelRatio;

        void run() {
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
                        key = genKey(key);
                        image.setDevicePixelRatio(devicePixelRatio);
                        images[key] = image;
                    }
                }
            }

            QCMainThreadRunner::start(Runnable::cleanup, this);
        }

        static void cleanup(void *data) {
            Runnable* runnable = (Runnable*) data;

            if (!runnable->owner.isNull()) {
                QMetaObject::invokeMethod(runnable->owner.data(), "onFinished", Qt::DirectConnection,
                                          Q_ARG(QVariant , QVariant::fromValue(runnable->images)));
            }

            delete runnable;
        }
    };

    QUrl url(path);
    if (url.scheme() == "qrc") {
        path = QString(":") + url.path();
    }

    QCMainThreadRunner::prepare();
    Runnable *runnable = new Runnable();
    runnable->setAutoDelete(false);
    runnable->owner = this;
    runnable->root = path;
    runnable->devicePixelRatio = qobject_cast<QGuiApplication*>(QGuiApplication::instance())->devicePixelRatio();

    QThreadPool* pool = QThreadPool::globalInstance();
    pool->start(runnable);
    m_pending++;
    updateRunning();
}

bool QCImageLoader::running() const
{
    return m_running;
}

QImage QCImageLoader::image(QString name) const
{
    QString key = genKey(name);

    QImage res;
    if (m_images.contains(key)) {
        res = m_images[key];
    }

    return res;
}

bool QCImageLoader::contains(QString name) const
{
    return m_images.contains(genKey(name));
}

void QCImageLoader::onFinished(const QVariant &result)
{
    QMap<QString,QImage> images = result.value<QMap<QString, QImage> >();

    QMap<QString,QImage>::Iterator iter = images.begin();

    while (iter != images.end()) {
        m_images.insert(iter.key(), iter.value());
        iter++;
    }

    m_pending--;
    updateRunning();

    if (!m_isLoaded) {
        setIsLoaded(true);
        emit loaded();
    }
}

void QCImageLoader::updateRunning()
{
    bool value = m_pending > 0 ? true: false;

    if (value != m_running) {
        m_running = value;
        emit runningChanged();
    }
}

bool QCImageLoader::isLoaded() const
{
    return m_isLoaded;
}

bool QCImageLoader::waitForLoaded(int timeout)
{
    if (m_isLoaded) {
        return true;
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

    return m_isLoaded;
}

void QCImageLoader::setIsLoaded(bool isLoaded)
{
    m_isLoaded = isLoaded;
    emit isLoadedChanged();
}

