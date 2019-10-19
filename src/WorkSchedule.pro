TEMPLATE += \
    app

QT += \
    qml \
    quick \
    sql

CODECFORTR = UTF-8

# Copy qml files post build
win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    PWD_WIN = $${PWD}
    PWD_WIN ~= s,/,\\,g
    QMAKE_POST_LINK += $$quote(cmd /c copy /y $${PWD_WIN}\\appdata.db $${DESTDIR_WIN}$$escape_expand(\\n\\t))
}
unix {
    QMAKE_POST_LINK += $$quote(cp $${PWD}/appdata.db $${DESTDIR}$$escape_expand(\\n\\t))
}

RC_FILE += myapp.rc

TRANSLATIONS += \
    lang/workschedule_sl.ts \
    lang/workschedule_en.ts

SOURCES += \
    main.cpp \
    sys/language.cpp \
    sys/languagestore.cpp \
    sys/worker.cpp \
    sys/databasemanager.cpp \
    sys/workerevent.cpp \
    sys/eventtype.cpp \
    sys/eventtypestore.cpp \
    sys/workerstore.cpp \
    sys/workereventstore.cpp \
    sys/holiday.cpp \
    sys/holidaystore.cpp \
    sys/error.cpp \
    sys/schedulestore.cpp \
    sys/schedulerow.cpp \
    sys/schedulecolumn.cpp \
    sys/entry.cpp \
    sys/yearlystatistics.cpp \
    sys/monthlystatistics.cpp

RESOURCES += \
    gui/gui.qrc \
    data.qrc

OTHER_FILES += \
    application.qml \
    gui/qml/Window.qml \
    gui/qml/Time.qml \
    gui/qml/TableView.qml \
    gui/qml/TableColumn.qml \
    gui/qml/TabContainer.qml \
    gui/qml/NumericalUpDown.qml \
    gui/qml/Label.qml \
    gui/qml/Input.qml \
    gui/qml/Dialog.qml \
    gui/qml/Container.qml \
    gui/qml/ComboBox.qml \
    gui/qml/CheckBox.qml \
    gui/qml/Calendar.qml \
    gui/qml/Button.qml \
    gui/qml/qmldir \
    gui/qml/IconButton.qml \
    gui/qml/BorderRectangle.qml \
    Schedule.qml \
    Statistics.qml

HEADERS += \
    sys/language.h \
    sys/languagestore.h \
    sys/worker.h \
    sys/store.h \
    sys/databasemanager.h \
    sys/workerevent.h \
    sys/eventtype.h \
    sys/eventtypestore.h \
    sys/workerstore.h \
    sys/workereventstore.h \
    sys/duration.h \
    sys/holiday.h \
    sys/holidaystore.h \
    sys/pointerstore.h \
    sys/error.h \
    sys/errorhandler.h \
    sys/schedulestore.h \
    sys/schedulerow.h \
    sys/schedulecolumn.h \
    sys/entry.h \
    sys/yearlystatistics.h \
    sys/monthlystatistics.h

include(xlsx/qtxlsx.pri)
