#ifndef QCUTILS_H
#define QCUTILS_H

#include <QString>

class QCUtils
{
public:
    static bool isImageProviderUrl(const QString& url);

    static bool isResourceUrl(const QString& url);

};

#endif // QCUTILS_H
