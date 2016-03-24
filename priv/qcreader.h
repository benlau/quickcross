#ifndef QCREADER_H
#define QCREADER_H

#include <QObject>

// Reader interface

class QCReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isReady READ isReady WRITE setIsReady NOTIFY isReadyChanged)
    Q_PROPERTY(bool isError READ isError WRITE setIsError NOTIFY isErrorChanged)
    Q_PROPERTY(bool isFinished READ isFinished WRITE setIsFinished NOTIFY isFinishedChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)

public:
    explicit QCReader(QObject *parent = 0);

    bool isReady() const;
    void setIsReady(bool isReady);

    bool isError() const;
    void setIsError(bool isError);

    bool isFinished() const;
    void setIsFinished(bool isFinished);

    QString errorString() const;
    void setErrorString(const QString &errorString);

signals:
    void isReadyChanged();
    void isFinishedChanged();
    void isErrorChanged();
    void errorStringChanged();

    void finished();

public slots:

protected:
    void clear();

    /// Set isFinished to true and emit finished();
    void finish();

private:
    bool m_isReady;
    bool m_isError;
    bool m_isFinished;
    QString m_errorString;
};

#endif // QCREADER_H
