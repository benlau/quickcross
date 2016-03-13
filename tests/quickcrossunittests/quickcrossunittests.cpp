#include <QQmlApplicationEngine>
#include <QtCore>
#include <QTest>
#include <QImageReader>
#include "qcdevice.h"
#include "quickcrossunittests.h"
#include "qcimageloader.h"
#include "automator.h"
#include "qcimageprovider.h"
#include "qcimagereader.h"
#include "priv/qcmainthreadrunner.h"
#include "priv/qcutils.h"

QuickCrossUnitTests::QuickCrossUnitTests()
{
}

void QuickCrossUnitTests::onWarning(const QList<QQmlError> &warnings)
{
    qDebug() << "on warning";
    this->warnings = warnings;
}

void QuickCrossUnitTests::initTestCase()
{
}

void QuickCrossUnitTests::cleanupTestCase()
{
}

void QuickCrossUnitTests::utils()
{
    QVERIFY(QCUtils::isResourceUrl("qrc:///test"));
    QVERIFY(QCUtils::isResourceUrl(":test"));
    QVERIFY(!QCUtils::isResourceUrl("/test"));

    QVERIFY(QCUtils::isImageProviderUrl("image://camera/1"));
    QVERIFY(!QCUtils::isImageProviderUrl("/test"));
}

void QuickCrossUnitTests::loader()
{
    QQmlApplicationEngine engine;

    connect(&engine,SIGNAL(warnings(QList<QQmlError>)),
            this,SLOT(onWarning(QList<QQmlError>)));
    QString source = QString(SRCDIR) + "/qml/loader.qml";

    engine.load(QUrl::fromLocalFile(source));

    QVERIFY(engine.rootObjects().size() > 0);

    Q_ASSERT(warnings.size() == 0);
}

void QuickCrossUnitTests::device()
{
    QCDevice* device = new QCDevice();

#ifdef Q_OS_LINUX
    QVERIFY(device->os() == "linux");
#endif

#ifdef Q_OS_MAC
    QVERIFY(device->os() == "osx");
#endif

    delete device;
}

void QuickCrossUnitTests::imageLoader()
{
    QCImageLoader *loader = QCImageLoader::instance();

    // Clear loaded images
    loader->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(loader->count() == 0);

    loader->load(QString(SRCDIR) + "img");
    QVERIFY(loader->count() == 0); // Not loaded yet

    QVERIFY(loader->running());
    QVERIFY(!loader->isLoaded());

    QVERIFY(loader->waitForLoaded(10000));

    QVERIFY(!loader->running());

    QCOMPARE(loader->count(), 1);

    Automator::waitUntil(loader, "isLoaded", true);
    QVERIFY(loader->isLoaded());


    QVERIFY(loader->contains("qt-logo-medium.png"));
    QVERIFY(loader->contains("qt-logo-medium"));
    QVERIFY(!loader->contains("not-existed"));

    QImage image = loader->image("qt-logo-medium.png");
    QCOMPARE(image.width(), 381);
    QCOMPARE(image.height(), 500);

    QVERIFY (loader->image("qt-logo-medium") == image);
}

void QuickCrossUnitTests::imageLoader_qrc()
{
    QCImageLoader *loader = QCImageLoader::instance();

    // Clear loaded images
    loader->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(loader->count() == 0);

    loader->load(":unittests/img");
    QVERIFY(loader->count() == 0); // Not loaded yet

    loader->waitForLoaded();
    QCOMPARE(loader->count(), 1);

    loader->clear();

    loader->load("qrc:///unittests/img");
    QVERIFY(loader->count() == 0); // Not loaded yet

    loader->waitForLoaded();
    QCOMPARE(loader->count(), 1);
}

void QuickCrossUnitTests::imageProvider()
{
    QCImageLoader *loader = QCImageLoader::instance();

    // Clear loaded images
    loader->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(loader->count() == 0);

    loader->load(QString(SRCDIR) + "img");
    QVERIFY(loader->count() == 0); // Not loaded yet

    QVERIFY(loader->running());
    QVERIFY(!loader->isLoaded());

    Automator::waitUntil(loader, "running", false);


    QCOMPARE(loader->count(), 1);

    Automator::waitUntil(loader, "isLoaded", true);
    QVERIFY(loader->isLoaded());


    QQmlApplicationEngine engine;

    engine.addImageProvider("custom", new QCImageProvider());

    connect(&engine,SIGNAL(warnings(QList<QQmlError>)),
            this,SLOT(onWarning(QList<QQmlError>)));
    QString source = QString(SRCDIR) + "/qml/imageProvider.qml";

    engine.load(QUrl::fromLocalFile(source));


    QVERIFY(engine.rootObjects().size() > 0);
    Automator automator(&engine);

    QObject* image = automator.findObject("image");
    QVERIFY(image);
    QSize size = image->property("sourceSize").value<QSize>();
    QCOMPARE(size.width(), 381);
    QCOMPARE(size.height(), 500);

    Q_ASSERT(warnings.size() == 0);

}

void QuickCrossUnitTests::mainThreadRunner()
{
    QCMainThreadRunner::prepare();

    static bool success = false;

    class Inner {
    public:
        static void test(void* data) {
            Q_UNUSED(data);
            success = QThread::currentThread() == QCoreApplication::instance()->thread();
        }
    };

    class Runnable : public QRunnable {
    public:
        virtual void run() {
            QCMainThreadRunner::start(Inner::test, 0);
        }
    };

    Runnable* runnable = new Runnable();
    runnable->setAutoDelete(true);
    QThreadPool::globalInstance()->start(runnable);

    Automator::wait(100);
    QVERIFY(success);
}

void QuickCrossUnitTests::imageReader()
{
    QImageReader reader;
    reader.setFileName(":/unittests/img/qt-logo-medium.png");

    QVERIFY(reader.canRead());

    QQmlApplicationEngine engine;
    Automator automator(&engine);

    engine.addImageProvider("custom", new QCImageProvider());

    connect(&engine,SIGNAL(warnings(QList<QQmlError>)),
            this,SLOT(onWarning(QList<QQmlError>)));

    QString source = QString(SRCDIR) + "/qml/imageReader.qml";

    engine.load(QUrl::fromLocalFile(source));

    QVERIFY(engine.rootObjects().size() > 0);

    QCImageReader* reader1 = qobject_cast<QCImageReader*>(automator.findObject("reader1"));
    QVERIFY(reader1);

    QVERIFY(!reader1->isFetched());

    reader1->fetch();

    QVERIFY(automator.waitUntil(reader1, "isFetched", true));

    QVERIFY(reader1->size() == QSize(381,500));

    reader1->read();

    automator.waitUntil(reader1,"isReady", true);

    QImage image = reader1->image();
    QVERIFY(image.size() == QSize(381,500));

    Q_ASSERT(warnings.size() == 0);
}
