#ifndef QCRECT_H
#define QCRECT_H

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QObject>
#include <QRectF>

class QCRect : public QObject
{
    Q_OBJECT
public:
    explicit QCRect(QObject *parent = 0);

signals:

public slots:
    bool intersects(const QRectF& r1, const QRectF& r2) const;
};

#endif // QCRECT_H
