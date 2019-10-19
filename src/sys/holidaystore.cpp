#include "holidaystore.h"
#include <QVariant>

HolidayStore::HolidayStore(DatabaseManager *manager, Error *error, QObject *parent)
    : Store<Holiday>(manager, error, parent)
{
    this->addRole("id");
    this->addRole("name");
    this->addRole("start");
    this->addRole("end");
    this->addRole("anual");
    this->addRole("anual_word");

    QSqlQuery query = this->oDatabase->exec("select * from holidays");
    while(query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QDate start = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        QDate end = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
        bool anual = query.value(4).toBool();
        this->add(Holiday(id, name, start, end, anual, this->oError), id);
    }
}

bool HolidayStore::intersectsWith(const QDate &start, const QDate &end, int id) const
{
    for(int i=0;i<this->aObjects.count();i++) {
        if(this->aObjects[i].id() != id && this->aObjects[i].intersectsWith(start, end))
            return true;
    }
    return false;
}

QVariant HolidayStore::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > this->rowCount())
        return QVariant();
    const Holiday &holiday = this->aObjects[index.row()];
    if (role == this->getRoleNumber("name"))
        return holiday.name();
    else if(role == this->getRoleNumber("id"))
        return holiday.id();
    else if(role == this->getRoleNumber("start"))
        return holiday.start().toString("yyyy-MM-dd");
    else if(role == this->getRoleNumber("end"))
        return holiday.end().toString("yyyy-MM-dd");
    else if(role == this->getRoleNumber("anual"))
        return holiday.anual();
    else if(role == this->getRoleNumber("anual_word"))
        return holiday.anual() ? this->tr("Yes") : this->tr("No");
    return QVariant();
}

QObject *HolidayStore::insertHoliday(const QString &name, const QString &start, const QString &end, bool anual)
{
    QDate startDate = QDate::fromString(start, "yyyy-MM-dd");
    QDate endDate = QDate::fromString(end, "yyyy-MM-dd");
    if(startDate.isNull() || !startDate.isValid())
        return this->setError(QObject::tr("Start date is not valid."));
    if(endDate.isNull() || !endDate.isValid())
        return this->setError(QObject::tr("End date is not valid."));
    if(startDate > endDate)
        return this->setError(QObject::tr("End date is smaller than start date."));
    if(this->intersectsWith(startDate, endDate))
        return this->setError(QObject::tr("Dates intersect with a previous holiday."));
    if(name.trimmed().size() == 0)
        return this->setError(QObject::tr("Name can not be empty."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("insert into holidays(name, start, end, anual) values(:Name,:Start,:End,:Anual)");
    query.bindValue(":Name", name);
    query.bindValue(":Start", start);
    query.bindValue(":End", end);
    query.bindValue(":Anual", anual);
    if(!query.exec())
        return this->setError(QObject::tr("Error while saving holiday to database."));
    int id = query.lastInsertId().toInt();
    Error *add = this->add(Holiday(id, name, startDate, endDate, anual, this->oError), id);
    if(add->isError())
        return add;
    return this->setNoError();
}

QObject *HolidayStore::updateHoliday(int id, const QString &name, const QString &start, const QString &end, bool anual)
{
    QDate startDate = QDate::fromString(start, "yyyy-MM-dd");
    QDate endDate = QDate::fromString(end, "yyyy-MM-dd");
    if(startDate.isNull() || !startDate.isValid())
        return this->setError(QObject::tr("Start date is not valid."));
    if(endDate.isNull() || !endDate.isValid())
        return this->setError(QObject::tr("End date is not valid."));
    if(startDate > endDate)
        return this->setError(QObject::tr("End date is smaller than start date."));
    if(this->intersectsWith(startDate, endDate, id))
        return this->setError(QObject::tr("Dates intersect with a previous holiday."));
    if(name.trimmed().size() == 0)
        return this->setError(QObject::tr("Name can not be empty."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("update holidays set name=:Name, start=:Start, end=:End, anual=:Anual where id=:Id");
    query.bindValue(":Name", name);
    query.bindValue(":Start", start);
    query.bindValue(":End", end);
    query.bindValue(":Anual", anual);
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while updating holiday in database."));
    Error *setter;
    setter = this->aObjects[this->mIds[id]].setName(name);
    if(setter->isError())
        return setter;
    setter = this->aObjects[this->mIds[id]].setStart(startDate);
    if(setter->isError())
        return setter;
    setter = this->aObjects[this->mIds[id]].setEnd(endDate);
    if(setter->isError())
        return setter;
    this->aObjects[this->mIds[id]].setAnual(anual);
    emit this->dataChanged(this->index(this->mIds[id]), this->index(this->mIds[id]));
    return this->setNoError();
}

QObject *HolidayStore::deleteHoliday(int id)
{
    if(!this->mIds.contains(id))
        return this->setError(QObject::tr("Holiday with id does not exists."));
    if(this->mIds[id] < 0 || this->mIds[id] >= this->aObjects.count())
        return this->setError(QObject::tr("Index out of range."));
    QSqlQuery query = this->oDatabase->query();
    query.prepare("delete from holidays where id=:Id");
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while deleting holiday from database."));
    Error *remove = this->remove(id);
    if(remove->isError())
        return remove;
    return this->setNoError();
}

bool HolidayStore::isHoliday(const QDate &date) const
{
    for(int i=0;i<this->aObjects.count();i++) {
        const Holiday &holiday = this->aObjects[i];
        if(holiday.isInside(date))
            return true;
    }
    return false;
}
