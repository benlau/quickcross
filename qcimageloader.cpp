#include <QCoreApplication>
#include <QRunnable>
#include <QPointer>
#include <QThreadPool>
#include <QImageReader>
#include <QVariant>
#include <QtCore>
#include <QQueue>
#include <QEventLoop>
#include "qcimageloader.h"


static QString removeSuffix(QString fileName) {
    //@TODO - handle directory with "."

    QStringList token = fileName.split(".");

    return token[0];
}

static QString genKey(QString name) {
    return removeSuffix(name).toLower();
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

        void run() {
            QStringList filters;
            QQueue<QString> queue;
            QMap<QString, QImage> images;

            queue.enqueue(root);
            filters << "*.png" << "*.jpg";

            while (queue.size() > 0) {
                //@TODO - Load sub-folder
                QString current = queue.dequeue();
                QDir dir(current);
                QFileInfoList infos = dir.entryInfoList(filters,QDir::Files);

                for (int i = 0 ; i < infos.size();i++) {
                    QFileInfo info = infos.at(i);
                    if (info.fileName() == "." || info.fileName() == "..")
                        continue;

                    QImageReader reader;
                    reader.setFileName(info.absoluteFilePath());

                    QImage image = reader.read();

                    if (image.isNull()) {
                        qWarning() << reader.errorString();
                    } else {
                        QString key = info.absoluteFilePath();
                        key.remove(0,root.size() + 1);
                        key = genKey(key);
                        images[key] = image;
                    }
                }
            }

            if (!owner.isNull()) {
                QMetaObject::invokeMethod(owner.data(), "onFinished", Qt::QueuedConnection,
                                          Q_ARG(QVariant , QVariant::fromValue(images)));
            }

        }
    };

    Runnable *runnable = new Runnable();
    runnable->setAutoDelete(true);
    runnable->owner = this;
    runnable->root = path;

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

