#ifndef HOLIDAY_H
#define HOLIDAY_H

#include <QDate>
#include "duration.h"

class Holiday : public Duration<QDate>
{
private:
    int iId;
    QString sName;
    bool bAnual;
public:
    Holiday(int id, const QString &name, const QDate &start, const QDate &end, bool anual, Error *error);
    Error *setId(int id);
    Error *setName(const QString &name);
    void setAnual(bool anual);
    int id() const;
    QString name() const;
    bool anual() const;
    bool isInside(const QDate &date) const;
    bool intersectsWith(const QDate &start, const QDate &end) const;
};

#endif // HOLIDAY_H
