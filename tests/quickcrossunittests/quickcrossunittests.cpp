#include <QQmlApplicationEngine>
#include <QtCore>
#include <QTest>
#include "qcdevice.h"
#include "quickcrossunittests.h"
#include "qcimageloader.h"
#include "automator.h"

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

    Automator::waitUntil(loader, "running", false);

    QVERIFY(loader->isLoaded());

    QCOMPARE(loader->count(), 1);

    QVERIFY(loader->contains("qt-logo-medium.png"));
    QVERIFY(loader->contains("qt-logo-medium"));
    QVERIFY(!loader->contains("not-existed"));

    QImage image = loader->image("qt-logo-medium.png");
    QCOMPARE(image.width(), 381);
    QCOMPARE(image.height(), 500);

    QVERIFY (loader->image("qt-logo-medium") == image);
}
