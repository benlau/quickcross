#include <QQmlApplicationEngine>
#include <QtCore>
#include <QTest>
#include <QImageReader>
#include <QtConcurrent>
#include "QCImagePool"
#include "testrunner.h"
#include "qcdevice.h"
#include "quickcrossunittests.h"
#include "qcimageloader.h"
#include "automator.h"
#include "qcimageprovider.h"
#include "qcimagereader.h"
#include "priv/qcutils.h"
#include "priv/qcimageloader_p.h"
#include "QCMainThreadRunner"

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

    QVERIFY(QCUtils::find(".").size() > 0);
    QVERIFY(QCUtils::find(QDir::current().path()).size() > 0);

    QVERIFY(QCUtils::find("..").size() > 0);

    QStringList files = QCUtils::find("..", QStringList() << "*.h");

    QVERIFY(files.filter(QRegExp("*.h",Qt::CaseInsensitive,QRegExp::Wildcard)).size() == files.size());

    /* rmdir */
    QDir dir("tmp");
    QString tmpPath = dir.absolutePath();;

    if (dir.exists()) {
        dir.removeRecursively();
    }

    dir.mkpath(tmpPath);

    qDebug() << tmpPath;
    QVERIFY(dir.exists());
    QVERIFY(QCUtils::rmdir(tmpPath));
    QVERIFY(!dir.exists());

    dir.mkdir(tmpPath);
    QFile file(tmpPath + "/files.txt");
    file.open(QIODevice::WriteOnly);
    file.close();

    QVERIFY(!QCUtils::rmdir(tmpPath));
    QVERIFY(dir.exists());

    QVERIFY(QCUtils::rmdir(tmpPath,true));
    QVERIFY(!dir.exists());

    /* touch */
    QFileInfo info("tmp.txt");
    if (info.exists()) {
        QFile::remove("tmp.txt");
    }

    info = QFileInfo("tmp.txt");
    QVERIFY(!info.exists());

    QCUtils::touch("tmp.txt");

    info = QFileInfo("tmp.txt");
    QVERIFY(info.exists());

    QVERIFY(QCUtils::touch("tmp.txt"));

    /* rm */
    QVERIFY(QCUtils::rm("tmp.txt"));

    info = QFileInfo("tmp.txt");
    QVERIFY(!info.exists());

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

void QuickCrossUnitTests::imagePool()
{
    QVERIFY(QCImagePool::normalizeKey("example") == "example");
    QVERIFY(QCImagePool::normalizeKey("example.png") == "example");
    QVERIFY(QCImagePool::normalizeKey("example?scaleToDpi=true") == "example?scaletodpi=true");
}

void QuickCrossUnitTests::imageLoader()
{
    QScopedPointer<QCImageLoader> loader(new QCImageLoader());
    QCImagePool* pool = QCImagePool::instance();

    // Clear loaded images
    pool->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(pool->count() == 0);

    loader->load(QString(SRCDIR) + "img");
    QVERIFY(pool->count() == 0); // Not loaded yet

    QVERIFY(loader->running());
    QVERIFY(!loader->isLoaded());

    loader->waitForLoaded(10000);

    QVERIFY(!loader->running());

    QCOMPARE(pool->count(), 2);

    Automator::waitUntil(loader.data(), "isLoaded", true);
    QVERIFY(loader->isLoaded());


    QVERIFY(pool->contains("qt-logo-medium.png"));
    QVERIFY(pool->contains("qt-logo-medium"));
    QVERIFY(!pool->contains("not-existed"));

    QImage image = pool->image(QCImagePool::normalizeKey("qt-logo-medium.png"));
    QCOMPARE(image.width(), 381);
    QCOMPARE(image.height(), 500);
    QVERIFY(image.devicePixelRatio() == 1);

    QVERIFY (pool->image("qt-logo-medium") == image);

    QVERIFY(pool->contains("button3"));

    QVERIFY(pool->image("button3").devicePixelRatio() == 3);
}

