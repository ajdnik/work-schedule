#ifndef SCHEDULEROW_H
#define SCHEDULEROW_H

#include <QList>
#include <QString>
#include <QObject>
#include <QStringList>

class Entry;

class ScheduleRow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString color READ color NOTIFY colorChanged)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString bonus READ bonus NOTIFY bonusChanged)
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(int workerCount READ workerCount NOTIFY workerCountChanged)
    Q_PROPERTY(bool nextTwin READ nextTwin NOTIFY nextTwinChanged)
    Q_PROPERTY(bool previousTwin READ previousTwin NOTIFY previousTwinChanged)
    Q_PROPERTY(QStringList workers READ workers NOTIFY workersChanged)
private:
    Entry *oWorkers;
    QString sColor;
    QStringList oNames;
    QString sBonus;
    bool bNextTwin;
    bool bPreviousTwin;
    int iDuration;
    int iWorkerCount;
public:
    ScheduleRow(int duration, QObject *parent = 0);
    ScheduleRow(int duration, Entry *entry, const QString &color = "transparent", bool nextTwin = false, bool previousTwin = false, QObject *parent = 0);
    QString color() const;
    bool nextTwin() const;
    int id() const;
    bool isStartEndEqual() const;
    bool previousTwin() const;
    QStringList workers() const;
    QString bonus() const;
    int duration() const;
    int workerCount() const;
    void addEntry(Entry *entry);
signals:
    void idChanged();
    void colorChanged();
    void nextTwinChanged();
    void previousTwinChanged();
    void workersChanged();
    void durationChanged();
    void workerCountChanged();
    void bonusChanged();
};

#endif // SCHEDULEROW_H
