#include <QtQml>
#include "qcdevice.h"

static QObject *deviceProvider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    QCDevice* object = new QCDevice();

    return object;
}

class QCQmlTypes {

public:
    QCQmlTypes() {
        // QADevice is a exception. Won't register by QAQmlTypes.

        qmlRegisterSingletonType<QCDevice>("QuickCross", 1, 0,
                                           "Device", deviceProvider);
    }
};

static QCQmlTypes registerHelper;
