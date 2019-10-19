#ifndef WORKEREVENT_H
#define WORKEREVENT_H

#include <QDateTime>
#include "duration.h"

class WorkerEvent : public Duration<QDateTime>
{
private:
    int iId;
    int iWorkerId;
    int iTypeId;
public:
    WorkerEvent(int id, int workerId, int typeId, const QDateTime &start, const QDateTime &end, Error *error);
    Error *setId(int id);
    Error *setWorkerId(int id);
    Error *setTypeId(int id);
    int id() const;
    int workerId() const;
    int typeId() const;
};

#endif // WORKEREVENT_H
