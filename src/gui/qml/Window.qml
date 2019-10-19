import QtQuick 2.0

Rectangle {
    id: window
    width: 1024
    height: 768

    property list<Container> containers
    property list<Dialog> dialogs
    property alias icons: icons
    property alias font: ptSans

    property var callback

    function confirmBox(message, callback) {
        msg.text = message
        window.callback = callback
        cover.show()
    }

    function timerBox(message, time) {
        timerT.interval = time
        timer.show(message)
    }

    QtObject {
        id: icons
        property int calendar: 1
        property int users: 2
        property int cog: 3
    }

    FontLoader {
        id: ptSans
        source: "qrc:/fonts/PT_Sans-Web-Bold.ttf"
    }

    FontLoader {
        id: ptSans2
        source: "qrc:/fonts/PT_Sans-Narrow-Web-Regular.ttf"
    }

    Rectangle {
        id: mainMenu
        width: 80
        anchors { top: parent.top; left: parent.left; bottom:parent.bottom }
        color: "#232323"

        property var currentButton

        Column {
            anchors.fill: parent

            Repeater {
                id: buttons
                model: window.containers.length

                Rectangle {
                    id: button
                    anchors { left: parent.left; right: parent.right }
                    height: 80

                    property string normalIcon: {
                        if(window.containers[index].icon == window.icons.calendar) {
                            "qrc:/images/5_white.png"
                        } else if(window.containers[index].icon == window.icons.users) {
                            "qrc:/images/4_white.png"
                        } else if(window.containers[index].icon == window.icons.cog) {
                            "qrc:/images/12_white.png"
                        }
                    }

                    property string hoverIcon: {
                        if(window.containers[index].icon == window.icons.calendar) {
                            "qrc:/images/5_darker.png"
                        } else if(window.containers[index].icon == window.icons.users) {
                            "qrc:/images/4_darker.png"
                        } else if(window.containers[index].icon == window.icons.cog) {
                            "qrc:/images/12_darker.png"
                        }
                    }

                    state: {
                        if(window.containers[index].active) {
                            mainMenu.currentButton = button
                            "ACTIVE"
                        } else {
                            "NORMAL"
                        }
                    }

                    states: [
                        State {
                            name: "NORMAL"
                            PropertyChanges {
                                target: button
                                color: "transparent"
                            }
                            PropertyChanges {
                                target: image
                                source: normalIcon
                            }
                        },
                        State {
                            name: "HOVER"
                            PropertyChanges {
                                target: button
                                color: "#2E2E2E"
                            }
                            PropertyChanges {
                                target: image
                                source: normalIcon
                            }
                        },
                        State {
                            name: "ACTIVE"
                            PropertyChanges {
                                target: button
                                color: "white"
                            }
                            PropertyChanges {
                                target: image
                                source: hoverIcon
                            }
                        }
                    ]

                    Image {
                        id: image
                        height: 32
                        width: 32
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered: {
                            if(mainMenu.currentButton != parent)
                                parent.state = "HOVER"
                        }
                        onExited: {
                            if(mainMenu.currentButton != parent)
                                parent.state = "NORMAL"
                        }
                        onPressed: {
                            if(mainMenu.currentButton != parent) {
                                mainMenu.currentButton.state = "NORMAL"
                                mainArea.currentArea.state = "HIDDEN"
                                mainArea.currentArea.visible = false
                                parent.state = "ACTIVE"
                                mainMenu.currentButton = parent
                                var area = areas.itemAt(index)
                                area.visible = true
                                area.state = "NORMAL"
                                mainArea.currentArea = area
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: mainArea
        anchors { top: parent.top; bottom: parent.bottom; right: parent.right; left: mainMenu.right }
        color: "white"

        property var currentArea

        Repeater {
            id: areas
            model: window.containers.length

            Rectangle {
                id: area
                anchors.fill: parent
                anchors.margins: 30
                visible: {
                    if(window.containers[index].active) {
                        true
                    } else {
                        false
                    }
                }

                state: {
                    if(window.containers[index].active) {
                        mainArea.currentArea = area
                        "NORMAL"
                    } else {
                        "HIDDEN"
                    }
                }

                Behavior on opacity {PropertyAnimation{}}

                states: [
                    State {
                        name: "NORMAL"
                        PropertyChanges {
                            target: area
                            opacity: 1
                        }
                    },
                    State {
                        name: "HIDDEN"
                        PropertyChanges {
                            target: area
                            opacity: 0
                        }
                    }
                ]

                Column {

                    anchors.fill: parent

                    Rectangle {
                        id: tabMenu
                        anchors { right: parent.right; left: parent.left; }
                        height: 50
                        color: "white"

                        visible: window.containers[index].tabbed

                        property var currentTab

                        Row {
                            anchors.fill: parent

                            Repeater {
                                id: tabs
                                model: window.containers[index].tabs.length

                                property var currentContainer: window.containers[index]

                                Rectangle {
                                    id: tab
                                    anchors { top: parent.top; bottom: parent.bottom; }
                                    width: title.font.pixelSize * title.text.length + 20

                                    Behavior on anchors.topMargin {NumberAnimation{}}

                                    state: {
                                        if(tabs.currentContainer.tabs[index].active) {
                                            tabMenu.currentTab = tab
                                            "ACTIVE"
                                        } else {
                                            "NORMAL"
                                        }
                                    }

                                    states: [
                                        State {
                                            name: "NORMAL"
                                            PropertyChanges {
                                                target: tab
                                                anchors.topMargin: 10
                                            }
                                            PropertyChanges {
                                                target: strip
                                                color: "#348B6C"
                                            }
                                            PropertyChanges {
                                                target: title
                                                color: "white"
                                            }
                                            PropertyChanges {
                                                target: tab
                                                color: "#1B7E5A"
                                            }
                                        },
                                        State {
                                            name: "HOVER"
                                            PropertyChanges {
                                                target: tab
                                                anchors.topMargin: 5
                                            }
                                            PropertyChanges {
                                                target: strip
                                                color: "transparent"
                                            }
                                            PropertyChanges {
                                                target: title
                                                color: "white"
                                            }
                                            PropertyChanges {
                                                target: tab
                                                color: "#166549"
                                            }
                                        },
                                        State {
                                            name: "ACTIVE"
                                            PropertyChanges {
                                                target: tab
                                                anchors.topMargin: 5
                                            }
                                            PropertyChanges {
                                                target: strip
                                                color: "transparent"
                                            }
                                            PropertyChanges {
                                                target: title
                                                color: "#232323"
                                            }
                                            PropertyChanges {
                                                target: tab
                                                color: "#EBE7DE"
                                            }
                                        }
                                    ]

                                    Rectangle {
                                        id: strip
                                        anchors { top: parent.top; bottom: parent.bottom; left: parent.left }
                                        width: 1
                                    }

                                    Text {
                                        id: title
                                        text: tabs.currentContainer.tabs[index].title
                                        font.capitalization: Font.AllUppercase
                                        anchors.centerIn: parent
                                        font.pixelSize: 14
                                        font.family: window.font.name
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true

                                        onEntered: {
                                            if(tabMenu.currentTab != parent)
                                                parent.state = "HOVER"
                                        }
                                        onExited: {
                                            if(tabMenu.currentTab != parent)
                                                parent.state = "NORMAL"
                                        }
                                        onPressed: {
                                            if(tabMenu.currentTab != parent) {
                                                tabMenu.currentTab.state = "NORMAL"
                                                tabArea.currentArea.state = "HIDDEN"
                                                tabArea.currentArea.visible = false
                                                parent.state = "ACTIVE"
                                                tabMenu.currentTab = parent
                                                var container = tabAreas.itemAt(index)
                                                container.visible = true
                                                container.state = "NORMAL"
                                                tabArea.currentArea = container
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: tabArea
                        color: "#EBE7DE"
                        anchors { left: parent.left; right: parent.right }
                        height: window.containers[index].tabbed ? parent.height - 50 : parent.height
                        visible: window.containers[index].tabbed

                        property var currentArea

                        Repeater {
                            id: tabAreas
                            model: window.containers[index].tabs.length
                            property var currentContainer: window.containers[index]

                            Rectangle {
                                id: container
                                anchors.fill: parent
                                color: "#EBE7DE"

                                state: {
                                    if(tabAreas.currentContainer.tabs[index].active) {
                                        tabArea.currentArea = container
                                        container.visible = true
                                        "NORMAL"
                                    } else {
                                        container.visible = false
                                        "HIDDEN"
                                    }
                                }

                                Behavior on opacity {PropertyAnimation{}}

                                states: [
                                    State {
                                        name: "NORMAL"
                                        PropertyChanges {
                                            target: container
                                            opacity: 1
                                        }
                                    },
                                    State {
                                        name: "HIDDEN"
                                        PropertyChanges {
                                            target: container
                                            opacity: 0
                                        }
                                    }
                                ]
                                Item {
                                    anchors { left: parent.left; top: parent.top; right: parent.right; bottom: parent.bottom; margins: 10 }
                                    children: tabAreas.currentContainer.tabs[index].children
                                }
                            }
                        }
                    }

                    Rectangle {
                        color: "#EBE7DE"
                        anchors { left: parent.left; right: parent.right }
                        height: parent.height
                        visible: !window.containers[index].tabbed

                        Item {
                            anchors { left: parent.left; top: parent.top; right: parent.right; bottom: parent.bottom; margins: 10 }
                            children: window.containers[index].children
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: timer
        height: 30
        anchors { top: parent.top; left: parent.left; right: parent.right }
        color: "#1B7E5A"

        property string message

        function show(message) {
            if(timer.state == "NORMAL") {
                timerMsg.text = message
                timerT.restart()
                return;
            }

            timer.message = message
            timerBorder.visible = true
            timer.state = "NORMAL"
        }

        function hide() {
            timerMsg.text = ""
            timerBorder.visible = false
            timer.state = "HIDDEN"
        }

        Timer {
            id: timerT
             interval: 5000
             running: false
             repeat: false
             onTriggered: {
                 timer.hide()
             }
         }

        state: "HIDDEN"

        states: [
            State {
                name: "NORMAL"
                PropertyChanges {
                    target: timer
                    height: 30
                }
                onCompleted: {
                    timerMsg.text = timer.message
                    timerT.start()
                }
            },
            State {
                name: "HIDDEN"
                PropertyChanges {
                    target: timer
                    height: 0
                }
            }
        ]

        transitions: [
            Transition {
                from: "NORMAL"
                to: "HIDDEN"
                NumberAnimation { target: timer; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
            },
            Transition {
                from: "HIDDEN"
                to: "NORMAL"
                NumberAnimation { target: timer; property: "height"; duration: 200; easing.type: Easing.InOutQuad }
            }
        ]

        Text {
            id: timerMsg
            anchors.centerIn: parent
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.family: "PT Sans"
            font.pixelSize: 12
            color: "white"
        }

        Rectangle {
            id: timerBorder
            color: "#166549"
            height: 1
            visible: false
            anchors { top: timer.bottom; left: parent.left; right: parent.right }
        }
    }

    Item {
        id: dialogArea
        anchors.fill: parent
        visible: false
        children: window.dialogs
    }

    Rectangle {
        id: cover
        anchors.fill: parent
        color: "#EE232323"

        state: "HIDDEN"

        function close() {
            cover.state = "HIDDEN"
        }

        function show() {
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
            id: confirm
            width: 300
            height: 120
            color: "white"
            anchors.centerIn: parent

            Item {
                id: confirm_box
                height: 70
                anchors { top: parent.top; left: parent.left; right: parent.right }

                Text {
                    id: msg
                    anchors.centerIn: parent
                    width: parent.width - 30
                    wrapMode: Text.WordWrap
                    font.family: "PT Sans"
                    font.pixelSize: 12
                    color: "#232323"
                }
            }

            Row {
                anchors { top: confirm_box.bottom; left: parent.left; right: parent.right; bottom: parent.bottom; margins: 10 }
                height: 40
                spacing: 5
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        cover.close()
                    }
                }
                Button {
                    text: qsTr("OK")
                    onClicked: {
                        cover.close()
                        window.callback()
                    }
                }
            }
        }
    }
}
