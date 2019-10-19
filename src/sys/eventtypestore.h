#ifndef EVENTTYPESTORE_H
#define EVENTTYPESTORE_H

#include "store.h"
#include "eventtype.h"

class EventTypeStore : public Store<EventType>
{
public:
    Q_OBJECT
public:
    EventTypeStore(DatabaseManager *manager, Error *error, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QString getName(int id) const;
};

#endif // EVENTTYPESTORE_H
