#include "workereventstore.h"
#include <QVariant>
#include "eventtypestore.h"
#include "workerstore.h"

WorkerEventStore::WorkerEventStore(WorkerStore *workers, EventTypeStore *eventTypeStore, DatabaseManager *manager, Error *error, QObject *parent)
    : PointerStore<WorkerEvent>(manager, error, parent)
{
    this->addRole("id");
    this->addRole("event_type");
    this->addRole("type_id");
    this->addRole("start");
    this->addRole("end");
    this->oEventTypeStore = eventTypeStore;
    this->oWorkers = workers;
    this->bShowExpired = false;
}

Error *WorkerEventStore::populate()
{
    Worker *worker = this->oWorkers->get(this->iWorkerId);
    for(int i=0;i<worker->eventsCount();i++) {
        WorkerEvent *event = worker->event(i);
        if(!this->bShowExpired) {
            if(event->end() >= QDateTime::currentDateTime()) {
                Error *add = this->add(event, event->id());
                if(add->isError())
                    return add;
            }
        } else {
            Error * add = this->add(event, event->id());
            if(add->isError())
                return add;
        }
    }
    return this->setNoError();
}

int WorkerEventStore::empty() const
{
    return -1;
}

bool WorkerEventStore::intersectsWith(const QDateTime &start, const QDateTime &end, int id) const
{
    for(int i=0;i<this->aObjects.count();i++) {
        if(this->aObjects[i]->id() != id && this->aObjects[i]->intersectsWith(start, end))
            return true;
    }
    return false;
}

QVariant WorkerEventStore::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > this->rowCount())
        return QVariant();
    const WorkerEvent *event = this->aObjects[index.row()];
    if (role == this->getRoleNumber("event_type"))
        return this->oEventTypeStore->getName(event->typeId());
    else if(role == this->getRoleNumber("id"))
        return event->id();
    else if(role == this->getRoleNumber("start"))
        return event->start().toString("yyyy-MM-dd HH:mm");
    else if(role == this->getRoleNumber("end"))
        return event->end().toString("yyyy-MM-dd HH:mm");
    else if(role == this->getRoleNumber("type_id"))
        return event->typeId();
    return QVariant();
}

QObject *WorkerEventStore::insertEvent(int typeId, const QString &start, const QString &end)
{
    QDateTime startDatetime = QDateTime::fromString(start, "yyyy-MM-dd HH:mm:ss");
    QDateTime endDatetime = QDateTime::fromString(end, "yyyy-MM-dd HH:mm:ss");
    if(startDatetime.isNull() || !startDatetime.isValid())
        return this->setError(QObject::tr("Start date-time is not valid."));
    if(endDatetime.isNull() || !endDatetime.isValid())
        return this->setError(QObject::tr("End date-time is not valid."));
    if(startDatetime > endDatetime)
        return this->setError(QObject::tr("End date-time is smaller than start date-time."));
    if(this->intersectsWith(startDatetime, endDatetime))
        return this->setError(QObject::tr("Date-times intersect with a previous event."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("insert into worker_events(worker_id,type_id,start,end) values(:WorkerId,:TypeId,:Start,:End)");
    query.bindValue(":WorkerId", this->iWorkerId);
    query.bindValue(":TypeId", typeId);
    query.bindValue(":Start", start);
    query.bindValue(":End", end);
    if(!query.exec())
        return this->setError(QObject::tr("Error while saving event to database."));
    int id = query.lastInsertId().toInt();
    Error *add = this->oWorkers->get(this->iWorkerId)->addEvent(WorkerEvent(id, this->iWorkerId, typeId, startDatetime, endDatetime, this->oError));
    if(add->isError())
        return add;
    if(!this->bShowExpired) {
        if(endDatetime >= QDateTime::currentDateTime()) {
            add = this->add(this->oWorkers->get(this->iWorkerId)->event(this->oWorkers->get(this->iWorkerId)->eventsCount() - 1), id);
            if(add->isError())
                return add;
        }
    } else {
        add = this->add(this->oWorkers->get(this->iWorkerId)->event(this->oWorkers->get(this->iWorkerId)->eventsCount() - 1), id);
        if(add->isError())
            return add;
    }
    return this->setNoError();
}

QObject *WorkerEventStore::updateEvent(int id, int typeId, const QString &start, const QString &end)
{
    if(!this->mIds.contains(id))
        return this->setError(QObject::tr("Event with id does not exists."));
    QDateTime startDatetime = QDateTime::fromString(start, "yyyy-MM-dd HH:mm:ss");
    QDateTime endDatetime = QDateTime::fromString(end, "yyyy-MM-dd HH:mm:ss");
    if(startDatetime.isNull() || !startDatetime.isValid())
        return this->setError(QObject::tr("Start date-time is not valid."));
    if(endDatetime.isNull() || !endDatetime.isValid())
        return this->setError(QObject::tr("End date-time is not valid."));
    if(startDatetime > endDatetime)
        return this->setError(QObject::tr("End date-time is smaller than start date-time."));
    if(this->intersectsWith(startDatetime, endDatetime, id))
        return this->setError(QObject::tr("Date-times intersect with a previous event."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("update worker_events set type_id=:TypeId, start=:Start, end=:End where id=:Id");
    query.bindValue(":Id", id);
    query.bindValue(":TypeId", typeId);
    query.bindValue(":Start", start);
    query.bindValue(":End", end);
    if(!query.exec())
        return this->setError(QObject::tr("Error while updating event in database."));
    Error *setter;
    setter = this->aObjects[this->mIds[id]]->setTypeId(typeId);
    if(setter->isError())
        return setter;
    setter = this->aObjects[this->mIds[id]]->setStart(startDatetime);
    if(setter->isError())
        return setter;
    setter = this->aObjects[this->mIds[id]]->setEnd(endDatetime);
    if(setter->isError())
        return setter;
    if(!this->bShowExpired) {
        if(endDatetime < QDateTime::currentDateTime()) {
            Error *remove = this->remove(id);
            if(remove->isError())
                return remove;
        } else {
            emit this->dataChanged(this->index(this->mIds[id]), this->index(this->mIds[id]));
        }
    } else {
        emit this->dataChanged(this->index(this->mIds[id]), this->index(this->mIds[id]));
    }
    return this->setNoError();
}

QObject *WorkerEventStore::deleteEvent(int id)
{
    if(!this->mIds.contains(id))
        return this->setError(QObject::tr("Event with id does not exists."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("delete from worker_events where id=:Id");
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while deleting event from database."));
    Error *remove = this->oWorkers->get(this->iWorkerId)->removeEvent(this->mIds[id]);
    if(remove->isError())
        return remove;
    remove = this->remove(id);
    if(remove->isError())
        return remove;
    return this->setNoError();
}

QObject *WorkerEventStore::setWorkerId(int id)
{
    if(id == this->empty()) {
        beginResetModel();
        this->aObjects.clear();
        this->mIds.clear();
        endResetModel();
        return this->setNoError();
    } else {
        if(id >= 0) {
            beginResetModel();
            this->aObjects.clear();
            this->mIds.clear();
            this->iWorkerId = id;
            Error *populate = this->populate();
            endResetModel();
            if(populate->isError())
                return populate;
            return this->setNoError();
        }

    }
    return this->setError(QObject::tr("Invalid id value."));
}

QObject *WorkerEventStore::setShowExpired(bool show)
{
    this->bShowExpired = show;
    beginResetModel();
    this->aObjects.clear();
    Error *populate = this->populate();
    endResetModel();
    if(populate->isError())
        return populate;
    return this->setNoError();
}
