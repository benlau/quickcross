#include <QtCore>
#include <QStandardPaths>
#include "qcstandardpaths.h"


/*!
 * \qmltype StandardPaths
 * \instantiates QCStandardPaths
 *
 * \brief QML Wrapper of QStandardPaths
 *
 * It is a singleton component
 *
 */

/*!
 \qmlproperty enumeration StandardPaths::stanardLocation

 It is equivalent to QStandardPaths::standardLocation()

  \code

    enum StandardLocation {
        DesktopLocation,
        DocumentsLocation,
        FontsLocation,
        ApplicationsLocation,
        MusicLocation,
        MoviesLocation,
        PicturesLocation,
        TempLocation,
        HomeLocation,
        DataLocation,
        CacheLocation,
        GenericDataLocation,
        RuntimeLocation,
        ConfigLocation,
        DownloadLocation,
        GenericCacheLocation,
        GenericConfigLocation,
        AppDataLocation,
        AppConfigLocation,
        AppLocalDataLocation = DataLocation
    };


  \endcode

 */

QCStandardPaths::QCStandardPaths(QObject *parent) : QObject(parent)
{

}

/*!
    \qmlmethod array StandardPaths::standardLocations(int standardLocation)

    It is equivalent to QStandardPaths::standardLocations()
 */

QStringList QCStandardPaths::standardLocations(int standardLocation) const
{
    return QStandardPaths::standardLocations((QStandardPaths::StandardLocation) standardLocation);
}

/*!
    \qmlmethod array StandardPaths::writableLocation(int standardLocation)

    It is equivalent to QStandardPaths::writableLocation()

 */

QString QCStandardPaths::writableLocation(int standardLocation) const
{
    return QStandardPaths::writableLocation((QStandardPaths::StandardLocation) standardLocation);
}

/*!
    \qmlmethod array StandardPaths::displayName(int standardLocation)

    It is equivalent to QStandardPaths::displayName()

 */

QString QCStandardPaths::displayName(int standardLocation) const
{
    return QStandardPaths::displayName((QStandardPaths::StandardLocation) standardLocation);
}


