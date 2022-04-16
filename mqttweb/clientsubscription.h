#ifndef CLIENTSUBSCRIPTION_H
#define CLIENTSUBSCRIPTION_H

#include "websocketiodevice.h"

#include <QtCore/QObject>
#include <QtMqtt/QMqttClient>
#include <QtWebSockets/QWebSocket>
#include <QtMqtt/QMqttSubscription>
class ClientSubscription : public QObject
{
    Q_OBJECT
public:
    ClientSubscription(QObject *parent = nullptr);

    Q_INVOKABLE void setUrl(const QUrl &url); // ie ws://broker.hivemq.com:8000/mqtt
    Q_INVOKABLE void setTopic(const QString &topic);
    Q_INVOKABLE void setVersion(int v);
    Q_INVOKABLE void publish2();
    Q_INVOKABLE void connectAndSubscribe();
    Q_INVOKABLE void publish(const QString &topic, const QString &message, int qos, bool retain);
signals:
    void messageReceived(QByteArray);
    void errorOccured();
    void statechanged();

public slots:
    void handleMessage(const QByteArray &msgContent);

private:
    QMqttClient m_client;
    QMqttSubscription *m_subscription;
    QUrl m_url;
    QString m_topic;
    WebSocketIODevice m_device;
    int m_version;
};

#endif // CLIENTSUBSCRIPTION_H
