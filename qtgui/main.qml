import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import QtMultimedia 5.8
/* import QtQuick.Controls.Universal 2.1 */
import Qt.labs.settings 1.0

import ruschi.PodcastEpisode 1.0

import "Icon.js" as MdiFont
import "Jsutil.js" as Util

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 480
    height: 320
    property alias playerControlWidget: playerControlWidget
    property string functionMode: "Clock"

    title: qsTr("DigitalRooster")

    Clock{
        id: currentTime
    }

	FontLoader { 
		id: materialdesignIconsFont;
		source: "materialdesignicons-webfont.ttf" 
	}

    header: ToolBar {
        Material.foreground: "white"
        RowLayout {
            spacing: 10
            anchors.fill: parent

            IconButton {
                text: MdiFont.Icon.menu
                onClicked: {
                    drawer.open()
                }
            }
            Label {
                id: titleLabel
                text: currentTime.timestring_lz
                font.pixelSize: 20
                elide: Label.ElideRight
                Layout.fillWidth: true
            }

            IconButton {
                id : backButton
                text: MdiFont.Icon.keyboardBackspace
                visible: (stackView.depth > 1)

                onClicked:{
                    if (stackView.depth > 1){
                        stackView.pop()
                        console.log("BackButton")
                    }
                    console.log("CurrentItem :" + stackView.currentItem.objectName)
                }

                Shortcut {
                    sequence: StandardKey.Back
                    onActivated: backButton.onClicked();
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: applicationWindow.width * 0.2
        height: applicationWindow.height
        interactive: true

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: IconButton {
                width: parent.width
                text: model.title
                highlighted: listView.currentIndex == index

                onClicked: {
                    if(listView.currentIndex != index){
                        listView.currentIndex = index

                        stackView.pop(null)
                        stackView.push(model.source)
                        console.log("CurrentItem :" + stackView.currentItem.objectName)
                    }
                    drawer.close()
                }
            }

            model: ListModel {
                ListElement { title: "\uf150"; source: "qrc:/ClockPage.qml";   objectName:"ClockPage"; }
                ListElement { title: "\uf223"; source: "qrc:/PodcastList.qml"; objectName:"PodcastList"; }
            }
        }

    }

    Rectangle {
        id: playerControlWidget

        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.8
        height: parent.height*0.3
        anchors.horizontalCenterOffset: 0
        anchors.bottomMargin: 0
        visible: false
        z: 1
        anchors.bottom: parent.bottom

        MediaPlayer {
            id: player

            property PodcastEpisode currentEpisode;

            onPlaybackStateChanged: {
                switch (playbackState){
                case MediaPlayer.PlayingState:
                    console.log("player.playing")
                    playBtn.text =  MdiFont.Icon.pause
                    break
                case MediaPlayer.PausedState:
                    console.log("player.paused")
                    playBtn.text = MdiFont.Icon.play
                    break
                case MediaPlayer.StoppedState:
                    console.log("player.stopped")
                    playBtn.text = MdiFont.Icon.play
                    break
                default:
                    console.log("player???")
                }
            }

            onPositionChanged: {
                if (currentEpisode != null){
                    currentEpisode.position = position
                    slider.value = position/currentEpisode.duration
                    timeElapsed.text = Util.msToTime(position)
                }
            }
        }


        function playEpisode(newEpisode){
            if(newEpisode == null){
                console.log("newEpisode is null")
            }
            if( player.currentEpisode != null){
                // remeber last position
                player.currentEpisode.position = player.position
            }
            player.stop()
            durationTotal.text = Util.msToTime(newEpisode.duration)

            var oldpos = newEpisode.position
            console.log("new episodes has positions set to "+ oldpos)
            player.currentEpisode = newEpisode
            player.source = newEpisode.url
            player.play()
            player.seek(oldpos) // restore previous position
            setVisible(true)
        }

        function setVisible(visible){
            interactiontimer.restart()
            playerControlWidget.visible=visible
        }

        Timer {
            id: interactiontimer
            interval: 2500;
            running: true;
            repeat: false;
            onTriggered: parent.setVisible(false)
        }


        IconButton {
            id: playBtn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top : parent.top
            anchors.topMargin: 2
            enabled: (player.currentEpisode != null)
            text: MdiFont.Icon.play // default to play icon

            onClicked: {
                console.log("playBtn")
                interactiontimer.restart()

                if(player.playbackState == MediaPlayer.PlayingState){
                    player.pause()
                }
                else{
                    player.play()
                }
            }
        }

        IconButton {
            id: forwardBtn
            anchors.left: playBtn.right
            anchors.leftMargin: 25
            anchors.top: playBtn.top
            text: MdiFont.Icon.fastForward
            onClicked: {
                console.log("forwardBtn")
                interactiontimer.restart()
                player.seek(player.position+5000)
            }
        }

        IconButton {
            id: backwardBtn
            anchors.right: playBtn.left
            anchors.rightMargin: 25

            text: MdiFont.Icon.rewind
            onClicked: {
                console.log("backwardBtn")
                interactiontimer.restart()
                player.seek(player.position-5000)
            }
        }

        Slider {
            id: slider
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width*0.85
            anchors.top: playBtn.bottom
            anchors.topMargin: -15
            value: player.position/player.duration
            enabled: (player.currentEpisode != null)

            onValueChanged: {
                interactiontimer.restart()
            }
            onMoved:{
                player.seek(value* player.currentEpisode.duration)
            }
        }
        Text{
            id: timeElapsed
            text: player.position
            anchors.horizontalCenter: slider.left
            anchors.top: slider.bottom
            anchors.margins: 2
        }
        Text{
            id: durationTotal
            anchors.horizontalCenter: slider.right
            anchors.top: slider.bottom
            anchors.margins: 2
        }
    }


    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: ClockPage{
            id:initalClockPage
            property string objectName : "InitialPage"
        }
    }
}
