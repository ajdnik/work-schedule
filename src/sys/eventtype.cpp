#include "eventtype.h"

EventType::EventType(int id, const QString &name)
{
    this->iId = id;
    this->sName = name;
}

void EventType::setId(int id)
{
    this->iId = id;
}

void EventType::setName(const QString &name)
{
    this->sName = name;
}

int EventType::id() const
{
    return this->iId;
}

QString EventType::name() const
{
    return this->sName;
}
