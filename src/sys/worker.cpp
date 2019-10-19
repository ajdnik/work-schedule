#include "worker.h"
#include "databasemanager.h"
#include <QVariant>

Worker::Worker(int id, const QString &name, Error *error, DatabaseManager *database)
    : ErrorHandler(error)
{
    this->oDatabase = database;
    if(id < 0)
         this->bNull = true;
    this->iId = id;
    this->sName = name;
    this->bCanSelect = true;
}

Error *Worker::setId(int id)
{
    if(id < 0)
        return this->setError(QObject::tr("Id must be nonnegative integer."));
    this->iId = id;
    return this->setNoError();
}

Error *Worker::setName(const QString &name)
{
    if(name.trimmed().size() == 0)
        return this->setError(QObject::tr("Name can not be empty."));
    this->sName = name;
    return this->setNoError();
}

bool Worker::canSelect(const QDateTime &start, const QDateTime &end) const
{
    if(!this->bCanSelect)
        return this->bCanSelect;
    for(int i=0;i<this->aWorkerEvents.count();i++) {
        if(this->aWorkerEvents[i].isInside(start, end)) {
            return false;
        }
    }
    return this->bCanSelect;
}

int Worker::id() const
{
    return this->iId;
}

QString Worker::name() const
{
    return this->sName;
}

int Worker::eventsCount() const
{
    return this->aWorkerEvents.count();
}

Error *Worker::addEvent(const WorkerEvent &event)
{
    if(event.isNull())
        return this->setError(QObject::tr("Event is invalid."));
    this->aWorkerEvents.append(event);
    return this->setNoError();
}

Error *Worker::removeEvent(int index)
{
    if(index < 0 || index >= this->aWorkerEvents.count())
        return this->setError(QObject::tr("Index out of range."));
    this->aWorkerEvents.removeAt(index);
    return this->setNoError();
}

WorkerEvent *Worker::event(int index)
{
    return &this->aWorkerEvents[index];
}

int Worker::getLoad(int year, int month)
{
    QSqlQuery query = this->oDatabase->exec(QString("select load from scores where worker_id=%1 and year=%2 and month=%3").arg(this->id()).arg(year).arg(month));
    if(query.next()) {
        int value = query.value(0).toInt();
        return value;
    } else {
        this->oDatabase->exec(QString("insert into scores(worker_id, year, month, hours, bonus, load) values(%1,%2,%3,0,0,0)").arg(this->id()).arg(year).arg(month));
        return 0;
    }
}

int Worker::getHours(int year, int month)
{
    QSqlQuery query = this->oDatabase->exec(QString("select hours from scores where worker_id=%1 and year=%2 and month=%3").arg(this->id()).arg(year).arg(month));
    if(query.next()) {
        int value = query.value(0).toInt();
        return value;
    } else {
        this->oDatabase->exec(QString("insert into scores(worker_id, year, month, hours, bonus, load) values(%1,%2,%3,0,0,0)").arg(this->id()).arg(year).arg(month));
        return 0;
    }
}

int Worker::getBonus(int year, int month)
{
    QSqlQuery query = this->oDatabase->exec(QString("select bonus from scores where worker_id=%1 and year=%2 and month=%3").arg(this->id()).arg(year).arg(month));
    if(query.next()) {
        int value = query.value(0).toInt();
        return value;
    } else {
        this->oDatabase->exec(QString("insert into scores(worker_id, year, month, hours, bonus, load) values(%1,%2,%3,0,0,0)").arg(this->id()).arg(year).arg(month));
        return 0;
    }
}

int Worker::getLoad(int year)
{
    QSqlQuery query = this->oDatabase->exec(QString("select sum(load) from scores where worker_id=%1 and year=%2").arg(this->id()).arg(year));
    if(query.next()) {
        int value = query.value(0).toInt();
        return value;
    } else {
        return 0;
    }
}

int Worker::getBonus(int year)
{
    QSqlQuery query = this->oDatabase->exec(QString("select sum(bonus) from scores where worker_id=%1 and year=%2").arg(this->id()).arg(year));
    if(query.next()) {
        int value = query.value(0).toInt();
        return value;
    } else {
        return 0;
    }
}

int Worker::getHours(int year)
{
    QSqlQuery query = this->oDatabase->exec(QString("select sum(hours) from scores where worker_id=%1 and year=%2").arg(this->id()).arg(year));
    if(query.next()) {
        int value = query.value(0).toInt();
        return value;
    } else {
        return 0;
    }
}

void Worker::setSelect(bool can)
{
    this->bCanSelect = can;
}

void Worker::setLoad(int load, int year, int month)
{
    QSqlQuery query = this->oDatabase->exec(QString("select id from scores where worker_id=%1 and year=%2 and month=%3").arg(this->id()).arg(year).arg(month));
    if(query.next()) {
        this->oDatabase->exec(QString("update scores set load=%1 where worker_id=%2 and year=%3 and month=%4").arg(load).arg(this->id()).arg(year).arg(month));
    } else {
        this->oDatabase->exec(QString("insert into scores(worker_id, year, month, load) values(%1,%2,%3,%4)").arg(this->id()).arg(year).arg(month).arg(load));
    }
}

void Worker::setBonus(int bonus, int year, int month)
{
    QSqlQuery query = this->oDatabase->exec(QString("select id from scores where worker_id=%1 and year=%2 and month=%3").arg(this->id()).arg(year).arg(month));
    if(query.next()) {
        this->oDatabase->exec(QString("update scores set bonus=%1 where worker_id=%2 and year=%3 and month=%4").arg(bonus).arg(this->id()).arg(year).arg(month));
    } else {
        this->oDatabase->exec(QString("insert into scores(worker_id, year, month, bonus) values(%1,%2,%3,%4)").arg(this->id()).arg(year).arg(month).arg(bonus));
    }
}

void Worker::setHours(int hours, int year, int month)
{
    QSqlQuery query = this->oDatabase->exec(QString("select id from scores where worker_id=%1 and year=%2 and month=%3").arg(this->id()).arg(year).arg(month));
    if(query.next()) {
        this->oDatabase->exec(QString("update scores set hours=%1 where worker_id=%2 and year=%3 and month=%4").arg(hours).arg(this->id()).arg(year).arg(month));
    } else {
        this->oDatabase->exec(QString("insert into scores(worker_id, year, month, hours) values(%1,%2,%3,%4)").arg(this->id()).arg(year).arg(month).arg(hours));
    }
}

