#ifndef MONTHLYSTATISTICS_H
#define MONTHLYSTATISTICS_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <QVariantList>

class WorkerStore;

class MonthlyStatistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(QVariantList value READ value NOTIFY valueChanged)
    Q_PROPERTY(QStringList real_val READ real_val NOTIFY real_valChanged)
    Q_PROPERTY(QStringList name READ name NOTIFY nameChanged)
    Q_PROPERTY(QStringList stat READ stat NOTIFY statChanged)
private:
    bool bShowBonus;
    bool bShowLoad;
    bool bShowHours;
    int iYear;
    int iMonth;
    WorkerStore *oWorkers;
public:
    MonthlyStatistics(WorkerStore *workers, QObject *parent = 0);
    int size() const;
    QVariantList value() const;
    QStringList real_val() const;
    QStringList name() const;
    QStringList stat() const;
    Q_INVOKABLE void setBonus(bool set);
    Q_INVOKABLE void setLoad(bool set);
    Q_INVOKABLE void setHours(bool set);
    Q_INVOKABLE void setDate(int year, int month);
signals:
    void sizeChanged();
    void valueChanged();
    void real_valChanged();
    void nameChanged();
    void statChanged();
public slots:
    void refreshWorkers();
    void refreshValues();
};

#endif // MONTHLYSTATISTICS_H
