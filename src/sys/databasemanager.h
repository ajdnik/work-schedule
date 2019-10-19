#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseManager : public QObject
{
    Q_OBJECT
private:
    QSqlDatabase oDatabase;
public:
    DatabaseManager(const QString &filename, QObject* parent = 0);
    QSqlQuery exec(const QString &query);
    QSqlQuery query();
public slots:
    void close();
};

#endif // DATABASEMANAGER_H
