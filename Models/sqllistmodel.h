#ifndef SQLLISTMODEL_H
#define SQLLISTMODEL_H

#include "basesqllistmodel.h"
#include "filteringcolumnitem.h"
#include "checkedsqllistmodel.h"

class SqlListModel : public BaseSqlListModel
{
    Q_OBJECT

    Q_PROPERTY(ListModel *columnsToFilter READ columnsToFilter NOTIFY columnsToFilterChanged)
    Q_PROPERTY(CheckedSqlListModel *columnDataModel READ columnDataModel NOTIFY columnDataModelChanged)

public:
    explicit SqlListModel(QObject *parent = Q_NULLPTR);

    ListModel *columnsToFilter() const { return m_columnsToFilter; }

    CheckedSqlListModel *columnDataModel() const { return m_columnDataModel; }
    Q_INVOKABLE void setColumnDataModel(const QString &columnName);
    Q_INVOKABLE void addColumnToFilter(const QString &name);

signals:
    void columnsToFilterChanged();
    void columnDataModelChanged();
    void columnToFilterSelectedChanged();

public slots:
    void updateFilter();

private slots:
    void columnDataItemChanged(QModelIndex start, QModelIndex end);
    void updateColumnDataModel();

private:
    // configure filtering
    ListModel *m_columnsToFilter = Q_NULLPTR;
    CheckedSqlListModel *m_columnDataModel = Q_NULLPTR;
    QHash<QString, CheckedSqlListModel*> m_columnDataModels;
    FilteringColumnItem *m_columnToFilterSelected = Q_NULLPTR;
};

#endif // SQLLISTMODEL_H
