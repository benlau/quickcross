#ifndef QCJSONREADER_H
#define QCJSONREADER_H

#include <QObject>
#include <QQmlParserStatus>
#include <QQmlEngine>
#include <QPointer>
#include "priv/qcreader.h"

class QCJsonReader : public QCReader, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(bool autoRead READ autoRead WRITE setAutoRead NOTIFY autoReadChanged)
    Q_PROPERTY(QJSValue object READ object WRITE setObject NOTIFY objectChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QCJsonReader(QObject *parent = 0);

    QString source() const;

    void setSource(const QString &source);

    bool autoRead() const;

    void setAutoRead(bool autoRead);

    QJSValue object() const;

    void setObject(const QJSValue &object);

signals:
    void sourceChanged();
    void autoReadChanged();
    void objectChanged();

public slots:
    void read();

private:
    void classBegin();
    void componentComplete();

    QString m_source;
    bool m_autoRead;
    bool m_componentCompleted;

    QPointer<QQmlEngine> m_engine;
    QJSValue m_object;
};

#endif // QCJSONREADER_H
