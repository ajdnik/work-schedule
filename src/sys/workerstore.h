#ifndef WORKERSTORE_H
#define WORKERSTORE_H

#include "store.h"
#include "worker.h"

class WorkerEventStore;
class ScheduleStore;

class WorkerStore : public Store<Worker>
{
    Q_OBJECT
private:
    WorkerEventStore *oEvents;
    ScheduleStore *oSchedules;

public:
    WorkerStore(DatabaseManager *manager, Error *error, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QObject *insertWorker(const QString &name);
    Q_INVOKABLE QObject *updateWorker(int id, const QString &name);
    Q_INVOKABLE QObject *deleteWorker(int id);
    void setWorkerEventStore(WorkerEventStore *store);
    void setScheduleStore(ScheduleStore *store);
signals:
    void workerChanged();
};

#endif // WORKERSTORE_H
