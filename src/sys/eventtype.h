#ifndef EVENTTYPE_H
#define EVENTTYPE_H

#include <QString>

class EventType
{
private:
    int iId;
    QString sName;
public:
    EventType(int id, const QString &name);
    void setId(int id);
    void setName(const QString &name);
    int id() const;
    QString name() const;
};

#endif // EVENTTYPE_H
