import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.classicjems as ClassicJems

Item {
    id: root
    width: 320
    height: 480

    ClassicJems.ClassicJemsLauncher {
        id: launcher
    }

    Rectangle {
        anchors.fill: parent
        color: "#0F0F12"
        radius: 8

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            RowLayout {
                Layout.fillWidth: true
                Label {
                    text: "ClassicJems Launcher"
                    color: "#FFFFFF"
                    font.pixelSize: 18
                    font.bold: true
                }
                Item { Layout.fillWidth: true }
            }

            // Quick list of J2ME launcher shortcuts
            ListView {
                id: gameListView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: ListModel {
                    ListElement { name: "Bounce Tales"; path: "BounceTales.jar"; platform: "J2ME" }
                    ListElement { name: "Doom RPG"; path: "DoomRPG.jar"; platform: "J2ME" }
                    ListElement { name: "Super Mario Run"; path: "SuperMarioRun.jar"; platform: "J2ME" }
                    ListElement { name: "N-Gage System"; path: "NGageSystem.sis"; platform: "Symbian" }
                }

                delegate: ItemDelegate {
                    width: gameListView.width
                    height: 50
                    
                    background: Rectangle {
                        color: hovered ? "#1A1A22" : "transparent"
                        border.color: "#292932"
                        border.width: 1
                        radius: 6
                    }

                    contentItem: RowLayout {
                        spacing: 12
                        anchors.fill: parent
                        anchors.margins: 8

                        Rectangle {
                            width: 24
                            height: 24
                            color: "#7F5AF0"
                            radius: 4
                            Text {
                                anchors.centerIn: parent
                                text: platform[0]
                                color: "#FFFFFF"
                                font.bold: true
                                font.pixelSize: 10
                            }
                        }

                        ColumnLayout {
                            spacing: 2
                            Text {
                                text: name
                                color: "#E1E1E6"
                                font.bold: true
                                font.pixelSize: 13
                            }
                            Text {
                                text: path
                                color: "#72728A"
                                font.pixelSize: 11
                            }
                        }
                    }

                    onDoubleClicked: {
                        launcher.launch(path, platform);
                    }
                }
            }

            Label {
                Layout.fillWidth: true
                text: "Double click to play game ROM"
                color: "#72728A"
                font.pixelSize: 11
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
