#include "schedulecolumn.h"
#include "schedulerow.h"

ScheduleColumn::ScheduleColumn(QObject *parent)
    : QObject(parent)
{
}

ScheduleColumn::~ScheduleColumn()
{
    qDeleteAll(this->aRows);
}

int ScheduleColumn::rowCount() const
{
    return this->aRows.count();
}

QList<QObject *> ScheduleColumn::rows() const
{
    return this->aRows;
}

void ScheduleColumn::add(ScheduleRow *row)
{
    this->aRows.append(row);
    emit this->rowsChanged();
    emit this->rowCountChanged();
}
