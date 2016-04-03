#ifndef QCTIME_H
#define QCTIME_H

#include <QObject>
#include <QJSValue>
#include <QQmlEngine>

/// Wrapper of QTime

class QCTime : public QObject
{
    Q_OBJECT
public:
    explicit QCTime(QObject *parent = 0);

    QQmlEngine *engine() const;
    void setEngine(QQmlEngine *engine);

signals:

public slots:
    QJSValue create();

    QJSValue _createValue();

    void start(QJSValue object);

    int elapsed(QJSValue object);

private:
    QQmlEngine* m_engine;
    QJSValue creator;
};

#endif // QCTIME_H