void QuickCrossUnitTests::imageLoader_qrc()
{
    QScopedPointer<QCImageLoader> loader(new QCImageLoader());
    QCImagePool* pool = QCImagePool::instance();

    // Clear loaded images
    pool->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(pool->count() == 0);

    loader->load(":unittests/img");
    QVERIFY(pool->count() == 0); // Not loaded yet

    loader->waitForLoaded();
    QCOMPARE(pool->count(), 2);

    pool->clear();

    loader->load("qrc:///unittests/img");
    QVERIFY(pool->count() == 0); // Not loaded yet

    loader->waitForLoaded();
    QCOMPARE(pool->count(), 2);
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

void QuickCrossUnitTests::imageLoader_destroyAtRunning()
{
    QCImageLoader* loader = new QCImageLoader();
    QCImagePool* pool = QCImagePool::instance();
    pool->clear();

    loader->load(QString(SRCDIR) + "img");
    QVERIFY(pool->count() == 0); // Not loaded yet

    QVERIFY(loader->running());
    QVERIFY(!loader->isLoaded());

    delete loader;

    Automator::wait(500);
}

void QuickCrossUnitTests::imageProvider()
{
    QScopedPointer<QCImageLoader> loader(new QCImageLoader());
    QCImagePool* pool = QCImagePool::instance();

    // Clear loaded images
    pool->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(pool->count() == 0);

    loader->load(QString(SRCDIR) + "img");
    QVERIFY(pool->count() == 0); // Not loaded yet

    QVERIFY(loader->running());
    QVERIFY(!loader->isLoaded());

    Automator::waitUntil(loader.data(), "running", false);

    QCOMPARE(pool->count(), 2);
    QVERIFY(pool->contains("button3"));

    Automator::waitUntil(loader.data(), "isLoaded", true);
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

    image = automator.findObject("image2");
    QVERIFY(image);

    size = image->property("sourceSize").value<QSize>();
    QCOMPARE(size.width(), 258 / 3);
    QCOMPARE(size.height(), 150 / 3);


    TestRunner* runner = TestRunner::defautInstance();
    int waitTime = runner->config()["waitTime"].toInt();

    Automator::wait(waitTime);

    Q_ASSERT(warnings.size() == 0);

}

void QuickCrossUnitTests::imageProvider_tintcolor()
{
    QScopedPointer<QCImageLoader> loader(new QCImageLoader());
    QCImagePool* pool = QCImagePool::instance();

    // Clear loaded images
    pool->clear();
    QVERIFY(!loader->isLoaded());

    // No. of loaded image = 0
    QVERIFY(pool->count() == 0);

    loader->load(QString(SRCDIR) + "img");
    QVERIFY(pool->count() == 0); // Not loaded yet

    QVERIFY(loader->running());
    QVERIFY(!loader->isLoaded());

    Automator::waitUntil(loader.data(), "running", false);

    QCOMPARE(pool->count(), 2);

    QQmlApplicationEngine engine;

    engine.addImageProvider("custom", new QCImageProvider());

    Automator automator(&engine);

    QString source = QString(SRCDIR) + "/qml/imageProvider_tintColor.qml";

    engine.load(QUrl::fromLocalFile(source));


    TestRunner* runner = TestRunner::defautInstance();
    int waitTime = runner->config()["waitTime"].toInt();

    Automator::wait(waitTime);


    QVERIFY(!automator.anyError());

}

void QuickCrossUnitTests::imageProvider_clip()
{
    QScopedPointer<QCImageLoader> loader(new QCImageLoader());
    QCImagePool* pool = QCImagePool::instance();

    // Clear loaded images
    pool->clear();
    loader->load(QString(SRCDIR) + "img");
    loader->waitForLoaded();

    QQmlApplicationEngine engine;

    engine.addImageProvider("custom", new QCImageProvider());

    Automator automator(&engine);

    QString source = QString(SRCDIR) + "/qml/imageProvider_clip.qml";

    engine.load(QUrl::fromLocalFile(source));

    QObject *image1 = automator.findObject("image1");
    QVERIFY(image1);

    QSize size = image1->property("sourceSize").value<QSize>();
    QVERIFY(size == QSize(190,200));

    TestRunner* runner = TestRunner::defautInstance();
    int waitTime = runner->config()["waitTime"].toInt();

    Automator::wait(waitTime);

    QVERIFY(!automator.anyError());


}

void QuickCrossUnitTests::mainThreadRunner()
{

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
            QCMainThreadRunner::run(Inner::test, (void*) 0);
        }
    };

    Runnable* runnable = new Runnable();
    runnable->setAutoDelete(true);
    QThreadPool::globalInstance()->start(runnable);

    Automator::wait(100);
    QVERIFY(success);

    /* lambda function */
    success = false;

    auto callback = [&] {
        success = QThread::currentThread() == QCoreApplication::instance()->thread();
    };

    auto func = [=]() {
        QCMainThreadRunner::run(callback);
    };

    QtConcurrent::run(func);

    Automator::wait(100);
    QVERIFY(success);

    success = false;

    // Call the function on main thread. Make sure it has
    QCMainThreadRunner::run(callback);
    QVERIFY(!success);
    Automator::wait(10);
    QVERIFY(success);

    /* QCMainThreadRunner */

    success = false;
    QFuture<void> future = QtConcurrent::run([&]() {
        QCMainThreadRunner::blockingRun([&]() {
            success = QThread::currentThread() == QCoreApplication::instance()->thread();
        });
    });
    QVERIFY(!success);
    future.waitForFinished();
    QVERIFY(success);
}

void QuickCrossUnitTests::imageReader()
{
    //Read from absolute path

    QScopedPointer<QCImageLoader> loader(new QCImageLoader());
    QCImagePool* pool = QCImagePool::instance();

    pool->clear();
    loader->load(QString(SRCDIR) + "img");
    loader->waitForLoaded(10000);

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
