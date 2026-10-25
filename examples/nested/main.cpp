#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>

#include <taffy/Taffy.h>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    taffy::registerQmlTypes();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("%1/main.qml").arg(TAFFY_EXAMPLE_QML_DIR));

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl) {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine.load(url);
    return app.exec();
}
