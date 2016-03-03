#ifndef QCIMAGELOADERWRAPPER_H
#define QCIMAGELOADERWRAPPER_H

#include <QObject>

/// Wrapper of QCImageLoader

class QCImageLoaderWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY isLoadedChanged)

public:
    explicit QCImageLoaderWrapper(QObject *parent = 0);

    bool running() const;

    bool isLoaded() const;

signals:
    void runningChanged();
    void isLoadedChanged();

private slots:
    void updateRunning();
    void updateIsLoaded();

private:
    void setRunning(bool running);
    void setIsLoaded(bool isLoaded);

    bool m_running;
    bool m_isLoaded;
};

#endif // QCIMAGELOADERWRAPPER_H
