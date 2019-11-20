import QtQuick 2.0
import QtQuick.Dialogs 1.0
import "qrc:/qml/"

Window {
    id: window
    containers: [
        Container {
            icon: window.icons.calendar
            active: true
            Column {
                anchors.fill: parent
                FileDialog {
                    id: fileDialog
                    title: qsTr("Please choose a file")
                    selectExisting: false
                    selectFolder: false
                    selectMultiple: false
                    visible: false
                    nameFilters: [ qsTr("Stylesheet (*.xlsx)"), qsTr("All files (*)") ]
                    modality: Qt.ApplicationModal
                    onAccepted: {
                        schedules.exportSchedule(Qt.resolvedUrl( fileDialog.fileUrl ).toString());
                        Qt.quit()
                    }
                    onRejected: {
                        Qt.quit()
                    }
                }
                Row {
                    width: 300
                    height: 40
                    spacing: 5
                    anchors.horizontalCenter: parent.horizontalCenter
                    IconButton {
                        icon: icons.save
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            if(!schedules.canGenerateSchedule())
                                fileDialog.visible = true
                            else
                                window.timerBox(qsTr("You need to generate a schedule first."), 5000)
                        }
                    }
                    IconButton {
                        icon: icons.calendar
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            if(schedules.canGenerateSchedule())
                                maxHours.show()
                            else
                                window.confirmBox(qsTr("Schedule generated. Are you sure you want to override it?"), function() {
                                    maxHours.show()
                                })
                        }
                    }
                    IconButton {
                        icon: icons.arrowl
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            if(schedule_months.value === 1) {
                                schedule_months.update(12)
                                schedule_years.update(schedule_years.value - 1)
                            } else {
                                schedule_months.update(schedule_months.value - 1)
                            }
                            schedules.setDate(schedule_years.value, schedule_months.value)
                        }
                    }
                    NumericalUpDown {
                        id: schedule_months
                        min: 1
                        max: 12
                        value: new Date().getMonth() + 1
                        anchors.verticalCenter: parent.verticalCenter
                        onChanged: {
                            schedules.setDate(schedule_years.value, schedule_months.value)
                        }
                    }
                    NumericalUpDown {
                        id: schedule_years
                        min: 1900
                        max: 3000
                        value: new Date().getFullYear()
                        anchors.verticalCenter: parent.verticalCenter
                        onChanged: {
                            schedules.setDate(schedule_years.value, schedule_months.value)
                        }
                    }
                    IconButton {
                        icon: icons.arrowr
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            if(schedule_months.value === 12) {
                                schedule_months.update(1)
                                schedule_years.update(schedule_years.value + 1)
                            } else {
                                schedule_months.update(schedule_months.value + 1)
                            }
                            schedules.setDate(schedule_years.value, schedule_months.value)
                        }
                    }
                    IconButton {
                        icon: icons.user
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            if(scheduleTable.entryId) {
                                changeWorker.show()
                            } else {
                                window.timerBox(qsTr("You must select a worker from the schedule to change."), 5000)
                            }
                        }
                    }
                    IconButton {
                        icon: icons.note
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            if(!schedules.canGenerateSchedule()) {
                                changeNote_note.text = schedules.note
                                changeNote.show()
                            } else {
                                window.timerBox(qsTr("You need to generate a schedule first."), 5000)
                            }
                        }
                    }
                }
                Schedule {
                    id: scheduleTable
                    width: parent.width
                    height: parent.height - 40
                }
            }
        },
        Container {
            icon: window.icons.users
            Row {
                anchors.fill: parent
                spacing: 10
                Column {
                    width: 200
                    height: parent.height
                    spacing: 10
                    Row {
                        width: 200
                        height: 30
                        spacing: 5

                        IconButton {
                            icon: icons.add
                            onClicked: addWorker.show()
                        }
                        IconButton {
                            icon: icons.edit
                            onClicked: {
                                if(workersTable.isSelected()) {
                                    editWorker.itemId = workersTable.currentId
                                    editWorker_name.text = workersTable.currentModel["name"]
                                    editWorker.show()
                                } else {
                                    window.timerBox(qsTr("You must select a worker from the table to edit."), 5000)
                                }
                            }
                        }
                        IconButton {
                            icon: icons.remove
                            onClicked: {
                                if(workersTable.isSelected()) {
                                    window.confirmBox(qsTr("Are you sure you want to delete this worker? Deleting it will also delete its events."), function() {
                                        var error = workers.deleteWorker(workersTable.currentId)
                                        if(error.isError()) {
                                             window.timerBox(qsTr("ERROR: ") + error.error, 10000)
                                        } else {
                                            workersTable.currentIndex = -1
                                        }
                                    })
                                } else {
                                    window.timerBox(qsTr("You must select a worker from the table to delete."), 5000)
                                }
                            }
                        }
                    }
                    TableView {
                        id: workersTable
                        model: workers
                        width: 200
                        height: parent.height - 40
                        currentIndex: -1
                        columns: [
                            TableColumn {
                                name: qsTr("Name")
                                source: "name"
                            }
                        ]
                        onSelectionChanged: {
                            eventsTable.currentIndex = -1
                            if(workersTable.currentId >= 0)
                                events.setWorkerId(workersTable.currentId)
                            else
                                events.setWorkerId(events.Empty)
                        }
                    }
                }
                Column {
                    width: parent.width - 210
                    height: parent.height
                    spacing: 10
                    Row {
                        width: parent.width
                        height: 30
                        spacing: 5
                        layoutDirection: Qt.RightToLeft

                        IconButton {
                            icon: icons.remove
                            onClicked: {
                                if(!workersTable.isSelected()) {
                                    window.timerBox(qsTr("You must select a worker from the table to delete its event."), 5000)
                                } else if(!shiftsTable.isSelected()) {
                                    window.timerBox(qsTr("You must select an event from the table to delete it."), 5000)
                                } else {
                                    window.confirmBox(qsTr("Are you sure you want to delete this event?"), function() {
                                        var error = events.deleteEvent(eventsTable.currentId)
                                        if(error.isError()) {
                                             window.timerBox(qsTr("ERROR: ") + error.error, 10000)
                                        } else {
                                            eventsTable.currentIndex = -1
                                        }
                                    })
                                }
                            }
                        }
                        IconButton {
                            icon: icons.edit
                            onClicked: {
                                if(!workersTable.isSelected()) {
                                    window.timerBox(qsTr("You must select a worker from the table to edit its event."), 5000)
                                } else if(!eventsTable.isSelected()) {
                                    window.timerBox(qsTr("You must select an event from the table to edit it."), 5000)
                                } else {
                                    editWorkerEvent.itemId = eventsTable.currentId
                                    editWorkerEvent_type.set(eventsTable.currentModel["event_type"], eventsTable.currentModel["type_id"])
                                    var startSplit = eventsTable.currentModel["start"].split(' ')
                                    var endSplit = eventsTable.currentModel["end"].split(' ')
                                    editWorkerEvent_startDate.setDate(startSplit[0])
                                    editWorkerEvent_endDate.setDate(endSplit[0])
                                    editWorkerEvent.show()
                                }
                            }
                        }
                        IconButton {
                            icon: icons.add
                            onClicked: {
                                if(workersTable.isSelected()) {
                                    addWorkerEvent.show()
                                } else {
                                    window.timerBox(qsTr("You must select a worker from the table to add new event."), 5000)
                                }
                            }
                        }
                        CheckBox {
                            id: showExpired
                            text: qsTr("Show expired?")
                            anchors.top: parent.top
                            anchors.topMargin: 3
                            onCheckChanged: {
                                if(workersTable.isSelected()) {
                                    var error = events.setShowExpired(showExpired.checkState)
                                    if(error.isError()) {
                                         window.timerBox(qsTr("ERROR: ") + error.error, 10000)
                                    }
                                } else {
                                    showExpired.uncheck()
                                    window.timerBox(qsTr("You must select a worker from the table."), 5000)
                                }
                            }
                        }
                    }
                    TableView {
                        id: eventsTable
                        model: events
                        width: parent.width
                        height: parent.height - 40
                        currentIndex: -1
                        columns: [
                            TableColumn {
                                name: qsTr("Event type")
                                source: "event_type"
                            },
                            TableColumn {
                                name: qsTr("Start")
                                source: "start"
                            },
                            TableColumn {
                                name: qsTr("End")
                                source: "end"
                            }
                        ]
                    }
                }
            }
        },
        Container {
            icon: window.icons.cog
            tabbed: true
            tabs: [
                TabContainer {
                    title: qsTr("Holidays")
                    active: true
                    Column {
                        anchors.fill: parent
                        spacing: 5
                        Row {
                            width: parent.width
                            height: 30
                            spacing: 5
                            IconButton {
                                icon: icons.add
                                onClicked: addHoliday.show()
                            }
                            IconButton {
                                icon: icons.edit
                                onClicked: {
                                    if(holidaysTable.isSelected()) {
                                        editHoliday.itemId = holidaysTable.currentId
                                        editHoliday_name.text = holidaysTable.currentModel["name"]
                                        editHoliday_startDate.setDate(holidaysTable.currentModel["start"])
                                        editHoliday_endDate.setDate(holidaysTable.currentModel["end"])
                                        editHoliday_anual.checkState = holidaysTable.currentModel["anual"]
                                        editHoliday.show()
                                    } else {
                                        window.timerBox(qsTr("You must select a holiday from the table to edit."), 5000)
                                    }
                                }
                            }
                            IconButton {
                                icon: icons.remove
                                onClicked: {
                                    if(holidaysTable.isSelected()) {
                                        window.confirmBox(qsTr("Are you sure you want to delete this holiday?"), function() {
                                            var error = holidays.deleteHoliday(holidaysTable.currentId)
                                            if(error.isError()) {
                                                 window.timerBox(qsTr("ERROR: ") + error.error, 10000)
                                            } else {
                                                holidaysTable.currentIndex = -1
                                            }
                                        })
                                    } else {
                                        window.timerBox(qsTr("You must select a holiday from the table to delete."), 5000)
                                    }
                                }
                            }
                        }
                        TableView {
                            id: holidaysTable
                            model: holidays
                            width: parent.width
                            height: parent.height - 35
                            currentIndex: -1
                            columns: [
                                TableColumn {
                                    name: qsTr("Name")
                                    source: "name"
                                },
                                TableColumn {
                                    name: qsTr("Start")
                                    source: "start"
                                },
                                TableColumn {
                                    name: qsTr("End")
                                    source: "end"
                                },
                                TableColumn {
                                    name: qsTr("Anual")
                                    source: "anual_word"
                                }
                            ]
                        }
                    }
                },
                TabContainer {
                    title: qsTr("Yearly Statistics")
                    Column {
                        anchors.fill: parent
                        Row {
                            width: 300
                            height: 40
                            spacing: 5
                            anchors.horizontalCenter: parent.horizontalCenter

                            IconButton {
                                icon: icons.arrowl
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {
                                    ystatistics_years.update(ystatistics_years.value - 1)
                                    ystat.setYear(ystatistics_years.value)
                                }
                            }
                            NumericalUpDown {
                                id: ystatistics_years
                                min: 1900
                                max: 3000
                                value: new Date().getFullYear()
                                anchors.verticalCenter: parent.verticalCenter
                                onChanged: {
                                    ystat.setYear(ystatistics_years.value)
                                }
                            }
                            IconButton {
                                icon: icons.arrowr
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {
                                    ystatistics_years.update(ystatistics_years.value + 1)
                                    ystat.setYear(ystatistics_years.value)
                                }
                            }
                            Item {
                                height: parent.height
                                width: 40
                            }
                            CheckBox {
                                id: yshowLoad
                                text: qsTr("Load")
                                checkState: true
                                anchors.verticalCenter: parent.verticalCenter
                                onCheckChanged: {
                                    ystat.setLoad(yshowLoad.checkState)
                                }
                            }
                            CheckBox {
                                id: yshowBonus
                                text: qsTr("Bonus")
                                checkState: true
                                anchors.verticalCenter: parent.verticalCenter
                                onCheckChanged: {
                                    ystat.setBonus(yshowBonus.checkState)
                                }
                            }
                            CheckBox {
                                id: yshowHours
                                text: qsTr("Events")
                                checkState: true
                                anchors.verticalCenter: parent.verticalCenter
                                onCheckChanged: {
                                    ystat.setHours(yshowHours.checkState)
                                }
                            }
                        }
                        Item {
                            width: parent.width
                            height: 10
                        }

                        Statistics {
                            id: yearlyTable
                            source: ystat
                            width: parent.width
                            height: parent.height - 50
                        }
                    }
                },
                TabContainer {
                    title: qsTr("Monthly Statistics")
                    Column {
                        anchors.fill: parent
                        Row {
                            width: 300
                            height: 40
                            spacing: 5
                            anchors.horizontalCenter: parent.horizontalCenter
                            IconButton {
                                icon: icons.arrowl
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {
                                    if(statistics_months.value === 1) {
                                        statistics_months.update(12)
                                        statistics_years.update(statistics_years.value - 1)
                                    } else {
                                        statistics_months.update(statistics_months.value - 1)
                                    }
                                    mstat.setDate(statistics_years.value, statistics_months.value)
                                }
                            }
                            NumericalUpDown {
                                id: statistics_months
                                min: 1
                                max: 12
                                value: new Date().getMonth() + 1
                                anchors.verticalCenter: parent.verticalCenter
                                onChanged: {
                                    mstat.setDate(statistics_years.value, statistics_months.value)
                                }
                            }
                            NumericalUpDown {
                                id: statistics_years
                                min: 1900
                                max: 3000
                                value: new Date().getFullYear()
                                anchors.verticalCenter: parent.verticalCenter
                                onChanged: {
                                    mstat.setDate(statistics_years.value, statistics_months.value)
                                }
                            }
                            IconButton {
                                icon: icons.arrowr
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: {
                                    if(statistics_months.value === 12) {
                                        statistics_months.update(1)
                                        statistics_years.update(statistics_years.value + 1)
                                    } else {
                                        statistics_months.update(statistics_months.value + 1)
                                    }
                                    mstat.setDate(statistics_years.value, statistics_months.value)
                                }
                            }
                            Item {
                                height: parent.height
                                width: 40
                            }
                            CheckBox {
                                id: showLoad
                                text: qsTr("Load")
                                checkState: true
                                anchors.verticalCenter: parent.verticalCenter
                                onCheckChanged: {
                                    mstat.setLoad(showLoad.checkState)
                                }
                            }
                            CheckBox {
                                id: showBonus
                                text: qsTr("Bonus")
                                checkState: true
                                anchors.verticalCenter: parent.verticalCenter
                                onCheckChanged: {
                                    mstat.setBonus(showBonus.checkState)
                                }
                            }
                            CheckBox {
                                id: showHours
                                text: qsTr("Events")
                                checkState: true
                                anchors.verticalCenter: parent.verticalCenter
                                onCheckChanged: {
                                    mstat.setHours(showHours.checkState)
                                }
                            }
                        }
                        Item {
                            width: parent.width
                            height: 10
                        }
                        Statistics {
                            id: monthlyTable
                            source: mstat
                            width: parent.width
                            height: parent.height - 50
                        }
                    }
                },
                TabContainer {
                    title: qsTr("Settings")
                    Column {
                        anchors.fill: parent
                        anchors.leftMargin: 10
                        anchors.topMargin: 5
                        Label { text: qsTr("Language:") }
                        ComboBox {
                            id: selectLanguage_type
                            model: languageTypes
                            modelProperty: "name"
                            displayedElements: 2
                            onChanged: {
                                var error = languageTypes.updateCurrentId(selectLanguage_type.currentId)
                                if(error.isError()) {
                                    window.timerBox(qsTr("ERROR: ") + error.error, 10000)
                                } else {
                                    window.timerBox(qsTr("Language preferences updated. Restart application for changes to take effect."), 5000)
                                }
                            }
                            onModelChanged: {
                                var id = languageTypes.getCurrentId()
                                selectLanguage_type.set(languageTypes.getName(id), id)
                            }
                        }
                    }
                }

            ]
        }
    ]
    dialogs: [
        Dialog {
            id: addHoliday
            dialogTitle: qsTr("Add holiday")
            dialogHeight: 220
            dialogWidth: 270

            function reset() {
                addHoliday_name.text = ""
                addHoliday_startDate.reset()
                addHoliday_endDate.reset()
                addHoliday_anual.checkState = false
            }

            function unfocus() {
                addHoliday_name.focus = false
                addHoliday_startDate.unfocus()
                addHoliday_endDate.unfocus()
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Name:") }
                Input {
                    id: addHoliday_name
                    width: 200
                }
                Label { text: qsTr("Start:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: addHoliday_startDate }
                }
                Label { text: qsTr("End:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: addHoliday_endDate }
                }
                CheckBox {
                    id: addHoliday_anual
                    text: qsTr("Is anual?")
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        addHoliday.close()
                        addHoliday.unfocus()
                        addHoliday.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        addHoliday.unfocus()
                        var error = holidays.insertHoliday(addHoliday_name.text, addHoliday_startDate.getDate(), addHoliday_endDate.getDate(), addHoliday_anual.checkState)
                        if(error.isError()) {
                            parent.info.show(error.error)
                        } else {
                            addHoliday.close()
                            addHoliday.reset()
                        }
                    }
                }
            ]
        },
        Dialog {
            id: editHoliday
            dialogTitle: qsTr("Edit holiday")
            dialogHeight: 220
            dialogWidth: 270

            property var itemId

            function reset() {
                editHoliday_name.text = ""
                editHoliday_startDate.reset()
                editHoliday_endDate.reset()
                editHoliday_anual.checkState = false
            }

            function unfocus() {
                editHoliday_name.focus = false
                editHoliday_startDate.unfocus()
                editHoliday_endDate.unfocus()
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Name:") }
                Input {
                    id: editHoliday_name
                    width: 200
                }
                Label { text: qsTr("Start:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: editHoliday_startDate }
                }
                Label { text: qsTr("End:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: editHoliday_endDate }
                }
                CheckBox {
                    id: editHoliday_anual
                    text: qsTr("Is anual?")
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        editHoliday.close()
                        editHoliday.unfocus()
                        editHoliday.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        editHoliday.unfocus()
                        var error = holidays.updateHoliday(editHoliday.itemId, editHoliday_name.text, editHoliday_startDate.getDate(), editHoliday_endDate.getDate(), editHoliday_anual.checkState)
                        if(error.isError()) {
                            parent.info.show(error.error)
                        } else {
                            editHoliday.close()
                            editHoliday.reset()
                        }
                    }
                }
            ]
        },
        Dialog {
            id: addWorker
            dialogTitle: qsTr("Add worker")
            dialogHeight: 70
            dialogWidth: 280

            function reset() {
                addWorker_name.text = ""
            }

            function unfocus() {
                addWorker_name.focus = false
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Name:") }
                Input {
                    id: addWorker_name
                    width: 200
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        addWorker.close()
                        addWorker.unfocus()
                        addWorker.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        addWorker.unfocus()
                        var error = workers.insertWorker(addWorker_name.text)
                        if(error.isError()) {
                            parent.info.show(error.error)
                        } else {
                            addWorker.close()
                            addWorker.reset()
                        }
                    }
                }
            ]
        },
        Dialog {
            id: editWorker
            dialogTitle: qsTr("Edit worker")
            dialogHeight: 70
            dialogWidth: 280

            property var itemId

            function reset() {
                editWorker_name.text = ""
            }

            function unfocus() {
                editWorker_name.focus = false
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Name:") }
                Input {
                    id: editWorker_name
                    width: 200
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        editWorker.close()
                        editWorker.unfocus()
                        editWorker.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        editWorker.unfocus()
                        var error = workers.updateWorker(editWorker.itemId, editWorker_name.text)
                        if(error.isError()) {
                            parent.info.show(error.error)
                        } else {
                            editWorker.close()
                            editWorker.reset()
                        }
                    }
                }
            ]
        },
        Dialog {
            id: addWorkerEvent
            dialogTitle: qsTr("Add event")
            dialogHeight: 200
            dialogWidth: 270

            function reset() {
                addWorkerEvent_type.reset()
                addWorkerEvent_startDate.reset()
                addWorkerEvent_endDate.reset()
            }

            function unfocus() {
                addWorkerEvent_startDate.unfocus()
                addWorkerEvent_endDate.unfocus()
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Type:") }
                ComboBox {
                    id: addWorkerEvent_type
                    model: eventTypes
                    modelProperty: "name"
                    displayedElements: 3
                }
                Label { text: qsTr("Start:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: addWorkerEvent_startDate }
                }
                Label { text: qsTr("End:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: addWorkerEvent_endDate }
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        addWorkerEvent.close()
                        addWorkerEvent.unfocus()
                        addWorkerEvent.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        if(addWorkerEvent_type.currentId > -1) {
                            addWorkerEvent.unfocus()
                            var error = events.insertEvent(addWorkerEvent_type.currentId, addWorkerEvent_startDate.getDate() + " 00:00:00", addWorkerEvent_endDate.getDate() + " 23:59:59")
                            if(error.isError()) {
                                parent.info.show(error.error)
                            } else {
                                addWorkerEvent.close()
                                addWorkerEvent.reset()
                            }
                        } else {
                            parent.info.show(qsTr("You must select an event type."))
                        }
                    }
                }
            ]
        },
        Dialog {
            id: editWorkerEvent
            dialogTitle: qsTr("Edit event")
            dialogHeight: 200
            dialogWidth: 270

            property var itemId

            function reset() {
                editWorkerEvent_type.reset()
                editWorkerEvent_startDate.reset()
                editWorkerEvent_endDate.reset()
            }

            function unfocus() {
                editWorkerEvent_startDate.unfocus()
                editWorkerEvent_endDate.unfocus()
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Type:") }
                ComboBox {
                    id: editWorkerEvent_type
                    model: eventTypes
                    modelProperty: "name"
                    displayedElements: 3
                }
                Label { text: qsTr("Start:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: editWorkerEvent_startDate }
                }
                Label { text: qsTr("End:") }
                Row {
                    width: parent.width
                    height: 40
                    spacing: 10
                    Calendar { id: editWorkerEvent_endDate }
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        editWorkerEvent.close()
                        editWorkerEvent.unfocus()
                        editWorkerEvent.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        if(editWorkerEvent_type.currentId > -1) {
                            editWorkerEvent.unfocus()
                            var error = events.updateEvent(editWorkerEvent.itemId, editWorkerEvent_type.currentId, editWorkerEvent_startDate.getDate() + " 00:00:00", editWorkerEvent_endDate.getDate() + " 23:59:59")
                            if(error.isError()) {
                                parent.info.show(error.error)
                            } else {
                                editWorkerEvent.close()
                                editWorkerEvent.reset()
                            }
                        } else {
                            parent.info.show(qsTr("You must select an event type."))
                        }
                    }
                }
            ]
        },
        Dialog {
            id: maxHours
            dialogTitle: qsTr("Maximum work hours")
            dialogHeight: 70
            dialogWidth: 260

            function reset() {
                maxHours_hours.text = ""
            }

            function unfocus() {
                maxHours_hours.focus = false
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Hours:") }
                Input {
                    id: maxHours_hours
                    width: 200
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        maxHours.close()
                        maxHours.unfocus()
                        maxHours.reset()
                    }
                },
                Button {
                    text: qsTr("Schedule")
                    onClicked: {
                        maxHours.unfocus()
                        var hours = parseInt(maxHours_hours.text, 10)
                        if(isNaN(hours)) {
                            parent.info.show(qsTr("Invalid entry. Must be an integer."))
                        } else {
                            var error = schedules.generateSchedule(schedule_years.value, schedule_months.value, hours)
                            if(error.isError()) {
                                 parent.info.show(error.error)
                            } else {
                                parent.info.hide()
                                maxHours.close()
                                maxHours.reset()
                            }
                        }
                    }
                }
            ]
        },
        Dialog {
            id: changeWorker
            dialogTitle: qsTr("Change worker")
            dialogHeight: 200
            dialogWidth: 280

            function reset() {
                changeWorker_select.currentIndex = -1
            }

            function unfocus() {
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                TableView {
                    id: changeWorker_select
                    model: workers
                    width: 200
                    height: 195
                    currentIndex: -1
                    columns: [
                        TableColumn {
                            name: qsTr("Name")
                            source: "name"
                        }
                    ]
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        changeWorker.close()
                        changeWorker.unfocus()
                        changeWorker.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                        if(changeWorker_select.currentIndex > -1) {
                            changeWorker.unfocus()
                            var error = schedules.canChangeWorker(scheduleTable.entryId, changeWorker_select.currentId)
                            if(error.isError()) {
                                parent.info.show(error.error)
                            } else {
                                schedules.changeWorker(scheduleTable.entryId, scheduleTable.workerNumber, changeWorker_select.currentId)
                                changeWorker.close()
                                changeWorker.reset()
                            }
                        } else {
                            parent.info.show(qsTr("You must select a worker."))
                        }
                    }
                }
            ]
        },
        Dialog {
            id: changeNote
            dialogTitle: qsTr("Change note")
            dialogHeight: 70
            dialogWidth: 260

            function reset() {
                changeNote_note.text = ""
            }

            function unfocus() {
                changeNote_note.focus = false
            }

            dialog: Column {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 5
                spacing: 5
                Label { text: qsTr("Note:") }
                Input {
                    id: changeNote_note
                    width: 200
                }
            }

            buttons: [
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        changeNote.close()
                        changeNote.unfocus()
                        changeNote.reset()
                    }
                },
                Button {
                    text: qsTr("Save")
                    onClicked: {
                            changeNote.unfocus()
                            schedules.note = changeNote_note.text
                            changeNote.close()
                            changeNote.reset()
                    }
                }
            ]
        }
    ]
}
