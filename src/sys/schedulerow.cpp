#include "schedulerow.h"
#include "entry.h"

ScheduleRow::ScheduleRow(int duration, QObject *parent)
    : QObject(parent)
{
    this->sColor = "transparent";
    this->bNextTwin = false;
    this->bPreviousTwin = false;
    this->iDuration = duration;
    this->oWorkers = 0;
    this->iWorkerCount = 1;
    this->oNames.append("");
}

ScheduleRow::ScheduleRow(int duration, Entry *entry, const QString &color, bool nextTwin, bool previousTwin, QObject *parent)
    : QObject(parent)
{
    this->sColor = color;
    this->bNextTwin = nextTwin;
    this->bPreviousTwin = previousTwin;
    this->iDuration = duration;
    this->oWorkers = entry;
    this->iWorkerCount = 2;
    this->sBonus = QString::number(this->oWorkers->bonus());
    this->oNames.append(this->oWorkers->name1());
    this->oNames.append(this->oWorkers->name2());
}

bool ScheduleRow::isStartEndEqual() const
{
    if(this->oWorkers == 0)
        return false;
    if(this->oWorkers->start().date().day() == this->oWorkers->end().date().day())
        return true;
    return false;
}

QString ScheduleRow::color() const
{
    return this->sColor;
}

QString ScheduleRow::bonus() const
{
    return this->sBonus;
}

int ScheduleRow::duration() const
{
    return this->iDuration;
}

int ScheduleRow::id() const
{
    return this->oWorkers->id();
}

int ScheduleRow::workerCount() const
{
    return this->iWorkerCount;
}

bool ScheduleRow::nextTwin() const
{
    return this->bNextTwin;
}

bool ScheduleRow::previousTwin() const
{
    return this->bPreviousTwin;
}

QStringList ScheduleRow::workers() const
{
    return this->oNames;
}

void ScheduleRow::addEntry(Entry *entry)
{
    this->oWorkers = entry;
}
