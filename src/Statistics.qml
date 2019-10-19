import QtQuick 2.0
import "qrc:/qml/"

Rectangle {
    id: root
    color: "white"
    property var source

    Row {
        height: parent.height
        width: parent.width

        Row {
            height: parent.height
            width: parent.width - 150

            Repeater {
                model: root.source.size
                Column {
                    height: parent.height
                    width: parent.width / root.source.size
                    Item {
                        width: parent.width
                        height: 20
                    }
                    Item {
                        width: parent.width
                        height: parent.height - 45
                        Rectangle {
                            width: parent.width * 0.8
                            height: parent.height * root.source.value[index]

                            Behavior on height {NumberAnimation{}}

                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: "#1B7E5A"
                            Text {
                                anchors.bottom: parent.top
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: root.source.real_val[index]
                                font.pixelSize: 10
                                font.family: "PT Sans"
                                font.capitalization: Font.AllUppercase
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                    Item {
                        id: textContainer
                        width: parent.width
                        height: 25
                        property var textValue : root.source.name[index]

                        Text {
                            id: textSize
                            font.pixelSize: 12
                            font.family: "PT Sans"
                            font.capitalization: Font.AllUppercase
                            visible: false
                        }

                        Text {
                            id: textField
                            anchors.centerIn: parent
                            font.pixelSize: 12
                            font.family: "PT Sans"
                            font.capitalization: Font.AllUppercase
                        }

                        onTextValueChanged: {
                            if(textContainer.width > 0) {
                                textSize.text = textContainer.textValue;
                                var fullWidth = textSize.paintedWidth;
                                if(fullWidth > textContainer.width) {
                                    var insertText = "";
                                    var counter = 1;
                                    do {
                                        textSize.text = textContainer.textValue.substring(0, counter) + "...";
                                        counter++;
                                    } while(textSize.paintedWidth < textContainer.width);
                                    counter -= 2;
                                    textField.text = textContainer.textValue.substring(0, counter) + "...";
                                } else {
                                    textField.text = textContainer.textValue;
                                }
                            }
                        }

                        onWidthChanged: {
                            if(textContainer.width > 0) {
                                textSize.text = textContainer.textValue;
                                var fullWidth = textSize.paintedWidth;
                                if(fullWidth > textContainer.width) {
                                    var insertText = "";
                                    var counter = 1;
                                    do {
                                        textSize.text = textContainer.textValue.substring(0, counter) + "...";
                                        counter++;
                                    } while(textSize.paintedWidth < textContainer.width);
                                    counter -= 2;
                                    textField.text = textContainer.textValue.substring(0, counter) + "...";
                                } else {
                                    textField.text = textContainer.textValue;
                                }
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            height: parent.height
            width: 2
            color: "#EBE7DE"
        }

        Column {
            height: parent.height
            width: 148
            Item {
                width: parent.width
                height: 10
            }
            Item {
                width: parent.width
                height: 20
                Text {
                    font.pixelSize: 14
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    color: "#1B7E5A"
                    font.capitalization: Font.AllUppercase
                    text: qsTr("Load Avg")
                }
            }
            Item {
                width: parent.width
                height: 12
                Text {
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    font.capitalization: Font.AllUppercase
                    text: root.source.stat[0]
                }
            }
            Item {
                width: parent.width
                height: 20
                Text {
                    font.pixelSize: 14
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    color: "#1B7E5A"
                    font.capitalization: Font.AllUppercase
                    text: qsTr("Load Std")
                }
            }
            Item {
                width: parent.width
                height: 12
                Text {
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    font.capitalization: Font.AllUppercase
                    text: root.source.stat[1]
                }
            }
            Item {
                width: parent.width
                height: 10
            }
            Item {
                width: parent.width
                height: 20
                Text {
                    font.pixelSize: 14
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    color: "#1B7E5A"
                    font.capitalization: Font.AllUppercase
                    text: qsTr("Bonus Avg")
                }
            }
            Item {
                width: parent.width
                height: 12
                Text {
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    font.capitalization: Font.AllUppercase
                    text: root.source.stat[2]
                }
            }
            Item {
                width: parent.width
                height: 20
                Text {
                    font.pixelSize: 14
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    color: "#1B7E5A"
                    font.capitalization: Font.AllUppercase
                    text: qsTr("Bonus Std")
                }
            }
            Item {
                width: parent.width
                height: 12
                Text {
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    font.capitalization: Font.AllUppercase
                    text: root.source.stat[3]
                }
            }
            Item {
                width: parent.width
                height: 10
            }
            Item {
                width: parent.width
                height: 20
                Text {
                    font.pixelSize: 14
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    color: "#1B7E5A"
                    font.capitalization: Font.AllUppercase
                    text: qsTr("Events Avg")
                }
            }
            Item {
                width: parent.width
                height: 12
                Text {
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    font.capitalization: Font.AllUppercase
                    text: root.source.stat[4]
                }
            }
            Item {
                width: parent.width
                height: 20
                Text {
                    font.pixelSize: 14
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    color: "#1B7E5A"
                    font.capitalization: Font.AllUppercase
                    text: qsTr("Events Std")
                }
            }
            Item {
                width: parent.width
                height: 12
                Text {
                    font.pixelSize: 10
                    anchors.centerIn: parent
                    font.family: "PT Sans"
                    font.capitalization: Font.AllUppercase
                    text: root.source.stat[5]
                }
            }
        }
    }
}
