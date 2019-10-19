#ifndef SCHEDULECOLUMN_H
#define SCHEDULECOLUMN_H

#include <QList>
#include <QObject>

class ScheduleRow;

class ScheduleColumn : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QObject *> rows READ rows NOTIFY rowsChanged)
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)
public:
    QList<QObject *> aRows;
public:
    ScheduleColumn(QObject *parent = 0);
    ~ScheduleColumn();
    QList<QObject *> rows() const;
    int rowCount() const;
    void add(ScheduleRow *row);
signals:
    void rowsChanged();
    void rowCountChanged();
};

#endif // SCHEDULECOLUMN_H
