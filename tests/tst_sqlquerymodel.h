#ifndef TST_SQLQUERYMODEL_H
#define TST_SQLQUERYMODEL_H

#include <QObject>
#include <QtTest>
#include <QDebug>

#include "SqlModel/sqlquerymodel.h"
#include "SqlModel/sqltablemodel.h"
#include "Models/sqllistmodel.h"

class tst_sqlquerymodel : public QObject
{
    Q_OBJECT

public:
    tst_sqlquerymodel() = default;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testCase_SqlListModel();
    void testCase_SqlQueryModel();
    void testCase_SqlTableModel();

    void testCase_SqlListModel_benchmark();
    void testCase_SqlQueryModel_benchmark();
    void testCase_SqlTableModel_benchmark();

    void testCase_SqlListModel_remove();
    void testCase_SqlQueryModel_remove();
    void testCase_SqlTableModel_remove();
};

#endif // TST_SQLQUERYMODEL_H
