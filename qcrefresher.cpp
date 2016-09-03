#include <QtCore>
#include "qcrefresher.h"

QCRefresher::QCRefresher(QObject* parent) : QObject(parent)
{
    m_pending = false;
    setFrameRate(60);
    timer.start();
    lastElapsedTimerEvent = -1;
}

void QCRefresher::markDirty(QObject *object)
{

    if (dirtyObjects.contains(object)) {
        return;
    }

    list << object;
    dirtyObjects[object] = true;

    requestRefresh();
}

void QCRefresher::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());
    lastElapsedTimerEvent = timer.elapsed();

    QList<QPointer<QObject> > queue = list;
    list.clear();
    dirtyObjects.clear();
    m_pending = false;

    for (int i = 0 ; i < queue.size() ; i++) {
        QObject* target = queue.at(i);
        if (target) {
            QMetaObject::invokeMethod(target,"refresh",Qt::DirectConnection);
        }
    }

    emit refresh();

}

int QCRefresher::frameRate() const
{
    return m_frameRate;
}

void QCRefresher::setFrameRate(int frameRate)
{
    if (frameRate <= 0 || frameRate > 1000) {
        qWarning() << "Refersher::Invalid frame rate";
        return;
    }

    m_frameRate = frameRate;
    m_interval = 1000 / m_frameRate;
    emit frameRateChanged();
}

void QCRefresher::requestRefresh()
{

    if (m_pending) {
        return;
    }

    m_pending = true;
    int elapsed = timer.elapsed();

    int next = elapsed % m_interval;

    if (next == 0 && lastElapsedTimerEvent == elapsed) {
        // Condition: Mark a item dirty just after executed the refresh function.
        next = m_interval - 1;
    }

    startTimer(next);
}
