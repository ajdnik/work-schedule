import QtQuick 2.0

Rectangle {
    id: root
    width: 130
    height: 36
    color: "#232323"
    z: 999

    property alias model: view.model
    property string modelProperty
    property string modelId: "id"
    property var currentId: -1
    property int displayedElements: 5

    signal changed

    function reset() {
        root.currentId = -1;
        title.text = "";
    }

    function set(text, id) {
        root.currentId = id;
        title.text = text;
    }

    Rectangle {
        width: 90
        height: 24
        color: "white"
        anchors { left: parent.left; top: parent.top; leftMargin: 5; topMargin: 6 }

        Text {
            id: title
            text: ""
            anchors.centerIn: parent
            verticalAlignment: Text.AlignLeft
            font.family: "PT Sans"
            font.pixelSize: 12
            font.capitalization: Font.AllUppercase
            color: "#232323"
        }
    }

    Image {
        width: 16
        height: 16
        source: "qrc:/images/category_light.png"
        anchors { top: parent.top; right: parent.right; topMargin: 10; rightMargin: 10 }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onEntered:  {
                parent.source = "qrc:/images/category_white.png"
            }
            onExited: {
                parent.source = "qrc:/images/category_light.png"
            }
            onPressed: {
                parent.source = "qrc:/images/category_light.png"
            }
            onReleased: {
                parent.source = "qrc:/images/category_white.png"
                if(menu.state == "HIDDEN")
                    menu.show()
                else
                    menu.hide()
            }
        }
    }

    Rectangle {
        id: menu
        anchors { left: parent.left; right: parent.right; top: parent.bottom }
        color: "#232323"

        state: "HIDDEN"

        function show() {
            menu.visible = true
            view.visible = true
            menu.state = "NORMAL"
        }

        function hide() {
            menu.state = "HIDDEN"
        }

        states: [
            State {
                name: "NORMAL"
                PropertyChanges {
                    target: menu
                    height: root.displayedElements * 20 + 10
                }

                PropertyChanges {
                    target: view
                    height: root.displayedElements * 20
                }
            },
            State {
                name: "HIDDEN"
                PropertyChanges {
                    target: menu
                    height: 0
                }
                PropertyChanges {
                    target: view
                    height: 0
                }
                onCompleted: {
                    menu.visible = false
                    view.visible = false
                }
            }

        ]

        transitions: [
            Transition {
                from: "NORMAL"
                to: "HIDDEN"
                NumberAnimation { target: menu; property: "height"; duration: 200 }
                NumberAnimation { target: view; property: "height"; duration: 150 }
            },
            Transition {
                from: "HIDDEN"
                to: "NORMAL"
                NumberAnimation { target: menu; property: "height"; duration: 200 }
                NumberAnimation { target: view; property: "height"; duration: 250 }
            }
        ]

        ListView {
            id: view
            anchors.fill: parent
            anchors.margins: 5
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            snapMode: ListView.SnapToItem

            delegate: Rectangle {
                width: parent.width
                height: 21
                color: "transparent"

                Rectangle {
                    id: row
                    width: parent.width
                    height: 20
                    anchors.top: parent.top
                    color: "white"

                    Text {
                        id: item
                        text: model[root.modelProperty]
                        anchors.centerIn: parent
                        font.family: "PT Sans"
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            row.color = "#1B7E5A"
                        }
                        onExited: {
                            row.color = "white"
                        }
                        onPressed: {
                            row.color = "white"
                        }
                        onDoubleClicked: {
                            title.text = item.text
                            root.currentId = model[root.modelId]
                            menu.hide()
                            root.changed()
                        }
                    }
                }
            }
        }
    }
}
