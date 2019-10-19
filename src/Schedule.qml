import QtQuick 2.0
import "qrc:/qml/"

Item {
    id: root
    property var entryId
    property var workerNumber

    function reset() {
        root.entryId = 0
        root.workerNumber = 0
        scheduleRow.currentCell = 0
        scheduleRow.twinCell = 0
    }

    Column {
        width: parent.width
        height: parent.height

        BorderRectangle {
            width: parent.width
            height: 40
            color: "#232323"
            borderTopVisible: false
            borderLeftVisible: false
            borderRightVisible: false
            Text {
                anchors.centerIn: parent
                text: schedules.monthName
                font.pixelSize: 16
                font.family: "PT Sans"
                color: "white"
                font.capitalization: Font.AllUppercase
            }
        }
        Row {
            id: scheduleRow
            width: parent.width
            height: parent.height - 140

            property var currentCell
            property var twinCell

            Column {
                width: 100
                height: parent.height
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "white"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Monday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "white"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Tuesday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "white"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Wednesday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "white"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Thursday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "white"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Friday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "#1B7E5A"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Saturday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
                BorderRectangle {
                    width: parent.width
                    height: parent.height / 7
                    borderTopVisible: false
                    borderLeftVisible: false
                    borderColor: "#E5E5E5"
                    color: "#166549"
                    Text {
                        anchors.centerIn: parent
                        text: qsTr("Sunday")
                        font.pixelSize: 14
                        font.family: "PT Sans"
                        font.capitalization: Font.AllUppercase
                    }
                }
            }
            Repeater {
                model: schedules.columnCount
                Column {
                    id: columnsRepeater
                    property int columnIndex: index
                    width: 30 + ((parent.width - (100 + schedules.columnCount * 30)) / schedules.columnCount)
                    height: parent.height
                    Row {
                        width: parent.width
                        height: parent.height
                        Column {
                            width: 30
                            height: parent.height
                            Repeater {
                                model: 7
                                BorderRectangle {
                                    width: parent.width
                                    height: parent.height / 7
                                    borderTopVisible: false
                                    borderLeftVisible: false
                                    borderColor: "#E5E5E5"
                                    color: schedules.dayColors[columnsRepeater.columnIndex * 7 + index]
                                    Text {
                                        anchors.centerIn: parent
                                        text: schedules.dayNumbers[columnsRepeater.columnIndex * 7 + index]
                                        font.pixelSize: 14
                                        font.family: "PT Sans"
                                        font.capitalization: Font.AllUppercase
                                    }
                                }
                            }
                        }
                        Column {
                            width: parent.width - 30
                            height: parent.height
                            Repeater {
                                model: schedules.columns[columnsRepeater.columnIndex].rowCount
                                Row {
                                    id: rowRepeater
                                    property int rowIndex: index
                                    width: parent.width
                                    height: (parent.height / (7 * 24)) * (schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].duration)
                                    Repeater {
                                        model: schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].workerCount
                                        BorderRectangle {
                                            id: workerRepeater
                                            property int workerIndex: index
                                            property var workerName: schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].workers[workerRepeater.workerIndex]
                                            property var currentText
                                            width: parent.width / (schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].workerCount)
                                            height: parent.height
                                            borderTopVisible: false
                                            borderLeftVisible: false
                                            borderColor: "#E5E5E5"
                                            color: schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].color
                                            Text {
                                                id: nameSize
                                                font.pixelSize: 12
                                                font.family: "PT Sans Narrow"
                                                visible: false
                                            }
                                            Text {
                                                id: nameField
                                                anchors.centerIn: parent
                                                font.pixelSize: 12
                                                font.family: "PT Sans Narrow"
                                            }

                                            MouseArea {
                                                id: mouseArea
                                                visible: schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].color == "transparent" ? false : true
                                                anchors.fill: parent
                                                hoverEnabled: true

                                                onEntered: {
                                                    workerRepeater.currentText = nameField.text
                                                    nameField.text = "B:" + schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].bonus
                                                }

                                                onExited: {
                                                    nameField.text = workerRepeater.currentText
                                                }

                                                onDoubleClicked: {
                                                    if(scheduleRow.currentCell != parent && scheduleRow.twinCell != parent) {
                                                        if(scheduleRow.currentCell) {
                                                            scheduleRow.currentCell.color = "white"
                                                            if(scheduleRow.twinCell)
                                                                scheduleRow.twinCell.color = "white"
                                                        }
                                                        root.entryId = schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].id
                                                        root.workerNumber = workerRepeater.workerIndex
                                                        parent.color = "#1B7E5A"
                                                        scheduleRow.currentCell = parent
                                                        if(schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].nextTwin) {
                                                            var nextTwin = scheduleRow.children[columnsRepeater.columnIndex + 2].children[0].children[1].children[0].children[workerRepeater.workerIndex];
                                                            nextTwin.color = "#1B7E5A"
                                                            scheduleRow.twinCell = nextTwin
                                                        } else if(schedules.columns[columnsRepeater.columnIndex].rows[rowRepeater.rowIndex].previousTwin) {
                                                            var prevTwin = scheduleRow.children[columnsRepeater.columnIndex].children[0].children[1].children[schedules.columns[columnsRepeater.columnIndex - 1].rowCount - 1].children[workerRepeater.workerIndex];
                                                            prevTwin.color = "#1B7E5A"
                                                            scheduleRow.twinCell = prevTwin
                                                        } else {
                                                            scheduleRow.twinCell = 0
                                                        }
                                                    }
                                                }
                                            }

                                            onWorkerNameChanged: {
                                                if(workerRepeater.width > 0) {
                                                    nameSize.text = workerRepeater.workerName;
                                                    var fullWidth = nameSize.paintedWidth;
                                                    if(fullWidth > workerRepeater.width) {
                                                        var insertText = "";
                                                        var counter = 1;
                                                        do {
                                                            nameSize.text = workerRepeater.workerName.substring(0, counter) + "...";
                                                            counter++;
                                                        } while(nameSize.paintedWidth < workerRepeater.width);
                                                        counter -= 2;
                                                        nameField.text = workerRepeater.workerName.substring(0, counter) + "...";
                                                    } else {
                                                        nameField.text = workerRepeater.workerName;
                                                    }
                                                }
                                            }

                                            onWidthChanged: {
                                                if(workerRepeater.width > 0) {
                                                    nameSize.text = workerRepeater.workerName;
                                                    var fullWidth = nameSize.paintedWidth;
                                                    if(fullWidth > workerRepeater.width) {
                                                        var insertText = "";
                                                        var counter = 1;
                                                        do {
                                                            nameSize.text = workerRepeater.workerName.substring(0, counter) + "...";
                                                            counter++;
                                                        } while(nameSize.paintedWidth < workerRepeater.width);
                                                        counter -= 2;
                                                        nameField.text = workerRepeater.workerName.substring(0, counter) + "...";
                                                    } else {
                                                        nameField.text = workerRepeater.workerName;
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        BorderRectangle {
            width: parent.width
            height: 100
            borderTopVisible: false
            borderLeftVisible: false
            borderRightVisible: false
            borderBottomVisible: false
            color: "white"
            BorderRectangle {
                id: noteBox
                width: 100
                height: 30
                borderTopVisible: false
                borderLeftVisible: false
                anchors { left: parent.left; top: parent.top }
                color: "#232323"
                Text {
                    anchors.centerIn: parent
                    text: qsTr("Notes:")
                    font.pixelSize: 14
                    font.family: "PT Sans"
                    color: "white"
                    font.capitalization: Font.AllUppercase
                }
            }
            Rectangle {
                width: parent.width
                height: 60
                anchors { left: parent.left; top: noteBox.bottom }
                Text {
                    horizontalAlignment: parent.left
                    verticalAlignment: parent.verticalCenter
                    text: schedules.note
                    font.pixelSize: 12
                    font.family: "PT Sans"
                    color: "#232323"
                }
            }
        }
    }
}
