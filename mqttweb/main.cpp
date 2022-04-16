#include "clientsubscription.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QLoggingCategory>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
//    QCoreApplication::setApplicationVersion("1.0");

//    QCommandLineParser parser;
//    parser.setApplicationDescription(QLatin1String("Websocket MQTT subscription tool"));
//    auto help = parser.addHelpOption();

    // Use http://www.hivemq.com/demos/websocket-client/ in browser to publish
//    QCommandLineOption urlOption(QStringList() << "host" << "url" << "broker",
//                                 QStringLiteral("Host to connect to, eg ws://broker.hivemq.com:8000/mqtt"),
//                                 "host");
//    parser.addOption(urlOption);

//    QCommandLineOption subscriptionOption(QStringList() << "t" << "topic",
//                                          QStringLiteral("Topic to subscribe to"), "topic");
//    parser.addOption(subscriptionOption);

//    QCommandLineOption debugOption(QStringList() << "d" << "debug",
//                                   QStringLiteral("Enable Debug mode"));
//    parser.addOption(debugOption);

//    QCommandLineOption versionOption(QStringList() << "v" << "version",
//                                     QStringLiteral("MQTT protocol version.\n3: MQTT 3.1\n4: MQTT 3.1.1"),
//                                     "version", "3");
//    parser.addOption(versionOption);

//    parser.process(a.arguments());

//    const QString debugLog = QString::fromLatin1("qtdemo.websocket.mqtt*=%1").arg(
//                                parser.isSet(debugOption) ? "true" : "false");
//    QLoggingCategory::setFilterRules(debugLog);

//    ClientSubscription clientsub;
//    clientsub.setUrl(QUrl("ws://broker.emqx.io:8083/mqtt"));
//    clientsub.setTopic(parser.value(subscriptionOption));

//    const QString versionString = parser.value(versionOption);

//    if (versionString == "4") {
//        clientsub.setVersion(4);
//    } else if (versionString == "5") {
//        clientsub.setVersion(5);
//    } else {
//        qInfo() << "Unknown MQTT version";
//        return -2;
//    }

//    clientsub.connectAndSubscribe();
//     clientsub.publish("testtopic/linsy","i am Qtclient",0,false);
//    qInfo() <<"run qInfo";
    QGuiApplication app(argc, argv);
 QQmlApplicationEngine engine;
    qmlRegisterType<ClientSubscription>("MqttClient", 1, 0, "MqttClient");
 //   qmlRegisterUncreatableType<QmlMqttSubscription>("MqttClient", 1, 0, "MqttSubscription", QLatin1String("Subscriptions are read-only"));
    engine.load(QUrl(QStringLiteral("qrc:/qml.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
     return app.exec();
}
