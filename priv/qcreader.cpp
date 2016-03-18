#include "qcreader.h"

QCReader::QCReader(QObject *parent) : QObject(parent)
{
    m_isReady = false;
    m_isError = false;
    m_isCompleted = false;
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

bool QCReader::isCompleted() const
{
    return m_isCompleted;
}

void QCReader::setIsCompleted(bool isCompleted)
{
    m_isCompleted = isCompleted;
    emit isCompletedChanged();
    if (m_isCompleted) {
        emit completed();
    }
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
    setIsCompleted(false);
    setIsError(false);
    setErrorString(QString());
}
