/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include "qcimageloaderwrapper.h"
#include "qcimageloader.h"

QCImageLoaderWrapper::QCImageLoaderWrapper(QObject *parent) : QObject(parent)
{
    m_running = false;
    m_isLoaded = false;

    QCImageLoader* loader = QCImageLoader::instance();

    connect(loader,SIGNAL(runningChanged()),
            this,SLOT(updateRunning()));

    connect(loader, SIGNAL(isLoadedChanged()),
            this, SLOT(updateIsLoaded()));
}

bool QCImageLoaderWrapper::running() const
{
    return m_running;
}

void QCImageLoaderWrapper::setRunning(bool running)
{
    m_running = running;
}

bool QCImageLoaderWrapper::isLoaded() const
{
    return m_isLoaded;
}

void QCImageLoaderWrapper::updateRunning()
{
    QCImageLoader* loader = QCImageLoader::instance();
    setRunning(loader->running());
}

void QCImageLoaderWrapper::updateIsLoaded()
{
    QCImageLoader* loader = QCImageLoader::instance();
    setIsLoaded(loader->isLoaded());
}

void QCImageLoaderWrapper::setIsLoaded(bool isLoaded)
{
    m_isLoaded = isLoaded;
}
