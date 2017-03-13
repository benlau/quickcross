#ifndef QCFUTURE_H
#define QCFUTURE_H

#include <QFuture>
#include <QObject>

namespace QCFuture
{

    template <typename T>
    class Defer : public QObject, public QFutureInterface<T>{
    public:
        inline Defer(QObject* parent = 0): QObject(parent) {}
    };


    template <typename T,typename F>
    auto subscribe(QFuture<T> future, F func, QObject* lifetime) -> QFuture<decltype(func(future.result()))>  {

        auto *defer = new Defer<decltype(func(future.result()))> (lifetime);

        QFutureWatcher<T> *watcher = new QFutureWatcher<T>(lifetime);
        watcher->setFuture(future);

        QObject::connect(watcher, &QFutureWatcher<T>::finished,[=](){
            defer->reportResult(func(future.result()));
            defer->reportFinished();
            watcher->deleteLater();
            defer->deleteLater();
        });

        return defer->future();
    }
}

#endif // QCFUTURE_H
