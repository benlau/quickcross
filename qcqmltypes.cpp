/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QtQml>
#include <QCImageLoader>
#include "qcdevice.h"
#include "qcstandardpaths.h"
#include "qcrect.h"
#include "qcline.h"
#include "qcimagereader.h"
#include "qcjsonreader.h"
#include "qceasingcurve.h"
#include "qcrefresher.h"
#include "qctime.h"

template<typename T>
static QObject* providerWithEngine(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    T* object = new T();
    object->setEngine(engine);

    return object;
}


template<typename T>
static QObject* provider(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    T* object = new T();

    return object;
}

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

        qmlRegisterSingletonType<QCDevice>("QuickCross", 1, 0,
                                           "Line", provider<QCLine> );

        qmlRegisterType<QCImageLoader>("QuickCross", 1, 0, "ImageLoader");

        qmlRegisterType<QCImageReader>("QuickCross", 1, 0, "ImageReader");

        qmlRegisterType<QCJsonReader>("QuickCross", 1, 0, "JsonReader");

        qmlRegisterSingletonType<QCEasingCurve>("QuickCross", 1, 0,
                                           "EasingCurve", providerWithEngine<QCEasingCurve> );

        qmlRegisterSingletonType<QCRefresher>("QuickCross", 1, 0,
                                           "Refresher", provider<QCRefresher> );

        qmlRegisterSingletonType<QCTime>("QuickCross", 1, 0,
                                           "Time", providerWithEngine<QCTime>);

    }
};

static QCQmlTypes registerHelper;
