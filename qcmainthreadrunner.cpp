#include "qcmainthreadrunner.h"

void QCMainThreadRunner::waitForFinished(QThreadPool &pool)
{
    QEventLoop loop;

    while (!pool.waitForDone(10)) {
        loop.processEvents();
    }
}
