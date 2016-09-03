#ifndef QCREFRESHER_H
#define QCREFRESHER_H

#include <QObject>
#include <QJSValue>
#include <QMap>
#include <QPointer>
#include <QTime>

class QCRefresher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int frameRate READ frameRate WRITE setFrameRate NOTIFY frameRateChanged)
public:
    QCRefresher(QObject* parent = 0);

    int frameRate() const;
    void setFrameRate(int frameRate);

public slots:
    void requestRefresh();

    void markDirty(QObject* object);

signals:
    void refresh();

    void frameRateChanged();

protected:
    void timerEvent(QTimerEvent *event);

private:
    int m_frameRate;
    int m_interval;

    bool m_pending;

    QTime timer;

    // The elapsed time of last timer event
    int lastElapsedTimerEvent;

    QMap<QObject*,bool > dirtyObjects;
    QList<QPointer<QObject> > list;
};

#endif // QCREFRESHER_H
