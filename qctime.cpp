#include <QTime>
#include <QtCore>
#include "qctime.h"
#include "priv/qcutils.h"

static QTime toTime(QJSValue value) {
    QTime res;

    if (value.hasProperty("value")) {
        res = value.property("value").toVariant().value<QTime>();
    } else {
        res = value.toVariant().value<QTime>();
    }

    return res;
}

QCTime::QCTime(QObject *parent) : QObject(parent)
{

}

QJSValue QCTime::create()
{
    QJSValueList args;

    return creator.call(args);
}

QJSValue QCTime::_createValue()
{
    QTime time;

    QJSValue value = m_engine->toScriptValue<QTime>(time);

    return value;
}

void QCTime::start(QJSValue object)
{
   QTime time = toTime(object);

   time.start();

   object.setProperty("value",m_engine->toScriptValue<QTime>(time));
}

int QCTime::elapsed(QJSValue object)
{
    QTime time = toTime(object);
    return time.elapsed();
}

QQmlEngine *QCTime::engine() const
{
    return m_engine;
}

void QCTime::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
    if (!m_engine) {
        return;
    }

    creator = QCUtils::loadJavascript(m_engine, "qrc:///QuickCross/js/time.js","create");
}
