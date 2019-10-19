import QtQuick 2.0

Rectangle {
    id: root
    color: "white"

    property alias model: table.model
    property list<TableColumn> columns
    property int selectedIndex: -1
    property alias currentIndex: table.currentIndex
    property string idColumn: "id"
    property var currentId
    property var currentModel

    function isSelected() {
        return root.currentIndex > -1
    }

    signal selectionChanged

    Rectangle {
        id: header
        width: parent.width
        height: 30
        color: "#232323"
        Row {
            anchors.fill: parent
            Repeater {
                model: root.columns.length
                Rectangle {
                    anchors { top: parent.top; bottom: parent.bottom }
                    width: parent.width / root.columns.length
                    color: "transparent"

                    Rectangle {
                        anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
                        width: 1
                        color: "#2E2E2E"
                        visible: index == 0 ? false : true
                    }

                    Text {
                        text: root.columns[index].name
                        anchors.centerIn: parent
                        color: "white"
                        font.family: "PT Sans"
                        font.pixelSize: 14
                        font.capitalization: Font.AllUppercase
                    }
                }
            }
        }
    }

    ListView {
        id: table
        anchors { top: header.bottom; left: parent.left; right: parent.right }
        height: root.height - 30
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        property var currentRow

        highlight: Rectangle {
            color: "#1B7E5A"
            anchors { left: parent.left; right: parent.right }
            height: 30
        }

        delegate: Rectangle {
            id: row
            width: parent.width
            height: 30
            color: "transparent"
            property var currentModel: model

            Column {
                anchors.fill: parent
                Row {
                    anchors { left: parent.left; right: parent.right }
                    height: 29
                    Repeater {
                        model: root.columns.length
                        Rectangle {
                            anchors { top: parent.top; bottom: parent.bottom }
                            width: parent.width / root.columns.length
                            color: "transparent"

                            Rectangle {
                                color: "#E5E5E5"
                                anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
                                width: 1
                                visible: index == 0 ? false : true
                            }

                            Text {
                                text: row.currentModel[root.columns[index].source]
                                anchors.centerIn: parent
                                font.family: "PT Sans"
                            }
                        }
                    }
                }

                Rectangle {
                    id: spliter
                    width: parent.width
                    height: 1
                    color: "#E5E5E5"
                }
            }

            MouseArea {
                anchors.fill: parent

                onDoubleClicked: {
                    table.currentIndex = index
                    root.currentId = model[root.idColumn]
                    root.currentModel = model
                    root.selectionChanged()
                }
            }
        }
    }
}
