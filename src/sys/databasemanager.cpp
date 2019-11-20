#include<QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QStandardPaths>
#include "databasemanager.h"

DatabaseManager::DatabaseManager(const QString &filename, QObject* parent)
    : QObject(parent)
{
    this->oDatabase = QSqlDatabase::addDatabase("QSQLITE");
    this->oDatabase.setDatabaseName(filename);
    this->oDatabase.open();
    this->oDatabase.exec("PRAGMA foreign_keys = ON;");
    qInfo() << "Database loaded from: " << filename;
    this->checkAndMigrate();
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

void DatabaseManager::checkAndMigrate()
{
    QString oldDbPath = QDir::homePath() + QDir::separator() + "UrnikDela" + QDir::separator() + "appdata.db";
    QFileInfo check(oldDbPath);
    if(check.exists() && check.isFile()) {
        QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "appdata.db";
        QString backupNew = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "appdata-" + QString::number(QDateTime::currentSecsSinceEpoch()) + ".db";
        QString backupOld = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QDir::separator() + "appdata-old-" + QString::number(QDateTime::currentSecsSinceEpoch()) + ".db";
        if(!QFile::copy(oldDbPath, backupOld)) {
            qCritical() << "Copying " << oldDbPath << " to " << backupOld << " failed.";
            return;
        }
        this->oDatabase.close();
        if(!QFile::copy(dbPath, backupNew)) {
            qCritical() << "Copying " << dbPath << " to " << backupNew << " failed.";
        }
        if(!QFile::remove(dbPath)) {
            qCritical() << "Removing " << dbPath << " failed.";
            return;
        }
        if(!QFile::copy(oldDbPath, dbPath)) {
            qCritical() << "Copying " << oldDbPath << " to " << dbPath << " failed.";
            return;
        }
        this->oDatabase = QSqlDatabase::addDatabase("QSQLITE");
        this->oDatabase.setDatabaseName(dbPath);
        this->oDatabase.open();
        this->oDatabase.exec("PRAGMA foreign_keys = ON;");
        qInfo() << "Database reloaded from: " << dbPath;
        if(!QFile::remove(oldDbPath)) {
            qCritical() << "Removing " << oldDbPath << " failed.";
            return;
        }
    }
    QString local =  QLocale::languageToString(QLocale::system().language());
    qInfo() << "Local language: " << local;
    QSqlQuery checkTable = this->exec("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='settings'");
    if(!checkTable.next()) {
        qCritical() << "settings table check failed";
        return;
    }
    int count = checkTable.value(0).toInt();
    if(count > 0) {
        return;
    }
    QSqlQuery create = this->query();
    create.prepare("CREATE TABLE 'settings' ('id' INTEGER PRIMARY KEY AUTOINCREMENT, 'property' INTEGER)");
    if(!create.exec()) {
        qCritical() << "Could not create settings table.";
        return;
    }
    int langId = 2;
    if(local == "English") {
        langId = 1;
    }
    QSqlQuery insert = this->query();
    insert.prepare("INSERT into settings(id, property) values(1, :Lang)");
    insert.bindValue(":Lang", langId);
    if(!insert.exec()) {
        qCritical() << "Could not insert language settings into settings table.";
        return;
    }
}
