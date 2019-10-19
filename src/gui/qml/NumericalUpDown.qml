import QtQuick 2.0

Rectangle {
    id: root
    height: 32
    width: (root.max + "").length * data.font.pixelSize + 24
    color: "#232323"
    state: "NORMAL"

    function update(val) {
        if(val <= root.max && val >= root.min) {
            root.value = val
            data.text = root.value
        }
    }

    function unfocus() {
        data.focus = false;
    }

    function show() {
        root.visible = true
        root.state = "NORMAL"
    }

    function hide() {
        root.state = "HIDDEN"
    }

    property int value: 0
    property int min: 0
    property int max: 10

    signal up
    signal down
    signal modified
    signal changed

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: root
                opacity: 1
            }
        },
        State {
            name: "HIDDEN"
            PropertyChanges {
                target: root
                opacity: 0
            }
            onCompleted: {
                root.visible = false
            }
        }
    ]

    transitions: [
        Transition {
            from: "NORMAL"
            to: "HIDDEN"
            PropertyAnimation { target: root; property: "opacity"; duration: 50 }
        },
        Transition {
            from: "HIDDEN"
            to: "NORMAL"
            PropertyAnimation { target: root; property: "opacity"; duration: 1000 }
        }
    ]

    Row {
        anchors.fill: parent
        anchors.margins: 2

        TextInput {
            id: data
            height: parent.height
            width: parent.width - 20
            color: "#F6F6F6"
            font.family: "PT Sans"
            font.pixelSize: 18
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter
            clip: true
            text: root.value

            onFocusChanged: {
                if(data.focus == false) {
                    var temp = parseInt(data.text)
                    if(temp <= root.max && temp >= root.min) {
                        root.value = temp
                        root.modified()
                        root.changed()
                    } else {
                        data.text = root.value
                    }
                }
            }
        }

        Column {
            height: parent.height
            width: 20

            Item {
                width: parent.width
                height: 2
            }

            Rectangle {
                id: up
                width: 20
                height: 12
                color: "transparent"

                Image {
                    id: upImage
                    width: 12
                    height: 12
                    anchors.centerIn: parent
                    rotation: -90
                    source: "qrc:/images/arrow_light.png"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: upImage.source = "qrc:/images/arrow_white.png"
                    onExited: upImage.source = "qrc:/images/arrow_light.png"
                    onPressed: upImage.source = "qrc:/images/arrow_light.png"
                    onReleased: {
                        if(root.value + 1 > root.max) {
                            root.value = root.min
                        } else {
                            root.value++
                        }
                        data.text = root.value
                        root.up()
                        root.changed()
                        upImage.source = "qrc:/images/arrow_white.png"
                    }
                }
            }

            Rectangle {
                id: down
                width: 20
                height: 12
                color: "transparent"

                Image {
                    id: downImage
                    width: 12
                    height: 12
                    anchors.centerIn: parent
                    rotation: 90
                    source: "qrc:/images/arrow_light.png"
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: downImage.source = "qrc:/images/arrow_white.png"
                    onExited: downImage.source = "qrc:/images/arrow_light.png"
                    onPressed: downImage.source = "qrc:/images/arrow_light.png"
                    onReleased: {
                        if(root.value - 1 < root.min) {
                            root.value = root.max
                        } else {
                            root.value--
                        }
                        data.text = root.value
                        root.down()
                        root.changed()
                        downImage.source = "qrc:/images/arrow_white.png"
                    }
                }
            }
        }

    }
}
