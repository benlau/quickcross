#include <QString>
#include <QtTest>
#include <QQmlApplicationEngine>

class QuickCrossUnitTests : public QObject
{
    Q_OBJECT

public:
    QuickCrossUnitTests();

public Q_SLOTS:
    void onWarning(const QList<QQmlError> & warnings);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void loader();

private:
    QList<QQmlError> warnings;
};

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
    QString source = QString(SRCDIR) + "/loader.qml";

    engine.load(QUrl::fromLocalFile(source));

    QVERIFY(engine.rootObjects().size() > 0);

    Q_ASSERT(warnings.size() == 0);
}


int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    QuickCrossUnitTests tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_quickcrossunittests.moc"
