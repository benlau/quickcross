#include <QtQml>
#include "qcdevice.h"
#include "qcstandardpaths.h"
#include "qcrect.h"

static QObject *rectProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QCRect* object = new QCRect();

    return object;
}

static QObject *deviceProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QCDevice* object = new QCDevice();

    return object;
}

static QObject *standardPathsProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QCStandardPaths* object = new QCStandardPaths();

    return object;
}

class QCQmlTypes {

public:
    QCQmlTypes() {

        qmlRegisterSingletonType<QCDevice>("QuickCross", 1, 0,
                                           "Device", deviceProvider);

        qmlRegisterSingletonType<QCDevice>("QuickCross", 1, 0,
                                           "StandardPaths", standardPathsProvider);

        qmlRegisterSingletonType<QCDevice>("QuickCross", 1, 0,
                                           "Rect", rectProvider);

    }
};

static QCQmlTypes registerHelper;
