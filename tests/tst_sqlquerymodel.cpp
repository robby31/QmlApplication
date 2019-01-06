#include "tst_sqlquerymodel.h"

void tst_sqlquerymodel::initTestCase()
{
    QSqlDatabase db = CREATE_DATABASE("QSQLITE", "TESTS");
    db.setDatabaseName(":memory:");
    QCOMPARE(db.open(), true);

    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS data (id INTEGER primary key, date DATE DEFAULT CURRENT_DATE, value REAL)");

    for (int index=0;index<1000;++index)
    {
        query.prepare("INSERT INTO data (value) VALUES (:value)");
        query.bindValue(":value", index);
        query.exec();
    }
}

void tst_sqlquerymodel::cleanupTestCase()
{

}

void tst_sqlquerymodel::testCase_SqlListModel()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    SqlListModel model;
    model.setConnectionName("TESTS");
    model.setQuery("SELECT * FROM data");

    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model.query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 1000);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);

    QCOMPARE(model.headerData(0, Qt::Horizontal), QString("id"));
    QCOMPARE(model.headerData(1, Qt::Horizontal), QString("date"));
    QCOMPARE(model.headerData(2, Qt::Horizontal), QString("value"));

    QCOMPARE(model.headerData(0, Qt::Vertical), 1);
    QCOMPARE(model.headerData(10, Qt::Vertical), 11);

    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 1);
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole), 1);
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole+1), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(0, 2), Qt::DisplayRole), 0);
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole+2), 0);

    QCOMPARE(model.data(model.index(1, 0), Qt::DisplayRole), 2);
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole), 2);
    QCOMPARE(model.data(model.index(1, 1), Qt::DisplayRole), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole+1), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(1, 2), Qt::DisplayRole), 1);
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole+2), 1);
}

void tst_sqlquerymodel::testCase_SqlListModel_remove()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    SqlListModel model;
    model.setConnectionName("TESTS");
    model.setTablename("data");
    model.setQuery("SELECT * FROM data");

    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model.tablename(), "data");
    QCOMPARE(model.query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 1000);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 1);

    QCOMPARE(model.remove(0), true);
    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model.query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 999);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 2);
}

void tst_sqlquerymodel::testCase_SqlQueryModel()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    SqlQueryModel model;
    model.setConnectionName("TESTS");
    model._setQuery("SELECT * FROM data");

    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model._query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 1000);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);

    QCOMPARE(model.headerData(0, Qt::Horizontal), QString("id"));
    QCOMPARE(model.headerData(1, Qt::Horizontal), QString("date"));
    QCOMPARE(model.headerData(2, Qt::Horizontal), QString("value"));

    QCOMPARE(model.headerData(0, Qt::Vertical), 1);
    QCOMPARE(model.headerData(10, Qt::Vertical), 11);

    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 1);
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole), 1);
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole+1), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(0, 2), Qt::DisplayRole), 0);
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole+2), 0);

    QCOMPARE(model.data(model.index(1, 0), Qt::DisplayRole), 2);
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole), 2);
    QCOMPARE(model.data(model.index(1, 1), Qt::DisplayRole), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole+1), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(1, 2), Qt::DisplayRole), 1);
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole+2), 1);
}

void tst_sqlquerymodel::testCase_SqlQueryModel_remove()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    SqlQueryModel model;
    model.setConnectionName("TESTS");
    model._setQuery("SELECT * FROM data");

    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model._query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 999);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 2);

    QCOMPARE(model.removeRow(0), false);   // cannot modify SqlQueryModel which is read-only
    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model._query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 999);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 2);
}

void tst_sqlquerymodel::testCase_SqlTableModel()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    SqlTableModel model("TESTS");
    model.setConnectionName("TESTS");
    model.setTable("data");
    model._setQuery("SELECT * FROM data");

    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model.tableName(), "data");
    QCOMPARE(model._query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 1000);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);

    QCOMPARE(model.headerData(0, Qt::Horizontal), QString("id"));
    QCOMPARE(model.headerData(1, Qt::Horizontal), QString("date"));
    QCOMPARE(model.headerData(2, Qt::Horizontal), QString("value"));

    QCOMPARE(model.headerData(0, Qt::Vertical), 1);
    QCOMPARE(model.headerData(10, Qt::Vertical), 11);

    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 1);
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole), 1);
    QCOMPARE(model.data(model.index(0, 1), Qt::DisplayRole), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole+1), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(0, 2), Qt::DisplayRole), 0);
    QCOMPARE(model.data(model.index(0, 0), Qt::UserRole+2), 0);

    QCOMPARE(model.data(model.index(1, 0), Qt::DisplayRole), 2);
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole), 2);
    QCOMPARE(model.data(model.index(1, 1), Qt::DisplayRole), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole+1), QDate::currentDate().toString("yyyy-MM-dd"));
    QCOMPARE(model.data(model.index(1, 2), Qt::DisplayRole), 1);
    QCOMPARE(model.data(model.index(1, 0), Qt::UserRole+2), 1);
}

void tst_sqlquerymodel::testCase_SqlTableModel_remove()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    SqlTableModel model("TESTS");
    model.setConnectionName("TESTS");
    model.setTable("data");
    model._setQuery("SELECT * FROM data");

    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model.tableName(), "data");
    QCOMPARE(model._query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 999);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);
    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 2);

    QCOMPARE(model.remove(0), true);
    QCOMPARE(model.connectionName(), "TESTS");
    QCOMPARE(model._query(), "SELECT * FROM data");
    QCOMPARE(model.rowCount(), 998);
    QCOMPARE(model.columnCount(), 3);
    QCOMPARE(model.roleNames().size(), 4);

    QCOMPARE(model.data(model.index(0, 0), Qt::DisplayRole), 3);
}

void tst_sqlquerymodel::testCase_SqlListModel_benchmark()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    QBENCHMARK
    {
        SqlListModel model;
        model.setConnectionName("TESTS");
        model.setQuery("SELECT * FROM data");
        QCOMPARE(model.rowCount(), 1000);
    }
}

void tst_sqlquerymodel::testCase_SqlQueryModel_benchmark()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    QBENCHMARK
    {
        SqlQueryModel model;
        model.setConnectionName("TESTS");
        model._setQuery("SELECT * FROM data");
        QCOMPARE(model.rowCount(), 1000);
    }
}

void tst_sqlquerymodel::testCase_SqlTableModel_benchmark()
{
    QSqlDatabase db = GET_DATABASE("TESTS");
    QCOMPARE(db.databaseName(), ":memory:");
    QCOMPARE(db.isOpen(), true);

    QBENCHMARK
    {
        SqlTableModel model("TESTS");
        model.setConnectionName("TESTS");
        model.setTable("data");
        model._setQuery("SELECT * FROM data");
        QCOMPARE(model.rowCount(), 1000);
    }
}
