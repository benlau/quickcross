#ifndef QUICKCROSSUNITTESTS_H
#define QUICKCROSSUNITTESTS_H

#include <QObject>
#include <QQmlError>

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

    void utils();

    void loader();

    void device();

    void imagePool();

    void imageLoader();
    void imageLoader_qrc();

    void imageLoader_filter();

    void imageLoader_destroyAtRunning();

    void imageProvider();

    void imageProvider_tintcolor();

    void imageProvider_clip();

    void mainThreadRunner();

    void imageReader();

    void refresher();

private:
    QList<QQmlError> warnings;
};

#endif // QUICKCROSSUNITTESTS_H
