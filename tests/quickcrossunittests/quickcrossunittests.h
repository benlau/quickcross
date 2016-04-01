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

    void imageLoader();
    void imageLoader_qrc();

    void imageLoader_filter();

    void imageProvider();

    void mainThreadRunner();

    void imageReader();

private:
    QList<QQmlError> warnings;
};

#endif // QUICKCROSSUNITTESTS_H
