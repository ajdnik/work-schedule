#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "error.h"

class ErrorHandler
{
protected:
    Error *oError;
    bool bNull;

    Error *setError(const QString &error)
    {
        this->oError->setError(error);
        return this->oError;
    }

    Error *setNoError()
    {
        this->oError->setNoError();
        return this->oError;
    }
public:
    ErrorHandler(Error *error)
    {
        this->oError = error;
    }

    bool isNull() const
    {
        return this->bNull;
    }
};

#endif // ERRORHANDLER_H
