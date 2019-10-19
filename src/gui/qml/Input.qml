import QtQuick 2.0

Rectangle {
    id: box
    height: 32
    width: 100
    color: "white"
    border { color: "#DDD8CD"; width: 2 }

    property alias text: data.text

    TextInput {
        id: data
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        color: "#232323"
        font.family: "PT Sans Narrow"
        font.pixelSize: 18
        horizontalAlignment: TextInput.AlignLeft
        verticalAlignment: TextInput.AlignVCenter
    }
}
