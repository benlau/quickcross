#ifndef QCLINE_H
#define QCLINE_H

#include <QObject>
#include <QPointF>
#include <QLineF>
#include <QVariantMap>
#include <QJSValue>

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

class QCLine : public QObject
{
    Q_OBJECT
public:
    explicit QCLine(QObject *parent = 0);

signals:

public slots:
    QVariantMap create(const QPointF& p1, const QPointF& p2) const;

    QVariantMap create(qreal x1, qreal y1, qreal x2, qreal y2) const;

    QVariantMap create(const QPointF& p2) const;

    QVariantMap create(const QVariantMap& line) const;

    bool equals(const QJSValue& line1, const QJSValue& line2) const;

    qreal length(QJSValue line) const;

    void setLength(QJSValue line, qreal length) const;

    QVariantMap unitVector(QJSValue line) const;

    qreal angle(QJSValue line) const;

    void setAngle(QJSValue line,qreal angle) const;

    void translate(QJSValue line, qreal dx, qreal dy) const;

    void translateToOrigin(QJSValue line) const;

    qreal dx(QJSValue line) const;

    qreal dy(QJSValue line) const;

    QPointF p1(QJSValue line) const;

    QPointF p2(QJSValue line) const;

};

#endif // QCLINE_H
