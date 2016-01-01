/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QSysInfo>
#include <QtGlobal>
#include "qcdevice.h"

#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#endif

static qreal m_dp = 1;

/*!
 * \qmltype Device
 * \instantiates QCDevice
 * \inqmlmodule QuickCross
 * \inherits QObject
 *
 * \brief Provider of device related information
 * It is a singleton component
 *
 */


QCDevice::QCDevice(QObject *parent) : QObject(parent)
{

#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resource = activity.callObjectMethod("getResources","()Landroid/content/res/Resources;");
    QAndroidJniObject metrics = resource.callObjectMethod("getDisplayMetrics","()Landroid/util/DisplayMetrics;");
    m_dp = metrics.getField<float>("density");
#endif
}

/*!
 * \qmlproperty string Device::os
 * This property hold the product name of the operating system this application is running in.
 * It is equivalent to QSysInfo::productType()
 *
 */

QString QCDevice::os() const
{
    QString res = "unknown";

#ifdef Q_OS_LINUX
    // Don't return a distribution name
    res = "linux"
#else
    res = QSysInfo::productType();
#endif

    return res;
}

/*!
 * \qmlproperty bool Device::isAndroid
 */

bool QCDevice::isAndroid() const
{
#ifdef Q_OS_ANDROID
    return true;
#else
    return false;
#endif
}

/*!
 * \qmlproperty bool Device::isLinux
 */

bool QCDevice::isLinux() const
{
#ifdef Q_OS_LINUX
    return true;
#else
    return false;
#endif
}

/*!
 * \qmlproperty bool Device::isIOS
 */

bool QCDevice::isIOS() const
{
#ifdef Q_OS_IOS
    return true;
#else
    return false;
#endif
}

/*!
 * \qmlproperty bool Device::isMac
 */

bool QCDevice::isMac() const
{
#ifdef Q_OS_MAC
    return true;
#else
    return false;
#endif

}

/*!
 * \qmlproperty bool Device::isWindows
 */

bool QCDevice::isWindows() const
{
#ifdef Q_OS_WIN32
    return true;
#else
    return false;
#endif
}

/*!
 * \qmlproperty real Device::dp
 *
 * This property hold the density independent pixel (DP) of the Android device this application is running.
 *
 * It is valid only in Android system. In other system, the default value is 1.
 *
 */

qreal QCDevice::dp() const
{
    return m_dp;
}

