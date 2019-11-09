#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QUrl>
#include <QSqlError>
#include "mysqldatabase.h"
#include "worker.h"

class DatabaseWorker : public Worker
{
    Q_OBJECT

public:
    explicit DatabaseWorker(QObject *parent = Q_NULLPTR);

    QString databaseConnectionName() const;
    void setdatabaseConnectionName(const QString &name);

    QSqlDatabase database() const;

private slots:
    void createDatabase(const QString &driverName, const QString &connectionName);
    void databaseOptions(const QString &options);
    void databaseOpened(const QString &databaseName);

private:
    QString m_databaseConnectionName;
};

#endif // DATABASEWORKER_H
