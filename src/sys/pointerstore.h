#ifndef POINTERSTORE_H
#define POINTERSTORE_H

#include <QAbstractListModel>
#include <QList>
#include <QHashIterator>
#include "errorhandler.h"
#include "databasemanager.h"

template<class T>
class PointerStore : public QAbstractListModel, public ErrorHandler
{
private:
    QHash<int, QByteArray> mRoles;
    QHash<QString, int> mRoleNumbers;
    int iRoleCounter;

protected:
    QList<T *> aObjects;
    QHash<int, int> mIds;
    DatabaseManager *oDatabase;

    Error *addRole(const QString &name)
    {
        if(this->mRoleNumbers.contains(name))
            return this->setError(QObject::tr("Role already exists."));
        this->mRoles[this->iRoleCounter] = name.toLocal8Bit();
        this->mRoleNumbers[name] = this->iRoleCounter;
        this->iRoleCounter++;
        return this->setNoError();
    }

    int getRoleNumber(const QString &name) const
    {
        if(!this->mRoleNumbers.contains(name))
            return -1;
        return this->mRoleNumbers[name];
    }

public:
    PointerStore(DatabaseManager *manager, Error *error, QObject *parent = 0)
        : QAbstractListModel(parent), ErrorHandler(error)
    {
        this->iRoleCounter = Qt::UserRole + 1;
        this->oDatabase = manager;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        return this->aObjects.count();
    }

    Error *add(T *object, int id)
    {
        if(id < 0)
            return this->setError(QObject::tr("Id in not valid. Must be nonnegative integer."));
        if(this->mIds.contains(id))
            return this->setError(QObject::tr("Id already exists."));
        this->beginInsertRows(QModelIndex(), this->rowCount(), this->rowCount());
        this->aObjects << object;
        this->mIds[id] = this->aObjects.count() - 1;
        this->endInsertRows();
        return this->setNoError();
    }

    Error *remove(int id)
    {
        if(id < 0)
            return this->setError(QObject::tr("Id in not valid. Must be nonnegative integer."));
        if(!this->mIds.contains(id))
            return this->setError(QObject::tr("Id does not exists."));
        int removeIndex = this->mIds[id];
        this->mIds.remove(id);
        QHashIterator<int, int> i(this->mIds);
        while(i.hasNext()) {
            i.next();
            if(i.value() > removeIndex)
                this->mIds[i.key()]--;
        }
        this->beginRemoveRows(QModelIndex(), removeIndex, removeIndex);
        this->aObjects.removeAt(removeIndex);
        this->endRemoveRows();
        return this->setNoError();
    }

    T *get(int id)
    {
        if(!this->mIds.contains(id))
            return 0;
        if(this->aObjects.count() <= this->mIds[id])
            return 0;
        return this->aObjects[this->mIds[id]];
    }

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;

    QHash<int, QByteArray> roleNames() const
    {
        return this->mRoles;
    }
};


#endif // POINTERSTORE_H
