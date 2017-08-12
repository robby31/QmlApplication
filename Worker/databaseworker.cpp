#include "databaseworker.h"

DatabaseWorker::DatabaseWorker(QObject *parent):
    Worker(parent)
{

}

void DatabaseWorker::createDatabase(const QString &driverName, const QString &connectionName)
{
    setdatabaseConnectionName(connectionName);
    CREATE_DATABASE(driverName, connectionName);
    qDebug() << this << "CREATE DATABASE" << driverName << connectionName;
}

void DatabaseWorker::databaseOptions(const QString &options)
{
    qDebug() << this << "set database options" << options;
    QSqlDatabase db = database();

    if (db.isValid())
        db.setConnectOptions(options);
    else
        qCritical() << this << "invalid database, unable to set options" << options;
}

void DatabaseWorker::databaseOpened(const QUrl &path)
{
    qDebug() << this << "OPEN DATABASE" << path;

    QSqlDatabase db = GET_DATABASE(databaseConnectionName());

    if (!db.isValid())
    {
        qCritical() << "invalid database" << db.driverName() << databaseConnectionName();
    }
    else
    {
        if (path.isLocalFile())
        {

            if (db.isOpen())
                db.close();

            db.setDatabaseName(path.toLocalFile());

            if (!db.open())
            {
                qCritical() << "unable to open database" << db.lastError().text();
            }
        }
        else
        {
            qCritical() << "invalid database pathname" << path;
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
