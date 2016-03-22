#ifndef QCEASINGCURVE_H
#define QCEASINGCURVE_H

#include <QObject>
#include <QVariant>
#include <QJSValue>
#include <QQmlEngine>

/// QML Wrapper of QEasingCurve

class QCEasingCurve : public QObject
{
    Q_OBJECT
    Q_ENUMS(Type)

public:
    explicit QCEasingCurve(QObject *parent = 0);

    enum Type {
        Linear,
        InQuad,
        OutQuad,
        InOutQuad,
        OutInQuad,
        InCubic,
        OutCubic,
        InOutCubic,
        OutInCubic,
        InQuart,
        OutQuart,
        InOutQuart,
        OutInQuart,
        InQuint,
        OutQuint,
        InOutQuint,
        OutInQuint,
        InSine,
        OutSine,
        InOutSine,
        OutInSine,
        InExpo,
        OutExpo,
        InOutExpo,
        OutInExpo,
        InCirc,
        OutCirc,
        InOutCirc,
        OutInCirc,
        InElastic, OutElastic, InOutElastic, OutInElastic,
        InBack, OutBack, InOutBack, OutInBack,
        InBounce, OutBounce, InOutBounce, OutInBounce,
        InCurve, OutCurve, SineCurve, CosineCurve,
        BezierSpline, TCBSpline, Custom, NCurveTypes
    };

    QQmlEngine *engine() const;

    void setEngine(QQmlEngine *engine);

signals:

public slots:
    QVariant _createValue(Type type = Linear);

    QJSValue create(Type type = Linear);

    qreal valueForProgress(QJSValue easingCurve, qreal progress);
private:
    QQmlEngine* m_engine;
    QJSValue creator;
};

#endif // QCEASINGCURVE_H
