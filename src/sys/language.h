#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>

class Language
{
private:
    int iId;
    QString sName;
    QString sPath;
public:
    Language(int id, const QString &name, const QString &path);
    void setId(int id);
    void setName(const QString &name);
    void setPath(const QString &path);
    int id() const;
    QString name() const;
    QString path() const;
};

#endif // LANGUAGE_H
