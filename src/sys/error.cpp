#include "error.h"
#include <QDebug>

Error::Error(QObject *parent)
    : QObject(parent)
{
    this->bIsError = false;
    this->sError = "";
}

Error::Error(const QString &error, QObject *parent)
    : QObject(parent)
{
    this->bIsError = true;
    this->sError = error;
    qWarning() << error;
}

void Error::setError(const QString &error)
{
    this->sError = error;
    this->bIsError = true;
    emit this->errorChanged();
}

void Error::setNoError()
{
    this->sError = "";
    this->bIsError = false;
    emit this->errorChanged();
}

QString Error::error() const
{
    return this->sError;
}

bool Error::isError() const
{
    return this->bIsError;
}
