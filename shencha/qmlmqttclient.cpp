/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmlmqttclient.h"
#include <QDebug>
#include <QNetworkRequest>
QmlMqttClient::QmlMqttClient(QObject *parent)
    : QMqttClient(parent)
{
}

QmlMqttSubscription* QmlMqttClient::subscribe(const QString &topic)
{
    auto sub = QMqttClient::subscribe(topic, 0);
    auto result = new QmlMqttSubscription(sub, this);
    qDebug() <<"subscribe";
    return result;
}

QmlMqttSubscription::QmlMqttSubscription(QMqttSubscription *s, QmlMqttClient *c)
    : sub(s)
    , client(c)
{
    connect(sub, &QMqttSubscription::messageReceived, this, &QmlMqttSubscription::handleMessage);
    m_topic = sub->topic();
}

QmlMqttSubscription::~QmlMqttSubscription()
{
}

void QmlMqttSubscription::handleMessage(const QMqttMessage &qmsg)
{
    emit messageReceived(qmsg.payload());
}
int QmlMqttClient::publishqml(const QString &topic, const QString &message, int qos, bool retain,const QString &tzl)
{
    QJsonObject json;
    json.insert("msg",message);
    json.insert("tzl",tzl);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    //int k=QMqttClient::publish(QMqttTopicName(topic),QByteArray(message.toUtf8()),qos,retain);
    int k=QMqttClient::publish(QMqttTopicName(topic),byteArray,qos,retain);
         qDebug() <<"k: "<<k<<"run here";
          qDebug() << "clientstate:"<<QMqttClient::state();;
          return k;
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
void  QmlMqttClient::getBaiduApiReturn(const QByteArray &inputcontent){   //调用百度API实现敏感词查询
    QUrl url;
    url.setUrl("https://aip.baidubce.com/rest/2.0/solution/v1/text_censor/v2/user_defined?access_token="+token);    //通过
    QNetworkAccessManager * pNetManager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    QByteArray byteArr;
    byteArr.append("text="+inputcontent);
    connect(pNetManager,&QNetworkAccessManager::finished,this,&QmlMqttClient::reganizeResult);
    pNetManager->post(request, byteArr);
}
void QmlMqttClient::reganizeResult(QNetworkReply* pReply){  //获取百度API的post请求的结果
    QNetworkReply::NetworkError err = pReply->error();
    if(err != QNetworkReply::NoError) {
    qDebug() << "Failed: " << pReply->readAll();
    }
    else{
        QJsonParseError jsonErr;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(pReply->readAll(),&jsonErr);
        if(jsonDoc.isObject()){
            QJsonObject jsonObj = jsonDoc.object();
            QString conclusion = jsonObj.take("conclusion").toString();  //获得结果
            qDebug() << "conclusion : " << conclusion;
            //emit messageReceived(qmsg.payload());
            emit conclusionResult(conclusion);
        }
    }
}
void QmlMqttClient::setToken(const QString qmlSetToken){
   token = qmlSetToken;   //设置百度的Token
}
