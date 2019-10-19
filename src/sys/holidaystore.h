#ifndef HOLIDAYSTORE_H
#define HOLIDAYSTORE_H

#include "store.h"
#include "holiday.h"

class HolidayStore : public Store<Holiday>
{
    Q_OBJECT
private:
    bool intersectsWith(const QDate &start, const QDate &end, int id = -1) const;
public:
    HolidayStore(DatabaseManager *manager, Error *error, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QObject *insertHoliday(const QString &name, const QString &start, const QString &end, bool anual);
    Q_INVOKABLE QObject *updateHoliday(int id, const QString &name, const QString &start, const QString &end, bool anual);
    Q_INVOKABLE QObject *deleteHoliday(int id);
    bool isHoliday(const QDate &date) const;
};

#endif // HOLIDAYSTORE_H
