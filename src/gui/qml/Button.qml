import QtQuick 2.0

Rectangle {
    id: element
    width: title.font.pixelSize * title.text.length + 20
    height: title.font.pixelSize + 20

    property alias text: title.text

    signal clicked

    state: "NORMAL"

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: title
                color: "white"
            }
            PropertyChanges {
                target: element
                color: "#1B7E5A"
            }
        },
        State {
            name: "HOVER"
            PropertyChanges {
                target: title
                color: "white"
            }
            PropertyChanges {
                target: element
                color: "#166549"
            }
        },
        State {
            name: "DISABLED"
            PropertyChanges {
                target: title
                color: "#A6A39D"
            }
            PropertyChanges {
                target: element
                color: "#DDD8CD"
            }
        }

    ]

    Text {
        id: title
        anchors.centerIn: parent
        font.pixelSize: 14
        font.capitalization: Font.AllUppercase
        font.family: "PT Sans"
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            if(parent.state != "DISABLED")
                parent.state = "HOVER"
        }
        onExited: {
            if(parent.state != "DISABLED")
                parent.state = "NORMAL"
        }
        onPressed: {
            if(parent.state != "DISABLED")
                parent.state = "NORMAL"
        }
        onReleased: {
            if(parent.state != "DISABLED") {
                parent.state = "HOVER"
                parent.clicked()
            }
        }
    }
}
