#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "timecomponent.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<TimeComponent>("custom", 1, 0, "TimeComponent");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
