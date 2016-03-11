#include <QCoreApplication>
#include <QVariant>
#include "qcmainthreadrunner.h"

QCMainThreadRunner::QCMainThreadRunner(QObject *parent) : QObject(parent)
{
}

QCMainThreadRunner *QCMainThreadRunner::instance()
{
    static QCMainThreadRunner* m_instance = 0;

    if (m_instance == 0) {
        QCoreApplication* app = QCoreApplication::instance();

        m_instance = new QCMainThreadRunner(app);
    }

    return m_instance;
}

void QCMainThreadRunner::prepare()
{
    instance();
}

void QCMainThreadRunner::start(QCMainThreadRunner::Callback func, void *data)
{
    QVariant f = QVariant::fromValue<void*>((void*) func );
    QVariant d = QVariant::fromValue<void*>((void*) data );

    QMetaObject::invokeMethod(instance(),"run",Qt::QueuedConnection,
                              Q_ARG(QVariant,f),
                              Q_ARG(QVariant,d));
}

void QCMainThreadRunner::run(QVariant func, QVariant data)
{
    Callback f = (Callback) func.value<void*>();
    void* d = data.value<void*>();

    f(d);
}
