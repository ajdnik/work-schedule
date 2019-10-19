#include "workerstore.h"
#include <QVariant>
#include "workereventstore.h"
#include "schedulestore.h"

WorkerStore::WorkerStore(DatabaseManager *manager, Error *error, QObject *parent)
    : Store<Worker>(manager, error, parent)
{
    this->addRole("id");
    this->addRole("name");
    this->oEvents = 0;
    this->oSchedules = 0;

    QSqlQuery query = this->oDatabase->exec("select id, name from workers");
    while(query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        Worker worker(id, name, this->oError, this->oDatabase);
        QSqlQuery eventQuery = this->oDatabase->exec(QString("select * from worker_events where worker_id=%1").arg(id));
        while(eventQuery.next()) {
            int eventId = eventQuery.value(0).toInt();
            int typeId = eventQuery.value(2).toInt();
            QDateTime start = QDateTime::fromString(eventQuery.value(3).toString(), "yyyy-MM-dd HH:mm:ss");
            QDateTime end = QDateTime::fromString(eventQuery.value(4).toString(), "yyyy-MM-dd HH:mm:ss");
            worker.addEvent(WorkerEvent(eventId, id, typeId, start, end, this->oError));
        }
        this->add(worker, id);
    }
}

QVariant WorkerStore::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > this->rowCount())
        return QVariant();
    const Worker &worker = this->aObjects[index.row()];
    if (role == this->getRoleNumber("name"))
        return worker.name();
    else if(role == this->getRoleNumber("id"))
        return worker.id();
    return QVariant();
}

QObject *WorkerStore::insertWorker(const QString &name)
{
    if(name.trimmed().size() == 0)
        return this->setError(QObject::tr("Name can not be empty."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("insert into workers(name) values(:Name)");
    query.bindValue(":Name", name);
    if(!query.exec())
        return this->setError(QObject::tr("Error while saving worker to database."));
    int id = query.lastInsertId().toInt();
    Error *add = this->add(Worker(id, name, this->oError, this->oDatabase), id);
    if(add->isError())
        return add;
    emit this->workerChanged();
    return this->setNoError();
}

QObject *WorkerStore::updateWorker(int id, const QString &name)
{
    if(name.trimmed().size() == 0)
        return this->setError(QObject::tr("Name can not be empty."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("update workers set name=:Name where id=:Id");
    query.bindValue(":Name", name);
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while updating worker in database."));
    query = this->oDatabase->query();
    query.prepare("update entries set worker_name_1=:Name where worker_id_1=:Id");
    query.bindValue(":Name", name);
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while updating worker entries in database."));
    query = this->oDatabase->query();
    query.prepare("update entries set worker_name_2=:Name where worker_id_2=:Id");
    query.bindValue(":Name", name);
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while updating worker entries in database."));
    Error *setter = this->aObjects[this->mIds[id]].setName(name);
    if(setter->isError())
        return setter;
    if(this->oSchedules != 0)
        this->oSchedules->updateCurrent();
    emit this->dataChanged(this->index(this->mIds[id]), this->index(this->mIds[id]));
    emit this->workerChanged();
    return this->setNoError();
}

QObject *WorkerStore::deleteWorker(int id)
{
    if(!this->mIds.contains(id))
        return this->setError(QObject::tr("Worker with id does not exists."));
    if(this->mIds[id] < 0 || this->mIds[id] >= this->aObjects.count())
        return this->setError(QObject::tr("Index out of range."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("delete from workers where id=:Id");
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while deleting worker from database."));
    this->oDatabase->exec(QString("delete from workers where id=%1").arg(id));
    Error *remove = this->remove(id);
    if(remove->isError())
        return remove;
    emit this->workerChanged();
    if(this->oEvents != 0) {
        return this->oEvents->setWorkerId(this->oEvents->empty());
    }
    return this->setNoError();
}

void WorkerStore::setWorkerEventStore(WorkerEventStore *store)
{
    this->oEvents = store;
}

void WorkerStore::setScheduleStore(ScheduleStore *store)
{
    this->oSchedules = store;
}
