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
#include "priv/qcimageloader_p.h"

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

    QVERIFY(QCUtils::normalizeResourceUrl("qrc:///test") == ":/test");
    QVERIFY(QCUtils::normalizeResourceUrl(":test") == ":test");
    QVERIFY(QCUtils::normalizeResourceUrl("/test") == "/test");

    QVERIFY(QCUtils::isImageProviderUrl("image://camera/1"));
    QVERIFY(!QCUtils::isImageProviderUrl("/test"));

    QVERIFY(QCUtils::imageProviderId("image://custom/1123") == "custom");


    QVERIFY(QCUtils::normalizeLocalUrl("qrc:///test") == ":/test");
    QVERIFY(QCUtils::normalizeLocalUrl(":test") == ":test");
    QVERIFY(QCUtils::normalizeLocalUrl("/test") == "/test");
    QVERIFY(QCUtils::normalizeLocalUrl("file:///test") == "/test");

    QQmlApplicationEngine engine;

    QJSValue loader = QCUtils::loadJavascript(&engine, "qrc:///unittests/js/loader.js" , "create");

    QVERIFY(!loader.isError());
    QVERIFY(loader.isCallable());

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

    QCOMPARE(loader->count(), 2);

    Automator::waitUntil(loader, "isLoaded", true);
    QVERIFY(loader->isLoaded());


    QVERIFY(loader->contains("qt-logo-medium.png"));
    QVERIFY(loader->contains("qt-logo-medium"));
    QVERIFY(!loader->contains("not-existed"));

    QImage image = loader->image("qt-logo-medium.png");
    QCOMPARE(image.width(), 381);
    QCOMPARE(image.height(), 500);
    QVERIFY(image.devicePixelRatio() == 1);

    QVERIFY (loader->image("qt-logo-medium") == image);

    QVERIFY(loader->contains("button3"));

    QVERIFY(loader->image("button3").devicePixelRatio() == 3);
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
    QCOMPARE(loader->count(), 2);

    loader->clear();

    loader->load("qrc:///unittests/img");
    QVERIFY(loader->count() == 0); // Not loaded yet

    loader->waitForLoaded();
    QCOMPARE(loader->count(), 2);
}

void QuickCrossUnitTests::imageLoader_filter()
{
    QString normalizedFile;
    qreal ratio = 0;

    normalizedFile = qcImageLoaderDecodeFileName("test@3x.png", &ratio);

    QVERIFY(normalizedFile == "test.png");
    QVERIFY(ratio == 3);


    QStringList input, output;

    input << "test@1.jpg" << "test@2.jpg";

    output = qcImageLoaderFilter(input, 3);
    QCOMPARE(output.size(), 2);

    input.clear();
    input << "test@1x.jpg" << "test@2x.jpg";
    output = qcImageLoaderFilter(input, 3);
    QCOMPARE(output.size(), 1);

    QVERIFY(output.at(0) == "test@2x.jpg");

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


    QCOMPARE(loader->count(), 2);

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
    //Read from absolute path

    QCImageLoader* loader = QCImageLoader::instance();
    loader->clear();
    loader->load(QString(SRCDIR) + "img");
    QVERIFY(loader->waitForLoaded(10000));

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

    QVERIFY(Automator::waitUntil(reader1,"isReady", true));

    QImage image = reader1->image();
    QVERIFY(image.size() == QSize(381,500));

    //Read from resource path

    reader1->setSource("qrc:///unittests/img/qt-logo-medium.png");

    QVERIFY(!reader1->isFetched());
    QVERIFY(!reader1->isError());
    QVERIFY(!reader1->isFinished());
    QVERIFY(!reader1->isReady());
    QVERIFY(!reader1->canRead());
    QVERIFY(reader1->size() == QSize());
    QVERIFY(reader1->image().isNull());

    reader1->fetch();
    QVERIFY(Automator::waitUntil(reader1, "isFetched", true));

    QVERIFY(reader1->size() == QSize(381,500));

    reader1->read();
    QVERIFY(Automator::waitUntil(reader1, "isReady", true));
    QVERIFY(!reader1->image().isNull());
    QVERIFY(image.size() == QSize(381,500));

    // Image Provider
    reader1->setSource("image://custom/qt-logo-medium");

    reader1->fetch();
    QVERIFY(Automator::waitUntil(reader1, "isFetched", true));
    QVERIFY(reader1->canRead());

    // Size is not available from image provider without really read it.
    QVERIFY(reader1->size() == QSize());

    reader1->read();
    QVERIFY(Automator::waitUntil(reader1, "isReady", true));
    QVERIFY(!reader1->image().isNull());
    QVERIFY(image.size() == QSize(381,500));
    QVERIFY(reader1->size() == QSize(381,500));

    Q_ASSERT(warnings.size() == 0);
}
