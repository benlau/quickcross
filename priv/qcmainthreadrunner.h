#ifndef QCMAINTHREADRUNNER_H
#define QCMAINTHREADRUNNER_H

#include <QObject>
#include <QVariant>

class QCMainThreadRunner : public QObject
{
    Q_OBJECT
public:

    /// A dummy function to make sure QCMainThreadRunner instance is created. You should call this function before start a QRunnable
    static void prepare();

    typedef void (*Callback)(void* data);

    /// Run a function with custom data on main thread. If it is already in main thread, it will be executed in next tick.
    /** @threadsafe
     */
    static void runOnMainThread(Callback func, void* data);

signals:

public slots:

private slots:
    void run(QVariant func, QVariant data);

private:
    explicit QCMainThreadRunner(QObject *parent = 0);

    static QCMainThreadRunner* instance();
};

#endif // QCMAINTHREADRUNNER_H
