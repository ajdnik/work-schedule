#ifndef DURATION_H
#define DURATION_H

#include "errorhandler.h"

template<class T>
class Duration : public ErrorHandler
{
protected:
    T oStart;
    T oEnd;

public:
    Duration(const T &start, const T &end, Error *error)
        : ErrorHandler(error)
    {
        if(start.isNull() || !start.isValid() || end.isNull() || !end.isValid()) {
            this->bNull = true;
        } else {
            this->bNull = false;
        }
        this->oStart = start;
        this->oEnd = end;
    }

    Error *setStart(const T &start)
    {
        if(start.isNull() || !start.isValid())
            return this->setError(QObject::tr("Start value is not valid."));
        this->oStart = start;
        return this->setNoError();
    }

    Error *setEnd(const T &end)
    {
        if(end.isNull() || !end.isValid())
            return this->setError(QObject::tr("End value is not valid."));
        this->oEnd = end;
        return this->setNoError();
    }

    T start() const
    {
        return this->oStart;
    }

    T end() const
    {
        return this->oEnd;
    }

    virtual bool intersectsWith(const T &start, const T &end) const
    {
        if(!start.isValid() || start.isNull() || end.isNull() || !end.isValid())
            return false;
        if((this->oStart <= start && this->oEnd <= end && this->oEnd >= start) || (this->oStart >= start && this->oEnd >= end && this->oStart <= end) || (this->oStart >= start && this->oEnd <= end))
            return true;
        else
            return false;
    }

    virtual bool isInside(const T &start, const T &end) const
    {
        if(!start.isValid() || start.isNull() || end.isNull() || !end.isValid())
            return false;
        if((this->oStart >= start && end >= this->oStart) || (this->oEnd <= end && start <= this->oEnd) || (this->oStart <= start && this->oEnd >= end))
            return true;
        else
            return false;
    }

    virtual bool isInside(const T &value) const
    {
        if(value.isNull() || !value.isValid())
            return false;
        if(this->oStart <= value && this->oEnd >= value)
            return true;
        else
            return false;
    }

    virtual bool isGreaterThan(const T &value) const
    {
        if(value.isNull() || !value.isValid())
            return false;
        if(this->oStart > value && this->oEnd > value)
            return true;
        else
            return false;
    }
};

#endif // DURATION_H
