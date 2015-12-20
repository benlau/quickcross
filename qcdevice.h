#ifndef QCDEVICE_H
#define QCDEVICE_H

/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QObject>

class QCDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString os READ os)
    Q_PROPERTY(bool isAndroid READ isAndroid)
    Q_PROPERTY(bool isLinux READ isLinux)
    Q_PROPERTY(bool isMac READ isMac)
    Q_PROPERTY(bool isIOS READ isIOS)
    Q_PROPERTY(bool isWindows READ isWindows)
    Q_PROPERTY(qreal dp READ dp)
        
public:
    explicit QCDevice(QObject *parent = 0);

    QString os() const;

    bool isAndroid() const;

    bool isLinux() const;

    bool isIOS() const;

    bool isMac() const;

    bool isWindows() const;

    qreal dp() const;

signals:

public slots:
};

#endif // QCDEVICE_H
