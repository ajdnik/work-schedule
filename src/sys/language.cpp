#include "language.h"

Language::Language(int id, const QString &name, const QString &path)
{
    this->iId = id;
    this->sName = name;
    this->sPath = path;
}

void Language::setId(int id)
{
    this->iId = id;
}

void Language::setName(const QString &name)
{
    this->sName = name;
}

void Language::setPath(const QString &path)
{
    this->sPath = path;
}

int Language::id() const
{
    return this->iId;
}

QString Language::name() const
{
    return this->sName;
}

QString Language::path() const
{
    return this->sPath;
}
