#ifndef QCREADER_H
#define QCREADER_H

#include <QObject>

// Reader interface

class QCReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isReady READ isReady WRITE setIsReady NOTIFY isReadyChanged)
    Q_PROPERTY(bool isError READ isError WRITE setIsError NOTIFY isErrorChanged)
    Q_PROPERTY(bool isCompleted READ isCompleted WRITE setIsCompleted NOTIFY isCompletedChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)

public:
    explicit QCReader(QObject *parent = 0);

    bool isReady() const;
    void setIsReady(bool isReady);

    bool isError() const;
    void setIsError(bool isError);

    bool isCompleted() const;
    void setIsCompleted(bool isCompleted);

    QString errorString() const;
    void setErrorString(const QString &errorString);

signals:
    void isReadyChanged();
    void isCompletedChanged();
    void isErrorChanged();
    void errorStringChanged();

    void completed();

public slots:

protected:
    void clear();

private:
    bool m_isReady;
    bool m_isError;
    bool m_isCompleted;
    QString m_errorString;
};

#endif // QCREADER_H
