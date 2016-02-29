#include <QtCore>
#include "qcline.h"

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

static QVariantMap toMap(const QLineF& line) {
    QVariantMap m;

    m["x1"] = line.x1();
    m["y1"] = line.y1();
    m["x2"] = line.x2();
    m["y2"] = line.y2();

    return m;
}

static void setJSValue(QJSValue& value, const QLineF& line) {

    value.setProperty("x1", line.x1());
    value.setProperty("y1", line.y1());
    value.setProperty("x2", line.x2());
    value.setProperty("y2", line.y2());

}

static QLineF toLine(const QVariantMap& map) {
    QLineF res;

    QPointF p1,p2;

    p1.setX(map["x1"].toDouble());
    p1.setY(map["y1"].toDouble());
    p2.setX(map["x2"].toDouble());
    p2.setY(map["y2"].toDouble());

    res.setP1(p1);
    res.setP2(p2);

    return res;
}

static QLineF toLine(const QJSValue& value) {
    QLineF res;

    QPointF p1,p2;

    p1.setX(value.property("x1").toNumber());
    p1.setY(value.property("y1").toNumber());
    p2.setX(value.property("x2").toNumber());
    p2.setY(value.property("y2").toNumber());

    res.setP1(p1);
    res.setP2(p2);

    return res;

}

/*!
 * \qmltype Line
 * \instantiates QCLine
 *
 * \brief QML Wrapper of QLineF
 *
 * It is a singleton component
 *
 */


QCLine::QCLine(QObject *parent) : QObject(parent)
{

}


/*!
    \qmlmethod object Line::create(point p1, point2)

    Constructs a line object that represents the line between p1 and p2.

 */

QVariantMap QCLine::create(const QPointF &p1, const QPointF &p2) const
{
    QLineF line(p1,p2);
    return toMap(line);
}

/*!
    \qmlmethod object Line::create(real x1, real y1, real x2, real y2)

    Constructs a line object that represents the line between (x1, y1) and (x2, y2).

 */


QVariantMap QCLine::create(qreal x1, qreal y1, qreal x2, qreal y2) const
{
    QLineF line(x1,y1,x2,y2);
    return toMap(line);
}

/*!
    \qmlmethod bool Line::equals(object line1, object line2)

    Returns true if the line1 is same as line2.

    A line is identical to another line if the start and end points are identical, and the internal order of the points is the same.

 */

bool QCLine::equals(const QJSValue &line1, const QJSValue &line2) const
{
    QLineF l1 = toLine(line1);
    QLineF l2 = toLine(line2);

    return l1 == l2;
}

/*!
    \qmlmethod qreal Line::length(object line)

    Get the length of the line
 */

qreal QCLine::length(QJSValue line) const
{
    QLineF l = toLine(line);
    return l.length();
}

/*!
    \qmlmethod void Line::setLength(object line, qreal length)

    Change the length of the line
 */

void QCLine::setLength(QJSValue line, qreal length) const
{
    QLineF l = toLine(line);
    l.setLength(length);

    setJSValue(line, l);
}

/*!
    \qmlmethod object Line::unitVector(object line)

    Returns the unit vector for this line, i.e a line starting at the same point as this line with a length of 1.0.
 */


QVariantMap QCLine::unitVector(QJSValue line) const
{
    QLineF l = toLine(line);

    QLineF uv = l.unitVector();

    return toMap(uv);
}

/*!
    \qmlmethod void Line::setAngle(object line, qreal angle)

    Set the angle of the line
 */

void QCLine::setAngle(QJSValue line, qreal angle) const
{
    QLineF l = toLine(line);
    l.setAngle(angle);

    setJSValue(line, l);
}

/*!
    \qmlmethod void Line::translate(object line, real dx, real dy)

    Translates this line the distance specified by dx and dy.

 */

void QCLine::translate(QJSValue line, qreal dx, qreal dy) const
{
    QLineF l = toLine(line);
    l.translate(dx, dy);

    setJSValue(line, l);
}

/*!
    \qmlmethod qreal Line::angle(object line)

    Get the angle of the line
 */

qreal QCLine::angle(QJSValue line) const
{
    QLineF l = toLine(line);

    return l.angle();
}
