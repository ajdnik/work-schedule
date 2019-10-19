import QtQuick 2.0

Rectangle {
    id: element
    width: 30
    height: 30

    property alias icons: icons
    property int icon: icons.add

    QtObject {
        id: icons
        property int add: 1
        property int edit: 2
        property int remove: 3
        property int arrowl: 4
        property int arrowr: 5
        property int user: 6
        property int calendar: 7
        property int note: 8
        property int save: 9
    }

    signal clicked

    state: "NORMAL"

    states: [
        State {
            name: "NORMAL"
            PropertyChanges {
                target: image
                source: {
                    if(element.icon == icons.add)
                        "qrc:/images/add_white.png"
                    else if(element.icon == icons.edit)
                        "qrc:/images/edit_white.png"
                    else if(element.icon == icons.arrowl)
                        "qrc:/images/arrowl_white.png"
                    else if(element.icon == icons.arrowr)
                        "qrc:/images/arrowr_white.png"
                    else if(element.icon == icons.user)
                        "qrc:/images/user_white.png"
                    else if(element.icon == icons.calendar)
                        "qrc:/images/calendar_white.png"
                    else if(element.icon == icons.note)
                        "qrc:/images/note_white.png"
                    else if(element.icon == icons.save)
                        "qrc:/images/save_white.png"
                    else
                        "qrc:/images/delete_white.png"
                }
            }
            PropertyChanges {
                target: element
                color: "#1B7E5A"
            }
        },
        State {
            name: "HOVER"
            PropertyChanges {
                target: image
                source: {
                    if(element.icon == icons.add)
                        "qrc:/images/add_white.png"
                    else if(element.icon == icons.edit)
                        "qrc:/images/edit_white.png"
                    else if(element.icon == icons.arrowl)
                        "qrc:/images/arrowl_white.png"
                    else if(element.icon == icons.arrowr)
                        "qrc:/images/arrowr_white.png"
                    else if(element.icon == icons.user)
                        "qrc:/images/user_white.png"
                    else if(element.icon == icons.calendar)
                        "qrc:/images/calendar_white.png"
                    else if(element.icon == icons.note)
                        "qrc:/images/note_white.png"
                    else if(element.icon == icons.save)
                        "qrc:/images/save_white.png"
                    else
                        "qrc:/images/delete_white.png"
                }
            }
            PropertyChanges {
                target: element
                color: "#166549"
            }
        },
        State {
            name: "DISABLED"
            PropertyChanges {
                target: image
                source: {
                    if(element.icon == icons.add)
                        "qrc:/images/add_light.png"
                    else if(element.icon == icons.edit)
                        "qrc:/images/edit_light.png"
                    else if(element.icon == icons.arrowl)
                        "qrc:/images/arrowl_light.png"
                    else if(element.icon == icons.arrowr)
                        "qrc:/images/arrowr_light.png"
                    else if(element.icon == icons.user)
                        "qrc:/images/user_light.png"
                    else if(element.icon == icons.calendar)
                        "qrc:/images/calendar_light.png"
                    else if(element.icon == icons.note)
                        "qrc:/images/note_light.png"
                    else if(element.icon == icons.save)
                        "qrc:/images/save_light.png"
                    else
                        "qrc:/images/delete_light.png"
                }
            }
            PropertyChanges {
                target: element
                color: "#DDD8CD"
            }
        }

    ]

    Image {
        id: image
        anchors.centerIn: parent
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
