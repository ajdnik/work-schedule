#include "holiday.h"

Holiday::Holiday(int id, const QString &name, const QDate &start, const QDate &end, bool anual, Error *error)
    : Duration<QDate>(start, end, error)
{
    if(id < 0 || name.trimmed().size() == 0)
        this->bNull = true;
    this->iId = id;
    this->sName = name;
    this->bAnual = anual;
}

Error *Holiday::setId(int id)
{
    if(id < 0)
        return this->setError(QObject::tr("Id must be nonnegative integer."));
    this->iId = id;
    return this->setNoError();
}

Error *Holiday::setName(const QString &name)
{
    if(name.trimmed().size() == 0)
        return this->setError(QObject::tr("Name can not be empty."));
    this->sName = name;
    return this->setNoError();
}

void Holiday::setAnual(bool anual)
{
    this->bAnual = anual;
}

int Holiday::id() const
{
    return this->iId;
}

QString Holiday::name() const
{
    return this->sName;
}

bool Holiday::anual() const
{
    return this->bAnual;
}

bool Holiday::isInside(const QDate &date) const
{
    if(date.isNull() || !date.isValid())
        return false;
    QDate s = this->oStart;
    QDate e = this->oEnd;
    QDate value(date.year(), date.month(), date.day());
    if(this->anual()) {
        int year = e.year() - s.year() + 1;
        s.setDate(1, s.month(), s.day());
        e.setDate(year, e.month(), e.day());
        value.setDate(1, value.month(), value.day());
    }
    if(s <= value && e >= value)
        return true;
    else
        return false;
}

bool Holiday::intersectsWith(const QDate &start, const QDate &end) const
{
    if(!start.isValid() || start.isNull() || end.isNull() || !end.isValid())
        return false;
    if(end < start)
        return false;
    QDate s = this->oStart;
    QDate e = this->oEnd;
    QDate sA(start.year(), start.month(), start.day());
    QDate eA(end.year(), end.month(), end.day());
    if(this->anual()) {
        int year = e.year() - s.year() + 1;
        s.setDate(1, s.month(), s.day());
        e.setDate(year, e.month(), e.day());
        year = end.year() - start.year() + 1;
        sA.setDate(1, start.month(), start.day());
        eA.setDate(year, end.month(), end.day());
    }
    if((s <= sA && e <= eA && e >= sA) || (s >= sA && e >= eA && s <= eA) || (s >= sA && e <= eA))
        return true;
    else
        return false;
}
