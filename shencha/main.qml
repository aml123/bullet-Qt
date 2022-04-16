
import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MqttClient 1.0
//import "Tzl.qml"
Window {
    visible: true
//    width: 640
//    height: 480
    width: 940
    height: 780
    title: qsTr("弹幕审核")
    id: root

    Tzl{
        id:tzl  //通知栏的对象
    }


    property var tempSubscription: 0  //订阅客户端对象
    property var msgid: 0
    property var jsonmsg: 0 //获取到的发送端发送过来的数据
    property var count: 0
    property var jsonfile: 0 //客户端配置文件(JSON格式)
    property var baiduAPI: 0 //是否开启百度文本审核( 1:开启 ; 0:关闭 )
    MqttClient {
        id: client
        Component.onCompleted: {
            var SettingFile = client.GetSettingFile(":/shencha.config");  //读取配置文件
   //         console.log(SettingFile);
            jsonfile = JSON.parse(SettingFile)
            client.setHostname(jsonfile.Hostname)
            client.setPort(jsonfile.Port)
            client.setProtocolVersion(jsonfile.ProtocolVersion)
            client.setToken(jsonfile.token)
            baiduAPI = jsonfile.BaiduAPI;
            client.connectToHost();  //连接客户端

        }
    }

    ListModel {
        id: messageModel
    }

    function addMessage(result)
    {
        console.log("result :"+result);
        if(result === "合规" || result === "疑似")
        {
messageModel.insert(0, {"payload" : jsonmsg.msg})

        if (messageModel.count >= 100)
            messageModel.remove(99)
}
    }
    function badidushencha(payload){   //使用百度AI开放平台进行敏感词检测
       if(baiduAPI === "1")  //开启敏感词过滤
        {
        jsonmsg = JSON.parse(payload)
         console.log("payload = "+jsonmsg.msg);
        client.getBaiduApiReturn(jsonmsg.msg)
        }
        else if(baiduAPI === "0")   //关闭敏感词过滤
       {
           jsonmsg = JSON.parse(payload)
           messageModel.insert(0, {"payload" : jsonmsg.msg})

           if (messageModel.count >= 100)
               messageModel.remove(99)
       }
    }

    Connections{
        target: client
        function onConnected(){
             tempSubscription = client.subscribe("$share/a/lsybysj")
            tempSubscription.messageReceived.connect(badidushencha)
                        client.conclusionResult.connect(addMessage)
        }
    }
    Connections{
        target: tzl
        function onSendcontent(sendtext){    //将通知栏的信息以JSON格式发送出去，此时tzl位不为null
            msgid=client.publishqml("testtopic/linsy",sendtext,0,false,"nonull")
        }
    }
//    Component.onCompleted: {
//        tzl.sendcontent.connect(tzlreceived)
//    }
//    function tzlreceived(sendtext){
//        console.log(sendtext)
//    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 2
        Button {
            id:mgcbt
           Layout.fillWidth: true
           text: baiduAPI === "1"? "敏感词过滤开启" : "敏感词过滤关闭"
           onClicked: {
                if(baiduAPI === "1"){
                    baiduAPI = "0"
                }
                else if(baiduAPI === "0"){
                    baiduAPI = "1"
                }
               // console.log("api :"+baiduAPI)
           }
        }

        Button {
            id:newwindowbutton
//            anchors.left: parent.left
//            anchors.right: parent.right
           // anchors.margins: 1
            Layout.fillWidth: true
            text: "发送通知"
            onClicked: {
                tzl.show()
            }
        }

        ListView {
            id: messageView
            anchors.top: newwindowbutton.bottom
         //   anchors.topMargin: 10
            anchors.bottom: labelstate.top
//            anchors.fill: parent
           // anchors.margins: 10
            anchors.topMargin: 10
      //      columns: 2
            model: messageModel
            height: 300
            width: 200
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            delegate: Rectangle {
                width: messageView.width
                height: 60
                color: index % 2 ? "#DDDDDD" : "#888888"
                radius: 5
                Text {
                    text: payload
                    anchors.centerIn: parent
                }
                Button {
                    text: "接受"
                    anchors.right: jujue.left
                    anchors.rightMargin: 5
                    anchors.top: jujue.top
                    anchors.topMargin: parent.top
                    height: 50
                    background: Rectangle {
                                   radius: 5
                                   color: "#8696a7"
                                   }
                    onClicked: {
                        msgid=client.publishqml("testtopic/linsy",payload,0,false,"null")
                          messageModel.remove(index);
                    }
                }

                Button {
                    id :jujue
                    text: "拒绝"
                    anchors.right: parent.right
                    anchors.rightMargin: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    height: 50
                    background: Rectangle {
                                   radius: 5
                                   color: "#965454"
                                   }
                    onClicked: {
                        messageModel.remove(index);
                    }

                }
            }
        }

        Label {
            function stateToString(value) {
                if (value === 0)
                    return "未连接"
                else if (value === 1)
                    return "连接中"
                else if (value === 2)
                    return "已连接"
                else
                    return "未知"
            }
            id:labelstate
            Layout.columnSpan: 2
            Layout.fillWidth: true
            color: "#333333"
            text: "MQTT客户端连接状态:" + stateToString(client.state)   //显示连接状态
            enabled: client.state === MqttClient.Connected
        }
    }
}
