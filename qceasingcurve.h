#ifndef QCEASINGCURVE_H
#define QCEASINGCURVE_H

#include <QObject>
#include <QVariant>
#include <QJSValue>

/// QML Wrapper of QEasingCurve

class QCEasingCurve : public QObject
{
    Q_OBJECT
public:
    explicit QCEasingCurve(QObject *parent = 0);

signals:

public slots:
    QVariant createValue();

    qreal valueForProgress(QJSValue easingCurve, qreal progress);
};

#endif // QCEASINGCURVE_H
