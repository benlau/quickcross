#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCImageLoader>
#include <QCImageProvider>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);


    QCImageLoader* loader = QCImageLoader::instance();

    loader->load("qrc:///img");
    loader->waitForLoaded();

    QQmlApplicationEngine engine;
    engine.addImageProvider("arts", new QCImageProvider());

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
