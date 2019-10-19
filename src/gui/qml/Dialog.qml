import QtQuick 2.0

Rectangle {
    id: cover
    anchors.fill: parent
    color: "#EE232323"

    property var dialog
    property list<Button> buttons
    property alias dialogHeight: container.height
    property alias dialogWidth: box.width
    property alias dialogTitle: title.text

    state: "HIDDEN"

    function close() {
        cover.state = "HIDDEN"
    }

    function show() {
        parent.visible = true
        cover.visible = true
        cover.state = "NORMAL"
    }

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: cover
                opacity: 1
            }
        },
        State {
            name: "HIDDEN"
            PropertyChanges {
                target: cover
                opacity: 0
            }
            onCompleted: {
                parent.visible = false
                cover.visible = false
            }
        }
    ]

    transitions: [
        Transition {
            from: "NORMAL"
            to: "HIDDEN"
            PropertyAnimation { target: cover; property: "opacity"; duration: 300 }
        },
        Transition {
            from: "HIDDEN"
            to: "NORMAL"
            PropertyAnimation { target: cover; property: "opacity"; duration: 300 }
        }
    ]

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
    }

    Rectangle {
        id: box
        width: 100
        height: container.height + 80
        color: "white"
        anchors.centerIn: parent

        Rectangle {
            id: titleBox
            anchors { top: parent.top; left: parent.left; right: parent.right; topMargin: 10; leftMargin: 10 }
            height: 20

            Text {
                id: title
                wrapMode: Text.WordWrap
                font.family: "PT Sans"
                font.capitalization: Font.AllUppercase
                font.pixelSize: 16
                color: "#232323"
            }
        }

        Item {
            id: container
            anchors { top: titleBox.bottom; left: parent.left; right: parent.right }

            Item {
                property alias info: info

                anchors { top: container.top; left: parent.left; right: parent.right }
                children: cover.dialog
            }
        }

        Row {
            anchors { top: container.bottom; left: parent.left; right: parent.right; bottom: parent.bottom; margins: 10 }
            height: 40
            spacing: 5

            property alias info: info

            children: cover.buttons
        }

        Rectangle {
            id: info
            anchors { top: parent.top; bottom: parent.bottom; left: parent.left }
            color: "#1B7E5A"

            property string message

            Connections {
                target: schedules
                onGenerating: {
                    info.message = qsTr("Generating...")
                    info.state = "STATUS"
                }

                onGenerated: {
                    msg.text = ""
                    button.opacity = 0
                    info.state = "HIDDEN"
                }
            }

            function show(message) {
                info.message = message
                info.state = "NORMAL"
            }

            function hide() {
                msg.text = ""
                button.opacity = 0
                info.state = "HIDDEN"
            }

            state: "HIDDEN"

            states: [
                State {
                    name: "NORMAL"
                    PropertyChanges {
                        target: info
                        width: info.parent.width
                    }
                    onCompleted: {
                        msg.text = info.message
                        button.opacity = 1
                    }
                },
                State {
                    name: "STATUS"
                    PropertyChanges {
                        target: info
                        width: info.parent.width
                    }
                    onCompleted: {
                        msg.text = info.message
                        button.opacity = 0
                    }
                },
                State {
                    name: "HIDDEN"
                    PropertyChanges {
                        target: info
                        width: 0
                    }
                }
            ]

            transitions: [
                Transition {
                    from: "NORMAL"
                    to: "HIDDEN"
                    NumberAnimation { target: info; property: "width"; duration: 200; easing.type: Easing.InOutQuad }
                },
                Transition {
                    from: "STATUS"
                    to: "HIDDEN"
                    NumberAnimation { target: info; property: "width"; duration: 200; easing.type: Easing.InOutQuad }
                },
                Transition {
                    from: "HIDDEN"
                    to: "NORMAL"
                    NumberAnimation { target: info; property: "width"; duration: 200; easing.type: Easing.InOutQuad }
                },
                Transition {
                    from: "HIDDEN"
                    to: "STATUS"
                    NumberAnimation { target: info; property: "width"; duration: 200; easing.type: Easing.InOutQuad }
                }
            ]

            Text {
                id: msg
                anchors.centerIn: parent
                width: parent.width - 30
                wrapMode: Text.WordWrap
                font.family: "PT Sans"
                font.pixelSize: 12
                color: "white"
            }

            Rectangle {
                id: button
                opacity: 0
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: 8
                anchors.rightMargin: 8
                width: 20
                height: 20
                color: "#1B7E5A"
                border { color: "#166549"; width: 1 }

                Image {
                    id: buttonImage
                    anchors.centerIn: parent
                    width: 12
                    height: 12
                    source: "qrc:/images/close.png"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        button.color = "#166549"
                    }
                    onExited: {
                        button.color = "#1B7E5A"
                    }
                    onPressed: {
                        button.color = "#1B7E5A"
                    }
                    onReleased: {
                        info.hide()
                    }
                }
            }
        }
    }
}
