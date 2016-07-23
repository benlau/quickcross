#ifndef QCMAINTHREADRUNNER_H
#define QCMAINTHREADRUNNER_H

#include <QObject>
#include <QCoreApplication>

class QCMainThreadRunner
{

public:

    /// Run a function on main thread. If it is already in main thread, it will be executed in next tick.
    template <typename F>
    static void start(F func) {
        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), std::move(func), Qt::QueuedConnection);
    }

    /// Run a function with custom data on main thread. If it is already in main thread, it will be executed in next tick.
    template <typename F,typename P1>
    static void start(F func,P1 p1) {
        auto wrapper = [=]() -> void{
            func(p1);
        };

        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), std::move(wrapper), Qt::QueuedConnection);
    }

    template <typename F,typename P1, typename P2>
    static void start(F func,P1 p1, P2 p2) {
        auto wrapper = [=]() -> void{
            func(p1, p2);
        };

        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), std::move(wrapper), Qt::QueuedConnection);
    }
};

#endif // QCMAINTHREADRUNNER_H
