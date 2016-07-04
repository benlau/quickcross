#ifndef QCMAINTHREADRUNNER_H
#define QCMAINTHREADRUNNER_H

#include <QObject>
#include <QVariant>

class QCMainThreadRunner : public QObject
{
    Q_OBJECT
public:

    typedef void (*Callback)(void* data);

    /// Run a function with custom data on main thread. If it is already in main thread, it will be executed in next tick.
    /** @threadsafe
     */
    static void start(Callback func, void* data);

signals:

public slots:

private slots:
    void run(QVariant func, QVariant data);

protected:
    static QCMainThreadRunner* instance();

private:
    explicit QCMainThreadRunner(QObject *parent = 0);
};

#endif // QCMAINTHREADRUNNER_H
