import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
import MqttClient 1.0

ApplicationWindow {
    visible: true
    width: screen.desktopAvailableWidth
    height: screen.desktopAvailableHeight
    title: qsTr("弹幕")
    color: "transparent"
    id: ap
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.NoDropShadowWindowHint |
               Qt.WindowStaysOnTopHint | Qt.WindowDoesNotAcceptFocus | Qt.WindowTransparentForInput  //设置窗口透明、全屏、鼠标穿透

    property var tempSubscription: 0  //订阅客户端对象
    property var jsonmsg: 0 //接收审核端的发送过来的数据
    property var jsonfile: 0 //客户端配置文件(JSON格式)
    MqttClient {
        id: client
        Component.onCompleted: {
            var SettingFile = client.GetSettingFile(":/shencha.config"); //读取配置文件
          //  console.log(SettingFile);
            jsonfile = JSON.parse(SettingFile)
            client.setHostname(jsonfile.Hostname)
            client.setPort(jsonfile.Port)
            client.setProtocolVersion(jsonfile.ProtocolVersion)
            client.connectToHost();  //连接客户端


        }
    }
    Connections{
        target: client
        function onConnected(){
            tempSubscription = client.subscribe("testtopic/linsy")
            // tempSubscription = client.subscribe("$share/a/lsybysj")
                               tempSubscription.messageReceived.connect(addItem)  //消息接收后填充弹幕
        }
    }

    property int index: 0

    function addItem(payload)
    {
        jsonmsg = JSON.parse(payload)
        console.log("tzl :"+jsonmsg.tzl)
        if(jsonmsg.tzl === "null")  //如果不是通知栏更改的消息则填充弹幕
   {
     var str=jsonmsg.msg;
        console.log(str)
        var od=Math.random()*500%200;
        for(var i=0 ; i<1 ; ++i){
            var component = Qt.createComponent("qrc:/TextItem.qml");
            if(component.status === Component.Ready){
                var textitem = component.createObject(ap);
                od +=30;
                textitem.y = od-30;
                index = Number(od%5);
                textitem.textstr = str;
            }
        }
}

    }


}
