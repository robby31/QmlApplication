#ifndef SQLLISTMODEL_H
#define SQLLISTMODEL_H

#include "libqmlapplication_global.h"
#include "basesqllistmodel.h"
#include "filteringcolumnitem.h"
#include "checkedsqllistmodel.h"

class QMLAPPLICATIONSHARED_EXPORT SqlListModel : public BaseSqlListModel
{
    Q_OBJECT

    Q_PROPERTY(ListModel *columnsToFilter READ columnsToFilter NOTIFY columnsToFilterChanged)
    Q_PROPERTY(CheckedSqlListModel *columnDataModel READ columnDataModel NOTIFY columnDataModelChanged)

public:
    explicit SqlListModel(QObject *parent = 0);

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
    ListModel *m_columnsToFilter;
    CheckedSqlListModel *m_columnDataModel;
    QHash<QString, CheckedSqlListModel*> m_columnDataModels;
    FilteringColumnItem *m_columnToFilterSelected;
};

#endif // SQLLISTMODEL_H
