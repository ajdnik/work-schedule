#include <QGuiApplication>
#include <QQuickView>
#include <QScreen>
#include <QQmlContext>
#include <QDir>
#include <QStandardPaths>
#include <QVariant>
#include "sys/databasemanager.h"
#include "sys/workerstore.h"
#include "sys/eventtypestore.h"
#include "sys/languagestore.h"
#include "sys/workereventstore.h"
#include "sys/holidaystore.h"
#include "sys/schedulestore.h"
#include "sys/yearlystatistics.h"
#include "sys/monthlystatistics.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    Error error(&app);

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    DatabaseManager manager(path + QDir::separator() + "appdata.db");
    QObject::connect(&app, SIGNAL(aboutToQuit()), &manager, SLOT(close()));

    WorkerStore workers(&manager, &error);
    EventTypeStore eventTypes(&manager, &error);
    LanguageStore languageTypes(&manager, &error, &app);
    WorkerEventStore events(&workers, &eventTypes, &manager, &error);
    workers.setWorkerEventStore(&events);
    HolidayStore holidays(&manager, &error);
    ScheduleStore schedules(&workers, &holidays, &manager, &error);
    workers.setScheduleStore(&schedules);
    YearlyStatistics ystat(&workers);
    QObject::connect(&schedules, SIGNAL(columnsChanged()), &ystat, SLOT(refreshValues()));
    QObject::connect(&workers, SIGNAL(workerChanged()), &ystat, SLOT(refreshWorkers()));
    MonthlyStatistics mstat(&workers);
    QObject::connect(&schedules, SIGNAL(columnsChanged()), &mstat, SLOT(refreshValues()));
    QObject::connect(&workers, SIGNAL(workerChanged()), &mstat, SLOT(refreshWorkers()));
    QQuickView view;

    QQmlContext* context = view.rootContext();
    context->setContextProperty("workers", &workers);
    context->setContextProperty("eventTypes", &eventTypes);
    context->setContextProperty("languageTypes", &languageTypes);
    context->setContextProperty("events", &events);
    context->setContextProperty("holidays", &holidays);
    context->setContextProperty("schedules", &schedules);
    context->setContextProperty("ystat", &ystat);
    context->setContextProperty("mstat", &mstat);

    view.setSource(QUrl("qrc:/application.qml"));
    view.setTitle(QObject::tr("Work schedule"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setMinimumHeight(768);
    view.setMinimumWidth(1024);

    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize = QGuiApplication::primaryScreen()->size();
    screenWidth = windowSize.width();
    screenHeight = windowSize.height();
    width = view.width();
    height = view.height();
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;

    view.setX(x);
    view.setY(y);

    view.showMaximized();

    return app.exec();
}
