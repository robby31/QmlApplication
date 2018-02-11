#include "databaseworker.h"

DatabaseWorker::DatabaseWorker(QObject *parent):
    Worker(parent)
{

}

void DatabaseWorker::createDatabase(const QString &driverName, const QString &connectionName)
{
    setdatabaseConnectionName(connectionName);
    CREATE_DATABASE(driverName, connectionName);
    qDebug() << QThread::currentThread() << "CREATE DATABASE" << driverName << connectionName;
}

void DatabaseWorker::databaseOptions(const QString &options)
{
    qDebug() << QThread::currentThread() << "set database options" << options;
    QSqlDatabase db = database();

    if (db.isValid())
        db.setConnectOptions(options);
    else
        qCritical() << QThread::currentThread() << "invalid database, unable to set options" << options;
}

void DatabaseWorker::databaseOpened(const QString &databaseName)
{
    qDebug() << QThread::currentThread() << "OPEN DATABASE" << databaseName;

    QSqlDatabase db = GET_DATABASE(databaseConnectionName());

    if (!db.isValid())
    {
        qCritical() << "invalid database" << db.driverName() << databaseConnectionName();
    }
    else
    {
        if (db.isOpen())
            db.close();

        db.setDatabaseName(databaseName);

        if (!db.open())
        {
            qCritical() << "unable to open database" << db.lastError().text();
        }
    }
}

QString DatabaseWorker::databaseConnectionName() const
{
    return m_databaseConnectionName;
}

void DatabaseWorker::setdatabaseConnectionName(const QString &name)
{
    if (!m_databaseConnectionName.isEmpty())
        qWarning() << "current database conncection will be lost.";

    m_databaseConnectionName = name;
}

QSqlDatabase DatabaseWorker::database() const
{
    return GET_DATABASE(databaseConnectionName());
}
