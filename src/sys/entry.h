#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QObject>
#include <QDateTime>

class Entry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name1 READ name1 NOTIFY name1Changed)
    Q_PROPERTY(QString name2 READ name2 NOTIFY name2Changed)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
private:
    int iId;
    int iWorkerId1;
    int iWorkerId2;
    QString oName1;
    QString oName2;
    int iLoad;
    int iHours;
    int iBonus;
    QDateTime oStart;
    QDateTime oEnd;
public:
    Entry(int id, int workerId1, int workerId2, const QString &name1, const QString &name2, int load, int hours, int bonus, const QDateTime &start, const QDateTime &end);
    QString name1() const;
    QString name2() const;
    int id1() const;
    int id2() const;
    int load() const;
    int bonus() const;
    int id() const;
    QDateTime start() const;
    QDateTime end() const;
    int duration() const;
    void setId1(int id);
    void setId2(int id);
    void setName1(const QString &name);
    void setName2(const QString &name);
signals:
    void name1Changed();
    void name2Changed();
    void durationChanged();
};

#endif // ENTRY_H
