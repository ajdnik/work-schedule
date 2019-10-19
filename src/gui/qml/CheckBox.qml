import QtQuick 2.0

Item {
    id: root
    height: 24
    width: msg.width + 20

    property alias text: msg.text
    property bool checkState: false

    signal checkChanged

    function uncheck() {
        box.state = "HOVER"
        root.checkState = false
    }

    Row {
        anchors.fill: parent
        spacing: 3

        Rectangle {
            id: box
            width: 16
            height: 16
            anchors.top: parent.top
            anchors.topMargin: 4

            state: root.checkState ? "CHECKED" : "NORMAL"

            Image {
                id: tick
                anchors.centerIn: parent
                width: 12
                height: 12
                source: "qrc:/images/tick.png"
            }

            states: [
                State {
                    name: "NORMAL"
                    PropertyChanges {
                        target: box
                        border { color: "#D3CFC8"; width: 1 }
                    }
                    PropertyChanges {
                        target: box
                        color: "transparent"
                    }
                    PropertyChanges {
                        target: tick
                        visible: false
                    }
                },
                State {
                    name: "HOVER"
                    PropertyChanges {
                        target: box
                        border { color: "#1B7E5A"; width: 2 }
                    }
                    PropertyChanges {
                        target: box
                        color: "transparent"
                    }
                    PropertyChanges {
                        target: tick
                        visible: false
                    }
                },
                State {
                    name: "CHECKED"
                    PropertyChanges {
                        target: box
                        border { color: "#1B7E5A"; width: 2 }
                    }
                    PropertyChanges {
                        target: box
                        color: "#1B7E5A"
                    }
                    PropertyChanges {
                        target: tick
                        visible: true
                    }
                }
            ]

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    if(box.state == "NORMAL")
                        box.state = "HOVER"
                }
                onExited: {
                    if(box.state == "HOVER")
                        box.state = "NORMAL"
                }
                onReleased: {
                    if(box.state == "HOVER") {
                        box.state = "CHECKED"
                        root.checkState = true
                    } else {
                        box.state = "HOVER"
                        root.checkState = false
                    }
                    root.checkChanged()
                }
            }
        }
        Text {
            id: msg
            anchors { top: parent.top; bottom: parent.bottom; topMargin: 3 }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignLeft
            width: msg.text * 10 + 10
            wrapMode: Text.WordWrap
            font.pixelSize: 14
            font.family: "PT Sans Narrow"
            color: "#232323"
        }
    }
}
