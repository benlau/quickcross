/* QuickCross Project
 * License: APACHE-2.0
 * Author: Ben Lau
 * Project Site: https://github.com/benlau/quickcross
 *
 */

#ifndef QCIMAGEREADER_H
#define QCIMAGEREADER_H

#include <QObject>
#include <QSize>
#include <QVariantMap>
#include <QImage>
#include <QQmlEngine>
#include <QQmlParserStatus>
#include "priv/qcreader.h"

/// Wrapper of QImageReader with asynchronous loading

class QCImageReader : public QCReader, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(bool canRead READ canRead NOTIFY canReadChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(bool isFetched READ isFetched NOTIFY isFetchedChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)

    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QCImageReader(QObject *parent = 0);

    bool canRead() const;
    void setCanRead(bool canRead);

    QSize size() const;
    void setSize(const QSize &size);

    bool isFetched() const;
    void setIsFetched(bool isFetched);

    QString source() const;

    void setSource(const QString &source);

    QImage image() const;

    void setImage(const QImage &image);

    static bool isImageProviderUrl(const QString& source);

    // Read image from image provider.
    static QImage readFromImageProvider(QQmlEngine* engine, const QString source);

public slots:

    /// Fetch image information without read the whole image
    void fetch();

    /// Read image from the source
    void read();

    void clear();

signals:
    void fetched();

    void sizeChanged();
    void canReadChanged();
    void isFetchedChanged();
    void sourceChanged();
    void imageChanged();

private slots:
    void onFetchFinished(QVariantMap map);
    void onReadImageFinished(QVariantMap map);

private:

    void classBegin();
    void componentComplete();

    bool m_canRead;
    QSize m_size;
    bool m_isFetched;

    QString m_source;
    QImage m_image;
    bool m_isReady;
    bool m_isError;
    bool m_isFinished;
    QString m_errorString;

    QQmlEngine* m_engine;
};

#endif // QCIMAGEREADER_H
