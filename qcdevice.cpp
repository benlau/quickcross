/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QSysInfo>
#include <QtGlobal>
#include "qcdevice.h"

/*!
 * \qmltype Device
 * \instantiates QCDevice
 * \inherits QtObject
 *
 * \brief Provier of device related information
 */

QCDevice::QCDevice(QObject *parent) : QObject(parent)
{

}

QString QCDevice::os() const
{
    return QSysInfo::productType();
}

bool QCDevice::isAndroid() const
{
#ifdef Q_OS_ANDROID
    return true;
#else
    return false;
#endif
}

bool QCDevice::isLinux() const
{
#ifdef Q_OS_LINUX
    return true;
#else
    return false;
#endif
}

bool QCDevice::isIOS() const
{
#ifdef Q_OS_IOS
    return true;
#else
    return false;
#endif
}

bool QCDevice::isMac() const
{
#ifdef Q_OS_MAC
    return true;
#else
    return false;
#endif

}

bool QCDevice::isWindows() const
{
#ifdef Q_OS_WIN32
    return true;
#else
    return false;
#endif
}

