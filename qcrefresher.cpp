#include <QtCore>
#include "qcrefresher.h"

QCRefresher::QCRefresher(QObject* parent) : QObject(parent)
{
    m_pending = false;
    setFrameRate(60);
    timer.start();
}

void QCRefresher::markDirty(QObject *object)
{

    if (dirtyObjects.contains(object)) {
        return;
    }

    list << object;
    dirtyObjects[object] = true;

    if (m_pending) {
        return;
    }

    int elapsed = timer.elapsed();

    int next = elapsed % m_interval;
    startTimer(next);
}

void QCRefresher::timerEvent(QTimerEvent *event)
{
    killTimer(event->timerId());

    for (int i = 0 ; i < list.size() ; i++) {
        QObject* target = list.at(i);
        if (target) {
            QMetaObject::invokeMethod(target,"refresh",Qt::DirectConnection);
        }
    }

    list.clear();
    dirtyObjects.clear();
}

int QCRefresher::frameRate() const
{
    return m_frameRate;
}

void QCRefresher::setFrameRate(int frameRate)
{
    m_frameRate = frameRate;
    m_interval = 1000 / m_frameRate;
}
