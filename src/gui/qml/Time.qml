import QtQuick 2.0

Rectangle {
    id: root
    height: 40
    width: hour.width + minute.width + 1
    color: "#232323"

    function getTime() {
        var time = "";
        if(hour.value < 10)
            time += "0";
        time += hour.value + ":";
        if(minute.value < 10)
            time += "0";
        time += minute.value + ":00";
        return time;
    }

    function setTime(timeString) {
        var parts = timeString.split(':');
        hour.update(parts[0]);
        minute.update(parts[1]);
    }

    function unfocus() {
        hour.unfocus()
        minute.unfocus()
    }

    function reset() {
        hour.update(0);
        minute.update(0);
    }

    Row {
        height: 32
        width: parent.width
        anchors.centerIn: parent
        spacing: 0
        NumericalUpDown {
            id: hour
            min: 0
            max: 23
            value: 0
        }
        Rectangle {
            width: 1
            height: parent.height
            color: "#7B7B7B"
        }
        NumericalUpDown {
            id: minute
            min: 0
            max: 59
            value: 0
        }
    }
}
