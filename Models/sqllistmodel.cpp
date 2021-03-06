#include "sqllistmodel.h"

SqlListModel::SqlListModel(QObject *parent) :
    BaseSqlListModel(parent)
{
    m_columnsToFilter = new ListModel(new FilteringColumnItem, this);

    connect(this, SIGNAL(queryChanged()), this, SLOT(updateColumnDataModel()));
}

void SqlListModel::addColumnToFilter(const QString &name)
{
    FilteringColumnItem *item;
    item = new FilteringColumnItem(name, m_columnsToFilter);
    m_columnsToFilter->appendRow(item);
}

void SqlListModel::columnDataItemChanged(QModelIndex start, QModelIndex end)
{
    Q_UNUSED(start)
    Q_UNUSED(end)

    auto item = qobject_cast<CheckedSqlListModel*>(sender());
    if (item)
    {
        bool columnFiltered = !item->checkedFilterCmd().isEmpty();
        if (m_columnToFilterSelected)
            m_columnToFilterSelected->setData(columnFiltered, FilteringColumnItem::UsedRole);
    }
}

void SqlListModel::updateColumnDataModel()
{
    for (auto column : m_columnDataModels)
    {
        if (column->checkedFilterCmd().isEmpty())
            column->setQueryData(query());
    }
}

void SqlListModel::setColumnDataModel(const QString &columnName)
{
    if (m_columnDataModels.contains(columnName))
    {
        m_columnDataModel = m_columnDataModels[columnName];
    }
    else
    {
        m_columnDataModels[columnName] = new CheckedSqlListModel(this);
        m_columnDataModels[columnName]->setConnectionName(connectionName());
        m_columnDataModels[columnName]->setTablename(tablename());
        m_columnDataModels[columnName]->setQueryData(query());
        m_columnDataModels[columnName]->setParameter(columnName);
        connect(m_columnDataModels[columnName], SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(columnDataItemChanged(QModelIndex,QModelIndex)));
        m_columnDataModel = m_columnDataModels[columnName];
    }

    m_columnToFilterSelected = Q_NULLPTR;
    for (int index=0;index<m_columnsToFilter->rowCount();++index)
    {
        auto item = qobject_cast<FilteringColumnItem*>(m_columnsToFilter->at(index));
        if (item)
        {
            QString name = item->data(FilteringColumnItem::NameRole).toString();
            if (name == columnName)
            {
                m_columnToFilterSelected = item;
                break;
            }
        }
    }

    emit columnDataModelChanged();
}

void SqlListModel::updateFilter()
{
    QStringList filterCmd;

    for (auto column : m_columnDataModels)
    {
        QString cmd = column->checkedFilterCmd();
        if (!cmd.isEmpty())
            filterCmd << cmd;
    }

    setFilter(filterCmd.join(" and "));
}
