#include "workerevent.h"

WorkerEvent::WorkerEvent(int id, int workerId, int typeId, const QDateTime &start, const QDateTime &end, Error *error)
    : Duration<QDateTime>(start, end, error)
{
    if(id < 0 || workerId < 0 || typeId < 0)
        this->bNull = true;
    this->iId = id;
    this->iWorkerId = workerId;
    this->iTypeId = typeId;
}

Error *WorkerEvent::setId(int id)
{
    if(id < 0)
        return this->setError(QObject::tr("Id value is not valid. Must be nonnegative integer."));
    this->iId = id;
    return this->setNoError();
}

Error *WorkerEvent::setWorkerId(int id)
{
    if(id < 0)
        return this->setError(QObject::tr("Worker id value is not valid. Must be nonnegative integer."));
    this->iWorkerId = id;
    return this->setNoError();
}

Error *WorkerEvent::setTypeId(int id)
{
    if(id < 0)
        return this->setError(QObject::tr("Type id value is not valid. Must be nonnegative integer."));
    this->iTypeId = id;
    return this->setNoError();
}

int WorkerEvent::id() const
{
    return this->iId;
}

int WorkerEvent::workerId() const
{
    return this->iWorkerId;
}

int WorkerEvent::typeId() const
{
    return this->iTypeId;
}
