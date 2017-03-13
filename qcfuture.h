#ifndef QCFUTURE_H
#define QCFUTURE_H

#include <QFuture>
#include <QObject>

namespace QCFuture
{
    namespace Private {

        template <typename R>
        class Delegate {
        public:

            template <typename T, typename F, typename D>
            static void call(QFuture<T> future,F f, D defer)  {
                R r = f(future.result());
                defer->reportResult(r);
            }

        };

        template <>
        class Delegate<void> {
        public:

            template<typename T, typename F, typename D>
            static void call(QFuture<T> future, F f, D defer) {
                Q_UNUSED(defer);
                f(future.result());
            }
        };

    }

    template <typename T>
    class Defer : public QObject, public QFutureInterface<T>{
    public:
        inline Defer(QObject* parent = 0): QObject(parent) {}
    };


    template <typename T,typename F>
    auto subscribe(QFuture<T> future, F func, QObject* context) -> QFuture<decltype(func(future.result()))>  {

        auto *defer = new Defer<decltype(func(future.result()))> (context);

        QFutureWatcher<T> *watcher = new QFutureWatcher<T>(context);
        watcher->setFuture(future);

        QObject::connect(watcher, &QFutureWatcher<T>::finished,[=](){
            Private::Delegate<decltype(func(future.result()))>::call(future, func, defer);
            defer->reportFinished();
            watcher->deleteLater();
            defer->deleteLater();
        });

        return defer->future();
    }
}

#endif // QCFUTURE_H
