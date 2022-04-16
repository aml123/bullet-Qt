import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id : root
    height: 30;
    property real endx: screen.desktopAvailableWidth;
    property alias textstr: roottext.text
    Text {
        id : roottext
        color: Qt.rgba(Math.random()+0.1,Math.random()+0.05,Math.random(),1);
        function getFontSize(){
            var SettingFile = client.GetSettingFile(":/shencha.config");

                        var jsonfile = JSON.parse(SettingFile)
            console.log(jsonfile.FontSize);
            return jsonfile.FontSize
        }

        font.pointSize: getFontSize()
    }
    NumberAnimation on  x{
        from : Math.random()*20
        to : endx ; duration:20000
        onStopped: root.destroy(1)
    }

}
