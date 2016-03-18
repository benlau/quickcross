#include <QtCore>
#include <QFile>
#include <QJsonDocument>
#include <QtQml>
#include "priv/qcutils.h"
#include "qcjsonreader.h"

QCJsonReader::QCJsonReader(QObject *parent) : QCReader(parent)
{
    m_autoRead = false;
    m_componentCompleted = false;
}

QString QCJsonReader::source() const
{
    return m_source;
}

void QCJsonReader::setSource(const QString &source)
{
    m_source = source;
    emit sourceChanged();

    if (!m_source.isEmpty() && m_autoRead && m_componentCompleted) {
        read();
    }
}

bool QCJsonReader::autoRead() const
{
    return m_autoRead;
}

void QCJsonReader::setAutoRead(bool autoRead)
{
    m_autoRead = autoRead;
    emit autoReadChanged();
}

void QCJsonReader::read()
{
    // Asynchonous read is not supported yet
    clear();

    if (m_engine.isNull()) {
        setIsError(true);
        setErrorString("Unexpected error");
        setIsCompleted(true);
        return;
    }


    QString path = QCUtils::normalizeResourceUrl(m_source);
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        setErrorString(file.errorString());
        setIsError(true);
        setIsCompleted(true);
        return;
    }

    QByteArray data = file.readAll();

    QJSValue value = m_engine->evaluate(data);

    if (value.isError()) {
        setErrorString(value.toString());
        setIsError(true);
        setIsCompleted(true);
    } else {
        setObject(value);
        setIsReady(true);
        setIsCompleted(true);
    }
}

void QCJsonReader::classBegin()
{

}

void QCJsonReader::componentComplete()
{
    m_componentCompleted = true;
    m_engine = qmlEngine(this);

    if (m_autoRead && !m_source.isEmpty()) {
        read();
    }
}

QJSValue QCJsonReader::object() const
{
    return m_object;
}

void QCJsonReader::setObject(const QJSValue &object)
{
    m_object = object;
    emit objectChanged();
}
