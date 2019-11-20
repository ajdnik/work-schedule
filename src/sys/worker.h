#ifndef WORKER_H
#define WORKER_H

#include <QString>
#include <QList>
#include <QHash>
#include "workerevent.h"
#include "errorhandler.h"

class DatabaseManager;

class Worker : public ErrorHandler
{
private:
    QString sName;
    int iId;
    bool bCanSelect;
    QList<WorkerEvent> aWorkerEvents;
    DatabaseManager *oDatabase;

public:
    Worker(int id, const QString &name, Error *error, DatabaseManager *database);
    Error *setId(int id);
    Error *setName(const QString &name);
    int id() const;
    QString name() const;
    int eventsCount() const;
    Error *addEvent(const WorkerEvent &event);
    Error *removeEvent(int index);
    WorkerEvent *event(int index);
    bool canSelect(const QDateTime &start, const QDateTime &end) const;
    bool canSelectForShift(const QDateTime &start, const QDateTime &end) const;
    int getLoad(int year);
    int getHours(int year);
    int getBonus(int year);
    int getLoad(int year, int month);
    int getHours(int year, int month);
    int getBonus(int year, int month);
    void setSelect(bool can);
    void setLoad(int load, int year, int month);
    void setHours(int hours, int year, int month);
    void setBonus(int bonus, int year, int month);
};

#endif // WORKER_H
