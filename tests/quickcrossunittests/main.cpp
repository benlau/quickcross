#include <QString>
#include <QtTest>
#include "quickcrossunittests.h"
#include "testrunner.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);

    TestRunner runner;
    runner.add<QuickCrossUnitTests>();

    bool error = runner.exec(app.arguments());

    if (!error) {
        qWarning() << "All test cases passed!";
    }

    return error;
}
