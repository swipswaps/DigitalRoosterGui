import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Extras 1.4
import ruschi.Alarm 1.0
import "Jsutil.js" as Util


Popup {
	property Alarm currentAlarm;
	property int index;
	focus: true
	closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

	enter: Transition {
		NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 ; duration: 300}
	}
	exit: Transition {
		NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 ; duration: 400}
	}

    contentItem: GridLayout {
		rowSpacing: 2;
		columnSpacing: 6;
		rows: 3;
		columns:2;

		Tumbler{
			id: timeTumbler
			Layout.maximumHeight: 96
			Layout.rowSpan: 2
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop

			TumblerColumn {
				id: hoursTumbler
				model: 24
				width: 48
				delegate: Text {
    				text: styleData.value
    				font.pointSize: 16;
					font.bold: true;
					horizontalAlignment: Text.AlignHCenter
    				opacity: 0.4 + Math.max(0, 1 - Math.abs(styleData.displacement)) * 0.6
				}
			}
			TumblerColumn {
				id: minutesTumbler
				model: 60
				width: 48
				delegate: Text {
    				text: styleData.value
    				font.pointSize: 16;
					font.bold: true;
					horizontalAlignment: Text.AlignHCenter
    				opacity: 0.4 + Math.max(0, 1 - Math.abs(styleData.displacement)) * 0.6
				}
			}
		}
		//----------

		Switch{
			id: enaAlarm;
			Layout.minimumWidth: 120
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop
			position: currentAlarm.enabled
			text: currentAlarm.enabled ? qsTr("enabled") : qsTr("disabled")

			onCheckedChanged:{
				currentAlarm.enabled= position;
			}
		}

		ComboBox {
			id: period
			Layout.minimumWidth: 120
			Layout.preferredWidth: 160
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop
			model: ListModel {
				id: model
				ListElement { text: qsTr("Once") }
				ListElement { text: qsTr("Daily") }
				ListElement { text: qsTr("Weekend") }
				ListElement { text: qsTr("Workdays") }
			}
			currentIndex: currentAlarm.period_id;

			onActivated: {
				console.log("new index" + currentIndex);
				currentAlarm.period_id = currentIndex;
			}
		}

		ComboBox {
			id: stations
			Layout.minimumWidth: 120
			Layout.preferredWidth: parent.width
			Layout.alignment: Qt.AlignLeft| Qt.AlignTop

			Layout.columnSpan: 2
			model: iradiolistmodel
			textRole: "station_name";

			onActivated: {
				console.log("new station" + currentIndex);
				currentAlarm.url = iradiolistmodel.get_station_url(currentIndex);
			}
		}
	} // Gridlayout

	onAboutToShow : {
		timeTumbler.setCurrentIndexAt(0,Util.get_hours(currentAlarm.time))
		timeTumbler.setCurrentIndexAt(1,Util.get_minutes(currentAlarm.time))

		for (var i=0; i<iradiolistmodel.rowCount() ; i++){
			if(iradiolistmodel.get_station_url(i) === currentAlarm.url){
				stations.currentIndex = i;
				console.log(iradiolistmodel.get_station_url(i) + " = idx " +i);
				break;
			}
		}
	}

	onAboutToHide : {
		var now = currentAlarm.time;
		var h_idx =timeTumbler.currentIndexAt(0);
		var m_idx = timeTumbler.currentIndexAt(1);
		now.setHours(h_idx);
		now.setMinutes(m_idx);
		currentAlarm.time = now;
		alarmlistmodel.update_row(alarmlistmodel.currentIndex);
	}
}
