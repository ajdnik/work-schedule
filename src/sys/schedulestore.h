#ifndef SCHEDULESTORE_H
#define SCHEDULESTORE_H

#include <QDate>
#include <QStringList>
#include <QList>
#include "errorhandler.h"

class HolidayStore;
class DatabaseManager;
class Entry;
class WorkerStore;
class Worker;

class ScheduleStore : public QObject, public ErrorHandler
{
    Q_OBJECT
    Q_PROPERTY(QString monthName READ monthName NOTIFY monthNameChanged)
    Q_PROPERTY(int columnCount READ columnCount NOTIFY columnCountChanged)
    Q_PROPERTY(QList<QObject *> columns READ columns NOTIFY columnsChanged)
    Q_PROPERTY(QStringList dayNumbers READ dayNumbers NOTIFY dayNumbersChanged)
    Q_PROPERTY(QStringList dayColors READ dayColors NOTIFY dayColorsChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
private:
    QDate oDate;
    QString sMonthName;
    QString sNote;
    QList<QObject *> aColumns;
    QList<Entry *> aSchedules;
    QStringList aDayNumbers;
    QStringList aDayColors;
    HolidayStore *oHolidays;
    DatabaseManager *oDatabase;
    WorkerStore *oWorkers;

    void populateDayNumbers(const QDate &previous, const QDate &next);
    void populateDayColors(const QDate &previous, const QDate &next);
    void populateShifts(const QDate &previous, const QDate &next);
    void populateTitle(int year, int month);
    void revertSchedule(int year, int month);
    Worker *getBestWorker(int load, int bonus, int hours, int maximumHours, const QDateTime &start, const QDateTime &end);
public:
    ScheduleStore(WorkerStore *workers, HolidayStore *holidays, DatabaseManager *manager, Error *error);
    ~ScheduleStore();
    Q_INVOKABLE void setDate(int year, int month);
    Q_INVOKABLE QObject *generateSchedule(int year, int month, int maxHours);
    Q_INVOKABLE void changeWorker(int entryId, int workerNumber, int workerId);
    Q_INVOKABLE bool canGenerateSchedule();
    Q_INVOKABLE void exportSchedule(const QString &filename);
    QString monthName() const;
    QString note() const;
    int columnCount() const;
    QList<QObject *> columns() const;
    QStringList dayNumbers() const;
    QStringList dayColors() const;
    void setMonthName(const QString &name);
    void setNote(const QString &note);
    void getNote();
    void updateCurrent();
signals:
    void monthNameChanged();
    void columnCountChanged();
    void columnsChanged();
    void dayColorsChanged();
    void dayNumbersChanged();
    void noteChanged();
    void generating();
    void generated();
};

#endif // SCHEDULESTORE_H
