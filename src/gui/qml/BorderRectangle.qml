import QtQuick 2.0

Rectangle {
    property var borderWidth: 1
    property var borderColor: "#2E2E2E"
    property alias borderTopVisible: topBorder.visible
    property alias borderTopWidth: topBorder.height
    property alias borderTopColor: topBorder.color
    property alias borderBottomVisible: bottomBorder.visible
    property alias borderBottomWidth: bottomBorder.height
    property alias borderBottomColor: bottomBorder.color
    property alias borderLeftVisible: leftBorder.visible
    property alias borderLeftWidth: leftBorder.width
    property alias borderLeftColor: leftBorder.color
    property alias borderRightVisible: rightBorder.visible
    property alias borderRightWidth: rightBorder.width
    property alias borderRightColor: rightBorder.color

    Rectangle {
        id: leftBorder
        width: parent.borderWidth
        height: parent.height
        anchors.left: parent.left
//        color: "#EBE7DE"
//        color: "#2E2E2E"
        color: parent.borderColor
    }
    Rectangle {
        id: rightBorder
        width: parent.borderWidth
        height: parent.height
        anchors.right: parent.right
        color: parent.borderColor
    }
    Rectangle {
        id: topBorder
        width: parent.width
        height: parent.borderWidth
        anchors.top: parent.top
        color: parent.borderColor
    }
    Rectangle {
        id: bottomBorder
        width: parent.width
        height: parent.borderWidth
        anchors.bottom: parent.bottom
        color: parent.borderColor
    }
}
