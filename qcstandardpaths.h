#ifndef QCSTANDARDPATHS_H
#define QCSTANDARDPATHS_H

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QObject>
#include <QStringList>

/*!
  \class QCStandardPaths
 */

class QCStandardPaths : public QObject
{
    Q_OBJECT
    Q_ENUMS(StandardLocation)

public:
    explicit QCStandardPaths(QObject *parent = 0);

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

signals:

public slots:    
    QStringList standardLocations(int standardLocation) const;

    QString writableLocation(int standardLocation) const;

    QString displayName(int standardLocation) const;
};

#endif // QCSTANDARDPATHS_H
