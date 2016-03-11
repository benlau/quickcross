/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QtCore>
#include "qcrect.h"


/*!
 * \qmltype Rect
 * \instantiates QCRect
 * \inqmlmodule QuickCross
 * \inherits QObject
 *
 * \brief Wrapper of QRectF utilities
 *
 * It is a singleton component
 *
 */


QCRect::QCRect(QObject *parent) : QObject(parent)
{

}

/*!
    \qmlmethod bool Rect::intersects(rect r1, rect r2)

    Returns true if this rectangle intersects with the given rectangle (i.e. there is a non-empty area of overlap between them), otherwise returns false.

 */


bool QCRect::intersects(const QRectF &r1, const QRectF &r2) const
{
    return r1.intersects(r2);
}
