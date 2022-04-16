#include "clientsubscription.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QLoggingCategory>
#include <QtMqtt/QMqttSubscription>
Q_LOGGING_CATEGORY(lcWebSocketMqtt, "qtdemo.websocket.mqtt")

ClientSubscription::ClientSubscription(QObject *parent) : QObject(parent)
{
    connect(this, &ClientSubscription::errorOccured, qApp, &QCoreApplication::quit);
   // connect(this, &ClientSubscription::statechanged,this,&ClientSubscription::publish());
}

void ClientSubscription::setUrl(const QUrl &url)
{
    m_url = url;
}

void ClientSubscription::setTopic(const QString &topic)
{
    m_topic = topic;
}

void ClientSubscription::setVersion(int v)
{
    m_version = v;
}

void ClientSubscription::connectAndSubscribe()
{
    qCDebug(lcWebSocketMqtt) << "Connecting to broker at " << m_url;

    m_device.setUrl(m_url);
    m_device.setProtocol(m_version == 5 ? "mqttv5" : "mqtt");

    connect(&m_device, &WebSocketIODevice::socketConnected, this, [this]() {
        qCDebug(lcWebSocketMqtt) << "WebSocket connected, initializing MQTT connection.";

        m_client.setProtocolVersion(m_version == 5 ? QMqttClient::MQTT_5_0 : QMqttClient::MQTT_3_1_1);
        m_client.setTransport(&m_device, QMqttClient::IODevice);

        connect(&m_client, &QMqttClient::connected, this, [this]() {
            qCDebug(lcWebSocketMqtt) << "MQTT connection established";

            m_subscription = m_client.subscribe(m_topic);
            if (!m_subscription) {
                qDebug() << "Failed to subscribe to " << m_topic;
                emit errorOccured();
            }

//            connect(m_subscription, &QMqttSubscription::stateChanged,
//                    [this](QMqttSubscription::SubscriptionState s) {
//                qCDebug(lcWebSocketMqtt) << "Subscription state changed:" << s;
//                emit statechanged();
//            });

            connect(m_subscription, &QMqttSubscription::messageReceived,
                    [this](QMqttMessage msg) {
                handleMessage(msg.payload());
            });
        });

        m_client.connectToHost();
//        connect(m_subscription, &QMqttSubscription::stateChanged,
//                [this](QMqttSubscription::SubscriptionState s) {
//            int k=m_client.publish(QMqttTopicName("testtopic/linsy"),"helloQt",0,false);
//             qDebug() <<"k: "<<k<<"run here";
//              qDebug() << "clientstate:"<<m_client.state();;
//        });



    });
    if (!m_device.open(QIODevice::ReadWrite))
        qDebug() << "Could not open socket device";
    //return m_subscription;
  //  emit connected2();
}

void ClientSubscription::handleMessage(const QByteArray &msgContent)
{
    // Should happen when the internal device has ready read?
    qInfo() << "New message:" << msgContent;
    emit messageReceived(msgContent);
}
void ClientSubscription::publish(const QString &topic, const QString &message, int qos, bool retain)
{

    if(m_client.state() == QMqttClient::Connected ){
        qDebug() <<topic<<" "<<message<<" "<<qos<<" "<<retain<<" "<<m_client.state();
        int k=m_client.publish(QMqttTopicName(topic), message.toUtf8(), qos, retain);
        // return result;
        qDebug() <<"k: "<<k<<"run here";
    }
}
void ClientSubscription::publish2()
{

    int k=m_client.publish(QMqttTopicName("testtopic/linsy"),"helloQt",0,false);
     qDebug() <<"k: "<<k<<"run here";
      qDebug() << "clientstate:"<<m_client.state();;
}
