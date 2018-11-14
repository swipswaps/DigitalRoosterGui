import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4

Popup {
	focus: true
	closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

	enter: Transition {
		NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 ; duration: 300}
	}
	exit: Transition {
		NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 ; duration: 400}
	}

    contentItem: GridLayout {
		rowSpacing: 2;
		columnSpacing: 4;
		rows: 2;
		columns:2;

		Tumbler{
			id: timeTumbler
			//Layout.maximumWidth: 80
			Layout.maximumHeight: 96 
			Layout.rowSpan: 2
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop
			Layout.fillHeight: true;
			
			TumblerColumn {
				id: hoursTumbler
				model: 24
			}
			TumblerColumn {
				id: minutesTumbler
				model: 60
			}
		}
		//----------
		
		Switch{
			id: enaAlarm;
			Layout.minimumWidth: 100
			Layout.preferredWidth: 200
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop
			position: alarmlistmodel.get_alarm(alarmlistmodel.currentIndex).enabled;
			text: alarmlistmodel.get_alarm(alarmlistmodel.currentIndex).enabled ? qsTr("enabled") : qsTr("disabled")
			
			onCheckedChanged:{
				alarmlistmodel.set_enabled(alarmlistmodel.currentIndex, position)
			}
		}

		Label{
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop
			text: timeTumbler.currentIndexAt(0) +" : " +  timeTumbler.currentIndexAt(1)
		}

	}
}
