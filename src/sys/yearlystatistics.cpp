#include "yearlystatistics.h"
#include "workerstore.h"
#include <QDate>
#include <qmath.h>

YearlyStatistics::YearlyStatistics(WorkerStore *workers, QObject *parent) :
    QObject(parent)
{
    this->oWorkers = workers;
    this->bShowBonus = true;
    this->bShowHours = true;
    this->bShowLoad = true;
    this->iYear = QDate::currentDate().year();
}

int YearlyStatistics::size() const
{
    return this->oWorkers->rowCount();
}

QStringList YearlyStatistics::stat() const
{
    QList<Worker *> workers = this->oWorkers->getData();
    QList<double> load;
    QList<double> bonus;
    QList<double> hours;
    double sumLoad = 0, sumBonus = 0, sumHours = 0;
    double avgLoad = 0, avgBonus = 0, avgHours = 0;
    for(int i=0;i<workers.count();i++) {
        load.append(workers[i]->getLoad(this->iYear));
        bonus.append(workers[i]->getBonus(this->iYear));
        hours.append(workers[i]->getHours(this->iYear));
        sumLoad += load.last();
        sumBonus += bonus.last();
        sumHours += hours.last();
    }
    avgLoad = sumLoad / (double)workers.count();
    avgBonus = sumBonus / (double)workers.count();
    avgHours = sumHours / (double)workers.count();
    double sumLoadStd = 0, sumBonusStd = 0, sumHoursStd = 0;
    for(int i=0;i<workers.count();i++) {
        sumLoadStd += qPow(load[i] - avgLoad, 2);
        sumBonusStd += qPow(bonus[i] - avgBonus, 2);
        sumHoursStd += qPow(hours[i] - avgHours, 2);
    }
    double stdLoad = qSqrt(sumLoadStd * 1.0 / (double)(workers.count() - 1));
    double stdBonus = qSqrt(sumBonusStd * 1.0 / (double)(workers.count() - 1));
    double stdHours = qSqrt(sumHoursStd * 1.0 / (double)(workers.count() - 1));
    QStringList values;
    values.append(QString::number(avgLoad, 'f', 3));
    values.append(QString::number(stdLoad, 'f', 3));
    values.append(QString::number(avgBonus, 'f', 3));
    values.append(QString::number(stdBonus, 'f', 3));
    values.append(QString::number(avgHours, 'f', 3));
    values.append(QString::number(stdHours, 'f', 3));
    return values;
}

QVariantList YearlyStatistics::value() const
{
    QList<double> values;
    QList<Worker *> workers = this->oWorkers->getData();
    double maxValue = 0.0;
    for(int i=0;i<workers.count();i++) {
        values.append(0);
        if(this->bShowBonus)
            values[i] += workers[i]->getBonus(this->iYear);
        if(this->bShowLoad)
            values[i] += workers[i]->getLoad(this->iYear);
        if(this->bShowHours)
            values[i] += workers[i]->getHours(this->iYear);
        if(i == 0)
            maxValue = values[i];
        else
            if(values[i] > maxValue)
                maxValue = values[i];
    }
    QVariantList vvalues;
    for(int i=0;i<values.count();i++) {
        if(maxValue > 0) {
            vvalues.append(values[i] / maxValue);
        } else {
            vvalues.append(0);
        }
    }
    return vvalues;
}

QStringList YearlyStatistics::real_val() const
{
    QList<int> values;
    QList<Worker *> workers = this->oWorkers->getData();
    for(int i=0;i<workers.count();i++) {
        values.append(0);
        if(this->bShowBonus)
            values[i] += workers[i]->getBonus(this->iYear);
        if(this->bShowLoad)
            values[i] += workers[i]->getLoad(this->iYear);
        if(this->bShowHours)
            values[i] += workers[i]->getHours(this->iYear);
    }
    QStringList vvalues;
    for(int i=0;i<values.count();i++) {
        if(values[i] == 0)
            vvalues.append("");
        else
            vvalues.append(QString::number(values[i]));
    }
    return vvalues;
}

QStringList YearlyStatistics::name() const
{
    QStringList names;
    QList<Worker *> workers = this->oWorkers->getData();
    for(int i=0;i<workers.count();i++) {
        names.append(workers[i]->name());
    }
    return names;
}

void YearlyStatistics::setBonus(bool set)
{
    this->bShowBonus = set;
    emit this->valueChanged();
    emit this->real_valChanged();
}

void YearlyStatistics::setHours(bool set)
{
    this->bShowHours = set;
    emit this->valueChanged();
    emit this->real_valChanged();
}

void YearlyStatistics::setLoad(bool set)
{
    this->bShowLoad = set;
    emit this->valueChanged();
    emit this->real_valChanged();
}

void YearlyStatistics::setYear(int year)
{
    this->iYear = year;
    emit this->valueChanged();
    emit this->real_valChanged();
    emit this->statChanged();
}

void YearlyStatistics::refreshWorkers()
{
    emit this->sizeChanged();
    emit this->nameChanged();
}

void YearlyStatistics::refreshValues()
{
    emit this->valueChanged();
    emit this->real_valChanged();
    emit this->statChanged();
}
