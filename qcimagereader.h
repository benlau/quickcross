#ifndef QCIMAGEREADER_H
#define QCIMAGEREADER_H

#include <QObject>
#include <QSize>
#include <QVariantMap>

/// Wrapper of QImageReader with asynchronous loading

class QCImageReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool canRead READ canRead NOTIFY canReadChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(bool isFetched READ isFetched NOTIFY isFetchedChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

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

public slots:

    /// Fetch image information without read the whole image
    void fetch();

signals:
    void fetched();

    void sizeChanged();
    void canReadChanged();
    void isFetchedChanged();
    void sourceChanged();

private slots:
    void onFetchFinished(QVariantMap map);

private:
    bool m_canRead;
    QSize m_size;
    bool m_isFetched;

    QString m_source;
};

#endif // QCIMAGEREADER_H
