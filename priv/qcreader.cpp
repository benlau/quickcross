#include "qcreader.h"

QCReader::QCReader(QObject *parent) : QObject(parent)
{
    m_isReady = false;
    m_isError = false;
    m_isFinished = false;
}

bool QCReader::isReady() const
{
    return m_isReady;
}

void QCReader::setIsReady(bool isReady)
{
    m_isReady = isReady;
    emit isReadyChanged();
}

bool QCReader::isError() const
{
    return m_isError;
}

void QCReader::setIsError(bool isError)
{
    m_isError = isError;
    emit isErrorChanged();
}

bool QCReader::isFinished() const
{
    return m_isFinished;
}

void QCReader::setIsFinished(bool value)
{
    m_isFinished = value;
    emit isFinishedChanged();
}

QString QCReader::errorString() const
{
    return m_errorString;
}

void QCReader::setErrorString(const QString &errorString)
{
    m_errorString = errorString;
    emit errorStringChanged();
}

void QCReader::clear()
{
    setIsReady(false);
    setIsFinished(false);
    setIsError(false);
    setErrorString(QString());
}

void QCReader::finish()
{
    setIsFinished(true);
    emit finished();
}
