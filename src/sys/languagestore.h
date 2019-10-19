#ifndef LANGUAGESTORE_H
#define LANGUAGESTORE_H

#include <QGuiApplication>
#include <QTranslator>
#include "store.h"
#include "language.h"

class LanguageStore : public Store<Language>
{
private:
   QTranslator oTranslator;
   QGuiApplication *oApp;
public:
    Q_OBJECT
public:
    LanguageStore(DatabaseManager *manager, Error *error, QGuiApplication *app, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE QString getName(int id) const;
    Q_INVOKABLE QString getPath(int id) const;
    Q_INVOKABLE int getCurrentId();
    Q_INVOKABLE QObject *updateCurrentId(int id);
};

#endif // LANGUAGESTORE_H
