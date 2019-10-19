#include "entry.h"

Entry::Entry(int id, int workerId1, int workerId2, const QString &name1, const QString &name2, int load, int hours, int bonus, const QDateTime &start, const QDateTime &end)
{
    this->iId = id;
    this->iWorkerId1 = workerId1;
    this->iWorkerId2 = workerId2;
    this->oName1 = name1;
    this->oName2 = name2;
    this->iLoad = load;
    this->iBonus = bonus;
    this->iHours = hours;
    this->oStart = start;
    this->oEnd = end;
}

void Entry::setId1(int id)
{
    this->iWorkerId1 = id;
}

void Entry::setId2(int id)
{
    this->iWorkerId2 = id;
}

void Entry::setName1(const QString &name)
{
    this->oName1 = name;
}

void Entry::setName2(const QString &name)
{
    this->oName2 = name;
}

int Entry::load() const
{
    return this->iLoad;
}

int Entry::bonus() const
{
    return this->iBonus;
}

int Entry::duration() const
{
    return this->iLoad;
}

int Entry::id1() const
{
    return this->iWorkerId1;
}

int Entry::id2() const
{
    return this->iWorkerId2;
}

QString Entry::name1() const
{
    return this->oName1;
}

QString Entry::name2() const
{
    return this->oName2;
}

QDateTime Entry::start() const
{
    return this->oStart;
}

QDateTime Entry::end() const
{
    return this->oEnd;
}

int Entry::id() const
{
    return this->iId;
}
