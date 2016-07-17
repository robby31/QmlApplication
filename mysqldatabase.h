#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QThread>
#include <QSqlDatabase>

/*
 *   This header defines function to use Thread-safe database connection
 *
 *   A database connection shall only be used in the thread it has been created.
 *
 *   Connection name is initialized with the thread pointer where the database has been created.
 *   Then if a function request a database in another thread, the returned database will be invalid.
*/

// connection name contains pointer of the thread where it has been created
// this connection shall only be used in this thread
#define CONNECTION_NAME(name) QString("%1 in Thread %2").arg(name).arg(quintptr(QThread::currentThread()))


// returns database created in the current thread based on connection name
// if no connection name is found or if the connection has been established
//    in another thread an invalid connection is returned
#define GET_DATABASE(name) QSqlDatabase::database(CONNECTION_NAME(name))


// create database and define connectioName related to current Thread
#define CREATE_DATABASE(driverName, name) QSqlDatabase::addDatabase(driverName, CONNECTION_NAME(name))

// remove database
#define REMOVE_DATABASE(name) QSqlDatabase::removeDatabase(CONNECTION_NAME(name))

#endif // MYSQLDATABASE_H
