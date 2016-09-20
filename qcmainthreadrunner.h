#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QEventLoop>

class QCMainThreadRunner
{

public:

    /// Run a function on main thread. If it is already in main thread, it will be executed in next tick.
    template <typename F>
    static void run(F func) {
        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), std::move(func), Qt::QueuedConnection);
    }

    /// Run a function with custom data on main thread. If it is already in main thread, it will be executed in next tick.
    template <typename F,typename P1>
    static void run(F func,P1 p1) {
        auto wrapper = [=]() -> void{
            func(p1);
        };

        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), std::move(wrapper), Qt::QueuedConnection);
    }

    template <typename F,typename P1, typename P2>
    static void run(F func,P1 p1, P2 p2) {
        auto wrapper = [=]() -> void{
            func(p1, p2);
        };

        QObject tmp;
        QObject::connect(&tmp, &QObject::destroyed, QCoreApplication::instance(), std::move(wrapper), Qt::QueuedConnection);
    }

    template <typename F>
    static void blockingRun(F func) {
        QEventLoop* loop = new QEventLoop();

        auto wrapper = [=]() -> void {
            func();

            QObject emitter2;
            QObject::connect(&emitter2, &QObject::destroyed,
                             loop, &QEventLoop::quit, Qt::QueuedConnection);
        };

        QObject* emitter1 = new QObject();

        QObject::connect(emitter1, &QObject::destroyed, QCoreApplication::instance(),
                         std::move(wrapper), Qt::QueuedConnection);
        delete emitter1;

        loop->exec();
        delete loop;
    }

    template <typename F, typename ... Args>
    static auto blockingRunRet(F func) -> decltype(func()) {
        QEventLoop* loop = new QEventLoop();
        decltype(func()) t;

        auto wrapper = [&]() -> void {
            t = func();

            QObject emitter2;
            QObject::connect(&emitter2, &QObject::destroyed,
                             loop, &QEventLoop::quit, Qt::QueuedConnection);
        };

        QObject* emitter1 = new QObject();

        QObject::connect(emitter1, &QObject::destroyed, QCoreApplication::instance(),
                         std::move(wrapper), Qt::QueuedConnection);
        delete emitter1;

        loop->exec();
        delete loop;

        return t;
    }
};
