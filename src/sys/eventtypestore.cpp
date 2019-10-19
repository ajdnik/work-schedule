#include "eventtypestore.h"
#include <QVariant>

EventTypeStore::EventTypeStore(DatabaseManager *manager, Error *error, QObject *parent)
    : Store<EventType>(manager, error, parent)
{
    this->addRole("id");
    this->addRole("name");

    this->add(EventType(1, QObject::tr("Vacation")), 1);
    this->add(EventType(2, QObject::tr("Medical")), 2);
    this->add(EventType(3, QObject::tr("Other")), 3);
}

QVariant EventTypeStore::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > this->rowCount())
        return QVariant();
    const EventType &eventType = this->aObjects[index.row()];
    if (role == this->getRoleNumber("name"))
        return eventType.name();
    else if(role == this->getRoleNumber("id"))
        return eventType.id();
    return QVariant();
}

QString EventTypeStore::getName(int id) const
{
    for(int i=0;i<this->aObjects.count();i++) {
        if(this->aObjects[i].id() == id)
            return this->aObjects[i].name();
    }
    return QString();
}
