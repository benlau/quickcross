/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#include <QtCore>
#include "qcimageprovider.h"
#include "qcimageloader.h"

QCImageProvider::QCImageProvider() : QQuickImageProvider(QQmlImageProviderBase::Image)
{

}

QImage QCImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(requestedSize);

    QCImageLoader* loader = QCImageLoader::instance();
    QImage result;

    if (loader->contains(id)) {
        result = loader->image(id);
        *size = result.size();
    }

    return result;
}
