import QtQuick 2.8
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1
Window {
    signal sendcontent(string sendc)
    width: 920; height: 60
    id :send
    title: qsTr("通知栏消息发送")
    visible: false	//该窗口一开始默认隐藏的
    GridLayout{
       // anchors.centerIn: parent
        anchors.fill: parent
        anchors.margins: 10
        columns: 2

        Keys.onPressed: {
            switch(event.key){
            case Qt.Key_Return :
               send.visible = false;
               sendcontent(sendtext.text);
               sendtext.text = "" ;
                break;
            }
        }

        TextField {
            id : sendtext
            Layout.fillWidth: true
            Layout.fillHeight: true
            //width: 700
            anchors.top: parent.top
            placeholderText: "发送内容"


            //lay
        }
        Button{
             //anchors.top: parent.top
            //Layout.fillWidth: true
            text: "发送"
            onClicked: {
                send.visible = false
                sendcontent(sendtext.text)
                sendtext.text = ""

            }
        }
    }
}
