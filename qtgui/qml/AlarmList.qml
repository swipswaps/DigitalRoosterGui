import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import ruschi.Alarm 1.0

ListView {
    id:alarmlist
    property string objectName : "Alarms"
    width: stackView.width;
    height: stackView.height;
    contentWidth: stackView.width;
    rebound: listBoundTransition;

    delegate: AlarmDelegate{
        id: alarmdelegate
    }

    AlarmEditDialog{
        id: alarmEditDlg;
        width: Style.contentWidth*0.8;
        x: Math.round((applicationWindow.width - width)/2)
        bottomMargin:15;
    }
    model: alarmlistmodel

    RoundButton {
        text: qsTr("+")
        highlighted: true
        width: 56;
        height: 56;
        anchors.margins: 10;
        anchors.left: parent.left;
        anchors.bottom: parent.bottom;
        onClicked: {
            alarmEditDlg.currentAlarm = alarmlistmodel.create_alarm();
            alarmlist.currentIndex = alarmlistmodel.rowCount()-1;
            alarmEditDlg.open();
        }
    }
}
