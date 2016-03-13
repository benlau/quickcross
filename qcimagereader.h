#ifndef QCIMAGEREADER_H
#define QCIMAGEREADER_H

#include <QObject>
#include <QSize>
#include <QVariantMap>
#include <QImage>

/// Wrapper of QImageReader with asynchronous loading

class QCImageReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool canRead READ canRead NOTIFY canReadChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(bool isFetched READ isFetched NOTIFY isFetchedChanged)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(bool isReady READ isReady WRITE setIsReady NOTIFY isReadyChanged)
    Q_PROPERTY(bool isError READ isError WRITE setIsError NOTIFY isErrorChanged)
    Q_PROPERTY(bool isCompleted READ isCompleted WRITE setIsCompleted NOTIFY isCompletedChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)

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

    bool isReady() const;
    void setIsReady(bool isReady);

    QString errorString() const;
    void setErrorString(const QString &errorString);

    bool isCompleted() const;
    void setIsCompleted(bool isCompleted);

    bool isError() const;
    void setIsError(bool isError);

public slots:

    /// Fetch image information without read the whole image
    void fetch();

    /// Read image from the source
    void read();

    void clear();

signals:
    void fetched();
    void completed();

    void sizeChanged();
    void canReadChanged();
    void isFetchedChanged();
    void sourceChanged();
    void imageChanged();
    void isReadyChanged();
    void isCompletedChanged();
    void isErrorChanged();
    void errorStringChanged();

private slots:
    void onFetchFinished(QVariantMap map);
    void onReadImageFinished(QVariantMap map);

private:
    bool m_canRead;
    QSize m_size;
    bool m_isFetched;

    QString m_source;
    QImage m_image;
    bool m_isReady;
    bool m_isError;
    bool m_isCompleted;
    QString m_errorString;
};

#endif // QCIMAGEREADER_H
