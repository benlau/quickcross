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
 * \inherits QtObject
 *
 * \brief Provider of device related information
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

qreal QCDevice::dp() const
{
    return m_dp;
}

