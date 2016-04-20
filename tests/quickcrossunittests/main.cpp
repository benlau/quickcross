#include <QString>
#include <QtTest>
#include "quickcrossunittests.h"
#include "testrunner.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc,argv);

    TestRunner runner;
    runner.add<QuickCrossUnitTests>();
    runner.add(QString(SRCDIR) + "/qml");

    int waitTime = 100;
    if (app.arguments().size() != 1) {
        waitTime = 60000;
    }

    QVariantMap config;
    config["waitTime"] = waitTime;
    runner.setConfig(config);

    bool error = runner.exec(app.arguments());

    if (!error) {
        qWarning() << "All test cases passed!";
    }

    return error;
}
