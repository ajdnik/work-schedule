#ifndef WORKEREVENTSTORE_H
#define WORKEREVENTSTORE_H

#include <QString>
#include "pointerstore.h"
#include "workerevent.h"

class EventTypeStore;
class WorkerStore;

class WorkerEventStore : public PointerStore<WorkerEvent>
{
    Q_OBJECT
    Q_PROPERTY(int Empty READ empty CONSTANT)
private:
    EventTypeStore *oEventTypeStore;
    WorkerStore *oWorkers;
    int iWorkerId;
    bool bShowExpired;

    Error *populate();
    bool intersectsWith(const QDateTime &start, const QDateTime &end, int id = -1) const;
public:
    WorkerEventStore(WorkerStore *workers, EventTypeStore *eventTypeStore, DatabaseManager *manager, Error *error, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QObject *insertEvent(int typeId, const QString &start, const QString &end);
    Q_INVOKABLE QObject *updateEvent(int id, int typeId, const QString &start, const QString &end);
    Q_INVOKABLE QObject *deleteEvent(int id);
    Q_INVOKABLE QObject *setWorkerId(int id);
    Q_INVOKABLE QObject *setShowExpired(bool show);
    int empty() const;
};

#endif // WORKEREVENTSTORE_H
