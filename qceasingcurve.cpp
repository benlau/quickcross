#include <QtCore>
#include <QEasingCurve>
#include "qceasingcurve.h"
#include "priv/qcutils.h"

static QEasingCurve toCurve(QJSValue value) {
    QEasingCurve res;

    if (value.hasProperty("value")) {
        res = value.property("value").toVariant().value<QEasingCurve>();
    } else {
        res = value.toVariant().value<QEasingCurve>();
    }

    return res;
}

QCEasingCurve::QCEasingCurve(QObject *parent) : QObject(parent)
{
    m_engine = 0;
}

QVariant QCEasingCurve::_createValue(QCEasingCurve::Type type)
{
    return QEasingCurve((QEasingCurve::Type) type);
}

QJSValue QCEasingCurve::create(QCEasingCurve::Type type)
{
    QJSValueList args;
    args << QJSValue((int) type);

    return creator.call(args);
}

qreal QCEasingCurve::valueForProgress(QJSValue value, qreal progress)
{
    QEasingCurve curve = toCurve(value);

    return curve.valueForProgress(progress);
}

QQmlEngine *QCEasingCurve::engine() const
{
    return m_engine;
}

void QCEasingCurve::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
    if (!m_engine) {
        return;
    }

    creator = QCUtils::loadJavascript(m_engine, "qrc:///QuickCross/js/easingcurve.js","create");
}
