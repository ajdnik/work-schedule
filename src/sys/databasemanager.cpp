#include<QDebug>
#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString &filename, QObject* parent)
    : QObject(parent)
{
    this->oDatabase = QSqlDatabase::addDatabase("QSQLITE");
    this->oDatabase.setDatabaseName(filename);
    this->oDatabase.open();
    this->oDatabase.exec("PRAGMA foreign_keys = ON;");
    qInfo() << "Database loaded from: " << filename;
}

QSqlQuery DatabaseManager::exec(const QString &query)
{
    return this->oDatabase.exec(query);
}

QSqlQuery DatabaseManager::query()
{
    return QSqlQuery(this->oDatabase);
}

void DatabaseManager::close()
{
    this->oDatabase.close();
}
