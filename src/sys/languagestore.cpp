#include "languagestore.h"
#include <QVariant>

LanguageStore::LanguageStore(DatabaseManager *manager, Error *error, QGuiApplication *app, QObject *parent)
    : Store<Language>(manager, error, parent)
{
    this->oApp = app;

    this->addRole("id");
    this->addRole("name");

    this->add(Language(1, QObject::tr("English"), ":/lang/workschedule_en.qm"), 1);
    this->add(Language(2, QObject::tr("Slovenian"), ":/lang/workschedule_sl.qm"), 2);

    int currentId = this->getCurrentId();
    if(currentId == 0) {
        this->oTranslator.load(this->getPath(1));
    } else {
        this->oTranslator.load(this->getPath(currentId));
    }
    this->oApp->installTranslator(&this->oTranslator);
}

QVariant LanguageStore::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > this->rowCount())
        return QVariant();
    const Language &eventType = this->aObjects[index.row()];
    if (role == this->getRoleNumber("name"))
        return eventType.name();
    else if(role == this->getRoleNumber("id"))
        return eventType.id();
    return QVariant();
}

QString LanguageStore::getName(int id) const
{
    for(int i=0;i<this->aObjects.count();i++) {
        if(this->aObjects[i].id() == id)
            return this->aObjects[i].name();
    }
    return QString();
}

QString LanguageStore::getPath(int id) const
{
    for(int i=0;i<this->aObjects.count();i++) {
        if(this->aObjects[i].id() == id)
            return this->aObjects[i].path();
    }
    return QString();
}

int LanguageStore::getCurrentId()
{
    QSqlQuery query = this->oDatabase->exec("select property from settings where id=1");
    if(query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

QObject *LanguageStore::updateCurrentId(int id)
{
    QSqlQuery query = this->oDatabase->query();
    query.prepare("update settings set property=:Id where id=1");
    query.bindValue(":Id", id);
    if(!query.exec())
        return this->setError(QObject::tr("Error while updating language settings in database."));
    this->oApp->removeTranslator(&this->oTranslator);
    this->oTranslator.load(this->getPath(id));
    this->oApp->installTranslator(&this->oTranslator);
    return this->setNoError();
}
