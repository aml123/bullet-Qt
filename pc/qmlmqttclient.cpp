

#include "qmlmqttclient.h"
#include <QDebug>

QmlMqttClient::QmlMqttClient(QObject *parent)
    : QMqttClient(parent)
{
}

QmlMqttSubscription* QmlMqttClient::subscribe(const QString &topic)
{
    auto sub = QMqttClient::subscribe(topic, 0);  //MQTT客户端订阅主题Topic
    auto result = new QmlMqttSubscription(sub, this);
    return result;   //返回订阅对象
}

QmlMqttSubscription::QmlMqttSubscription(QMqttSubscription *s, QmlMqttClient *c)
    : sub(s)
    , client(c)
{
    connect(sub, &QMqttSubscription::messageReceived, this, &QmlMqttSubscription::handleMessage);  //连接处理消息接收后信号从而接入处理消息函数
    m_topic = sub->topic();
}

QmlMqttSubscription::~QmlMqttSubscription()
{
}

void QmlMqttSubscription::handleMessage(const QMqttMessage &qmsg)    //处理消息函数
{
    QString str=qmsg.payload();    //获取到消息中的有效部分(即具体的弹幕)
    QString str2=str;
   // QString str2=str.section('"', 3,3);
    emit messageReceived(str2);   //发送信息接收的信号
}
QString QmlMqttClient::GetSettingFile(const QString &filename)
{
    QFile * file = new QFile(filename);
    QString content;
    if( content.length() == 0 ) {
            file->open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(file);
            content = in.readAll();
            if( content.length() == 0) {
                qDebug() << "[Warning] FileContent: file " << filename << "is empty" << Qt::endl;
            }
        }
        return content;
}
