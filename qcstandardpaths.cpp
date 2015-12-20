#include <QtCore>
#include <QStandardPaths>
#include "qcstandardpaths.h"


/*!
 * \qmltype StandardPaths
 * \instantiates QCStandardPaths
 * \inherits QtObject
 *
 * \brief QML Wrapper of QStandardPaths
 */

QCStandardPaths::QCStandardPaths(QObject *parent) : QObject(parent)
{

}

QStringList QCStandardPaths::standardLocations(int standardLocation) const
{
    return QStandardPaths::standardLocations((QStandardPaths::StandardLocation) standardLocation);
}

QString QCStandardPaths::writableLocation(int standardLocation) const
{
    return QStandardPaths::writableLocation((QStandardPaths::StandardLocation) standardLocation);
}

QString QCStandardPaths::displayName(int standardLocation) const
{
    return QStandardPaths::displayName((QStandardPaths::StandardLocation) standardLocation);
}


