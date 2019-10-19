import QtQuick 2.0

Rectangle {
    id: root
    height: 40
    width: day.width + month.width + year.width + 2
    color: "#232323"

    property var date: new Date()

    Behavior on width {NumberAnimation{}}

    function showYear() {
        year.show()
        separator.visible = true
        root.width = day.width + month.width + year.width + 2
    }

    function hideYear() {
        year.hide()
        separator.visible = false
        root.width = day.width + month.width + 1
    }

    function unfocus() {
        day.unfocus()
        month.unfocus()
        year.unfocus()
    }

    function reset() {
        root.date = new Date();
        root.update();
    }

    function setDate(dateString) {
        var parts = dateString.split('-');
        root.date = new Date(parts[0], parts[1]-1, parts[2]);
        root.update();
    }

    function getDate() {
        var curr_date = root.date.getDate();
        var curr_month = root.date.getMonth() + 1;
        var curr_year = root.date.getFullYear();
        var output = curr_year + "-";
        if(curr_month < 10)
            output += "0";
        output += curr_month + "-";
        if(curr_date < 10)
            output += "0";
        output += curr_date;
        return output;
    }

    function update() {
        var m = new Date(root.date.getFullYear(), root.date.getMonth() + 1, 0)
        day.max = m.getDate()
        day.update(root.date.getDate())
        month.update(root.date.getMonth() + 1)
        year.update(root.date.getFullYear())
    }

    Row {
        height: 32
        width: parent.width
        anchors.centerIn: parent
        spacing: 0
        NumericalUpDown {
            id: day
            min: 1
            max: {
                var m = new Date(root.date.getFullYear(), root.date.getMonth(), 0)
                m.getDate()
            }
            value: root.date.getDate()
            onUp: {
                root.date.setDate(root.date.getDate() + 1)
                root.update()
            }
            onDown: {
                root.date.setDate(root.date.getDate() - 1)
                root.update()
            }
            onModified: {
                root.date.setDate(day.value)
                root.update()
            }
        }
        Rectangle {
            width: 1
            height: parent.height
            color: "#7B7B7B"
        }
        NumericalUpDown {
            id: month
            min: 1
            max: 12
            value: root.date.getMonth() + 1
            onUp: {
                root.date.setMonth(root.date.getMonth() + 1)
                root.update()
            }
            onDown: {
                root.date.setMonth(root.date.getMonth() - 1)
                root.update()
            }
            onModified: {
                root.date.setMonth(month.value - 1)
                root.update()
            }
        }
        Rectangle {
            id: separator
            width: 1
            height: parent.height
            color: "#7B7B7B"
        }
        NumericalUpDown {
            id: year
            min: 1900
            max: 3000
            value: root.date.getFullYear()
            onUp: {
                root.date.setFullYear(root.date.getFullYear() + 1)
                root.update()
            }
            onDown: {
                root.date.setFullYear(root.date.getFullYear() - 1)
                root.update()
            }
            onModified: {
                root.date.setFullYear(year.value)
                root.update()
            }
        }
    }
}
