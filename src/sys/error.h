#ifndef ERROR_H
#define ERROR_H

#include <QObject>
#include <QString>

class Error : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
private:
    QString sError;
    bool bIsError;
public:
    explicit Error(QObject *parent = 0);
    Error(const QString &error, QObject *parent = 0);
    void setError(const QString &error);
    void setNoError();
    QString error() const;
    Q_INVOKABLE bool isError() const;
signals:
    void errorChanged();
};

#endif // ERROR_H
