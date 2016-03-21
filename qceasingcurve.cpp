#include <QEasingCurve>
#include "qceasingcurve.h"

QCEasingCurve::QCEasingCurve(QObject *parent) : QObject(parent)
{
}

QVariant QCEasingCurve::createValue()
{
    return QEasingCurve(QEasingCurve::Linear);
}

qreal QCEasingCurve::valueForProgress(QJSValue value, qreal progress)
{
    QEasingCurve curve = value.toVariant().value<QEasingCurve>();

    return curve.valueForProgress(progress);
}
